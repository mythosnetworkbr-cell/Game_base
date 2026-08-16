extends Node

## NYX Mobile synchronization layer.
## This is deliberately transport-agnostic: the current prototype uses Godot RPC,
## while the future SA-MP/RakNet bridge can feed the same state contract.

const TICK_RATE := 20.0
const SNAPSHOT_RATE := 10.0
const INTERPOLATION := 12.0
const MAX_VEHICLES := 256

var accumulator := 0.0
var snapshot_accumulator := 0.0
var player_states: Dictionary = {}
var vehicle_states: Dictionary = {}
var vehicle_nodes: Dictionary = {}
var local_vehicle_id := -1
var sequence := 0

func _ready() -> void:
    set_process(true)

func _process(delta: float) -> void:
    accumulator += delta
    snapshot_accumulator += delta
    _interpolate_remote_players(delta)
    _interpolate_remote_vehicles(delta)
    if accumulator >= 1.0 / TICK_RATE:
        accumulator = 0.0
        _publish_local_player()
    if snapshot_accumulator >= 1.0 / SNAPSHOT_RATE:
        snapshot_accumulator = 0.0
        _publish_local_vehicle()

func _root() -> Node:
    return get_parent()

func _local_player() -> Node3D:
    var root := _root()
    if root == null:
        return null
    return root.get("local_player") as Node3D

func _publish_local_player() -> void:
    var player := _local_player()
    if player == null or not multiplayer.multiplayer_peer:
        return
    sequence += 1
    player_state.rpc(player.global_position, player.rotation.y, sequence)

@rpc("any_peer", "unreliable_ordered", 0)
func player_state(pos: Vector3, yaw: float, seq: int) -> void:
    var sender := multiplayer.get_remote_sender_id()
    if sender <= 0 or sender == multiplayer.get_unique_id():
        return
    var previous: Dictionary = player_states.get(sender, {})
    if previous.has("seq") and seq <= int(previous.seq):
        return
    player_states[sender] = {
        "position": pos,
        "yaw": yaw,
        "seq": seq
    }

func _interpolate_remote_players(delta: float) -> void:
    var root := _root()
    if root == null:
        return
    var players: Dictionary = root.get("players")
    for id in player_states.keys():
        if not players.has(id):
            continue
        var node := players[id] as Node3D
        var state: Dictionary = player_states[id]
        node.global_position = node.global_position.lerp(state.position, min(1.0, delta * INTERPOLATION))
        node.rotation.y = lerp_angle(node.rotation.y, float(state.yaw), min(1.0, delta * INTERPOLATION))

## Register a vehicle with an authoritative network id. The actual GTA model,
## handling and native vehicle object remain owned by the game/client bridge.
func register_vehicle(vehicle_id: int, model_id: int, position: Vector3, yaw: float = 0.0) -> bool:
    if vehicle_id < 0 or vehicle_id >= MAX_VEHICLES:
        return false
    vehicle_states[vehicle_id] = {
        "model": model_id,
        "position": position,
        "yaw": yaw,
        "velocity": Vector3.ZERO,
        "driver": -1,
        "seq": 0
    }
    return true

func set_vehicle_state(vehicle_id: int, position: Vector3, yaw: float, velocity: Vector3 = Vector3.ZERO, driver: int = -1) -> void:
    if not vehicle_states.has(vehicle_id):
        register_vehicle(vehicle_id, 0, position, yaw)
    var state: Dictionary = vehicle_states[vehicle_id]
    state.position = position
    state.yaw = yaw
    state.velocity = velocity
    state.driver = driver
    state.seq = int(state.seq) + 1
    vehicle_states[vehicle_id] = state

func remove_vehicle(vehicle_id: int) -> void:
    vehicle_states.erase(vehicle_id)
    if vehicle_nodes.has(vehicle_id):
        var node := vehicle_nodes[vehicle_id] as Node
        if is_instance_valid(node):
            node.queue_free()
        vehicle_nodes.erase(vehicle_id)

func _publish_local_vehicle() -> void:
    if local_vehicle_id < 0 or not vehicle_states.has(local_vehicle_id):
        return
    var state: Dictionary = vehicle_states[local_vehicle_id]
    vehicle_state.rpc(local_vehicle_id, int(state.model), state.position, float(state.yaw), state.velocity, int(state.driver), int(state.seq))

@rpc("any_peer", "unreliable_ordered", 1)
func vehicle_state(vehicle_id: int, model_id: int, pos: Vector3, yaw: float, velocity: Vector3, driver: int, seq: int) -> void:
    var sender := multiplayer.get_remote_sender_id()
    if sender <= 0 or vehicle_id < 0 or vehicle_id >= MAX_VEHICLES:
        return
    var previous: Dictionary = vehicle_states.get(vehicle_id, {})
    if previous.has("seq") and seq <= int(previous.seq):
        return
    vehicle_states[vehicle_id] = {
        "model": model_id,
        "position": pos,
        "yaw": yaw,
        "velocity": velocity,
        "driver": driver,
        "seq": seq,
        "authority": sender
    }
    _ensure_vehicle_node(vehicle_id, model_id, pos, yaw)

func _ensure_vehicle_node(vehicle_id: int, model_id: int, pos: Vector3, yaw: float) -> void:
    if vehicle_nodes.has(vehicle_id):
        return
    var root := _root()
    if root == null:
        return
    var vehicle := Node3D.new()
    vehicle.name = "Vehicle_%d" % vehicle_id
    vehicle.position = pos
    vehicle.rotation.y = yaw
    root.add_child(vehicle)
    var body := MeshInstance3D.new()
    var mesh := BoxMesh.new()
    mesh.size = Vector3(1.9, 0.65, 3.6)
    body.mesh = mesh
    body.material_override = _vehicle_material(model_id)
    body.position.y = 0.55
    vehicle.add_child(body)
    var collision := CollisionShape3D.new()
    var shape := BoxShape3D.new()
    shape.size = Vector3(1.9, 1.1, 3.6)
    collision.shape = shape
    collision.position.y = 0.55
    var physics := StaticBody3D.new()
    physics.name = "VehicleCollision"
    physics.add_child(collision)
    vehicle.add_child(physics)
    vehicle_nodes[vehicle_id] = vehicle

func _vehicle_material(model_id: int) -> StandardMaterial3D:
    var material := StandardMaterial3D.new()
    var hue := fmod(float(model_id) * 0.071, 1.0)
    material.albedo_color = Color.from_hsv(hue, 0.68, 0.82)
    material.roughness = 0.35
    return material

func _interpolate_remote_vehicles(delta: float) -> void:
    for id in vehicle_states.keys():
        if not vehicle_nodes.has(id):
            continue
        var node := vehicle_nodes[id] as Node3D
        if not is_instance_valid(node):
            vehicle_nodes.erase(id)
            continue
        var state: Dictionary = vehicle_states[id]
        node.global_position = node.global_position.lerp(state.position, min(1.0, delta * INTERPOLATION))
        node.rotation.y = lerp_angle(node.rotation.y, float(state.yaw), min(1.0, delta * INTERPOLATION))

func request_vehicle_spawn(vehicle_id: int, model_id: int, pos: Vector3, yaw: float = 0.0) -> void:
    register_vehicle(vehicle_id, model_id, pos, yaw)
    vehicle_spawn.rpc(vehicle_id, model_id, pos, yaw)

@rpc("any_peer", "reliable")
func vehicle_spawn(vehicle_id: int, model_id: int, pos: Vector3, yaw: float) -> void:
    if vehicle_id < 0 or vehicle_id >= MAX_VEHICLES:
        return
    register_vehicle(vehicle_id, model_id, pos, yaw)
    _ensure_vehicle_node(vehicle_id, model_id, pos, yaw)

func request_vehicle_remove(vehicle_id: int) -> void:
    vehicle_remove.rpc(vehicle_id)

@rpc("any_peer", "reliable")
func vehicle_remove(vehicle_id: int) -> void:
    remove_vehicle(vehicle_id)
