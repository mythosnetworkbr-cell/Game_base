extends Node3D

const SERVER_PORT := 7777
const DEFAULT_SERVER := "127.0.0.1"
const WORLD_SIZE := 260.0
const ROAD_WIDTH := 14.0

var peer: ENetMultiplayerPeer
var local_player: CharacterBody3D
var players: Dictionary = {}
var menu: Control
var hud: Control
var status_label: Label
var server_edit: LineEdit
var connected := false
var input_state := {"forward": false, "back": false, "left": false, "right": false, "run": false}
var touch_origin := Vector2.ZERO
var touch_active := false
var last_sync := 0.0

func _ready() -> void:
    _build_world()
    _build_menu()
    multiplayer.peer_connected.connect(_on_peer_connected)
    multiplayer.peer_disconnected.connect(_on_peer_disconnected)

func _process(delta: float) -> void:
    if local_player and connected:
        _update_player(delta)
        last_sync += delta
        if last_sync >= 0.05:
            last_sync = 0.0
            _send_state()
    _update_camera(delta)

func _build_world() -> void:
    var env := WorldEnvironment.new()
    var environment := Environment.new()
    environment.background_mode = Environment.BG_SKY
    var sky := Sky.new()
    var sky_mat := ProceduralSkyMaterial.new()
    sky_mat.sky_top_color = Color("#151a35")
    sky_mat.sky_horizon_color = Color("#f0a66a")
    sky_mat.ground_bottom_color = Color("#10121d")
    sky_mat.ground_horizon_color = Color("#b96858")
    sky.sky_material = sky_mat
    environment.sky = sky
    environment.ambient_light_source = Environment.AMBIENT_SOURCE_SKY
    environment.ambient_light_energy = 0.75
    environment.tonemap_mode = Environment.TONE_MAPPER_ACES
    environment.fog_enabled = true
    environment.fog_light_color = Color("#9a7892")
    environment.fog_density = 0.006
    env.environment = environment
    add_child(env)

    var sun := DirectionalLight3D.new()
    sun.rotation_degrees = Vector3(-48, -28, 0)
    sun.light_energy = 1.8
    sun.shadow_enabled = true
    sun.directional_shadow_max_distance = 180.0
    add_child(sun)

    _add_box("Ground", Vector3(WORLD_SIZE, 0.4, WORLD_SIZE), Vector3(0, -0.2, 0), Color("#252833"))
    _add_box("Beach", Vector3(55, 0.3, WORLD_SIZE), Vector3(-102, 0.05, 0), Color("#c9a879"))
    _add_box("Water", Vector3(45, 0.15, WORLD_SIZE), Vector3(-150, 0.0, 0), Color("#1b6682"))

    for x in range(-100, 101, 40):
        _add_box("RoadX", Vector3(ROAD_WIDTH, 0.08, WORLD_SIZE), Vector3(x, 0.05, 0), Color("#171922"))
        _add_box("RoadXLine", Vector3(0.22, 0.1, WORLD_SIZE), Vector3(x, 0.11, 0), Color("#e4c85c"))
    for z in range(-100, 101, 40):
        _add_box("RoadZ", Vector3(WORLD_SIZE, 0.08, ROAD_WIDTH), Vector3(0, 0.05, z), Color("#171922"))
        _add_box("RoadZLine", Vector3(WORLD_SIZE, 0.1, 0.22), Vector3(0, 0.11, z), Color("#e4c85c"))

    var rng := RandomNumberGenerator.new()
    rng.seed = 80415
    for x in range(-100, 101, 20):
        for z in range(-100, 101, 20):
            if abs(x) % 40 < 18 and abs(z) % 40 < 18:
                continue
            var h := rng.randf_range(8.0, 32.0)
            var w := rng.randf_range(8.0, 15.0)
            var d := rng.randf_range(8.0, 15.0)
            var color := Color.from_hsv(rng.randf_range(0.55, 0.95), rng.randf_range(0.08, 0.32), rng.randf_range(0.22, 0.58))
            _add_building(Vector3(x + rng.randf_range(-3,3), h * 0.5, z + rng.randf_range(-3,3)), Vector3(w, h, d), color)

    for i in range(24):
        var px := -100.0 + float(i % 8) * 28.0
        var pz := -105.0 + float(i / 8) * 70.0
        _add_palm(Vector3(px, 0, pz))

    for i in range(10):
        var vx := -90.0 + float(i % 5) * 45.0
        var vz := -105.0 + float(i / 5) * 210.0
        _add_car(Vector3(vx, 0.65, vz), Color.from_hsv(float(i) / 10.0, 0.65, 0.85))

    _add_bridge()

func _add_box(label: String, size: Vector3, pos: Vector3, color: Color) -> MeshInstance3D:
    var mesh := BoxMesh.new()
    mesh.size = size
    var node := MeshInstance3D.new()
    node.name = label
    node.mesh = mesh
    node.position = pos
    node.material_override = _material(color, 0.9)
    add_child(node)
    var body := StaticBody3D.new()
    body.position = pos
    var shape := CollisionShape3D.new()
    var box := BoxShape3D.new()
    box.size = size
    shape.shape = box
    body.add_child(shape)
    add_child(body)
    return node

func _add_building(pos: Vector3, size: Vector3, color: Color) -> void:
    var mesh := BoxMesh.new()
    mesh.size = size
    var node := MeshInstance3D.new()
    node.mesh = mesh
    node.position = pos
    node.material_override = _material(color, 0.72)
    add_child(node)
    var body := StaticBody3D.new()
    body.position = pos
    var collision := CollisionShape3D.new()
    var shape := BoxShape3D.new()
    shape.size = size
    collision.shape = shape
    body.add_child(collision)
    add_child(body)
    for floor in range(1, int(size.y / 4.0)):
        var window := MeshInstance3D.new()
        var wm := BoxMesh.new()
        wm.size = Vector3(0.08, 1.0, min(size.z * 0.55, 6.0))
        window.mesh = wm
        window.position = pos + Vector3(size.x * 0.505, -size.y * 0.5 + floor * 4.0, 0)
        window.material_override = _material(Color("#f5d88a"), 0.3, true)
        add_child(window)

func _add_palm(pos: Vector3) -> void:
    var trunk := CylinderMesh.new()
    trunk.top_radius = 0.28
    trunk.bottom_radius = 0.48
    trunk.height = 6.0
    var t := MeshInstance3D.new()
    t.mesh = trunk
    t.position = pos + Vector3(0, 3, 0)
    t.material_override = _material(Color("#6d4428"), 0.95)
    add_child(t)
    for i in range(7):
        var leaf := BoxMesh.new()
        leaf.size = Vector3(0.35, 0.25, 4.8)
        var l := MeshInstance3D.new()
        l.mesh = leaf
        l.position = pos + Vector3(0, 6.0, 0)
        l.rotation_degrees = Vector3(-12, float(i) * 51.4, 0)
        l.material_override = _material(Color("#2f7b4b"), 0.9)
        add_child(l)

func _add_car(pos: Vector3, color: Color) -> void:
    var car := MeshInstance3D.new()
    var body := BoxMesh.new()
    body.size = Vector3(3.8, 1.0, 7.2)
    car.mesh = body
    car.position = pos
    car.material_override = _material(color, 0.35)
    add_child(car)
    var roof := MeshInstance3D.new()
    var r := BoxMesh.new()
    r.size = Vector3(3.0, 0.9, 3.5)
    roof.mesh = r
    roof.position = pos + Vector3(0, 0.85, 0.2)
    roof.material_override = _material(Color("#20242f"), 0.35)
    add_child(roof)

func _add_bridge() -> void:
    _add_box("BridgeDeck", Vector3(24, 1.0, 140), Vector3(-120, 8, 0), Color("#4b4e58"))
    for x in [-132.0, -108.0]:
        _add_box("BridgePillar", Vector3(3, 16, 3), Vector3(x, 0, 0), Color("#77727a"))
    for z in range(-60, 61, 20):
        var cable := MeshInstance3D.new()
        var c := CylinderMesh.new()
        c.top_radius = 0.08
        c.bottom_radius = 0.08
        c.height = 18
        cable.mesh = c
        cable.position = Vector3(-120, 10, z)
        cable.rotation_degrees = Vector3(0, 90, 0)
        cable.material_override = _material(Color("#292b34"), 0.8)
        add_child(cable)

func _material(color: Color, roughness: float, emission := false) -> StandardMaterial3D:
    var m := StandardMaterial3D.new()
    m.albedo_color = color
    m.roughness = roughness
    if emission:
        m.emission_enabled = true
        m.emission = color
        m.emission_energy_multiplier = 2.0
    return m

func _build_menu() -> void:
    menu = Control.new()
    menu.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
    menu.mouse_filter = Control.MOUSE_FILTER_IGNORE
    add_child(menu)

    var panel := ColorRect.new()
    panel.color = Color(0.025, 0.02, 0.05, 0.72)
    panel.position = Vector2(0, 0)
    panel.size = Vector2(430, 720)
    menu.add_child(panel)

    var title := Label.new()
    title.text = "NYX\nROLEPLAY"
    title.position = Vector2(42, 62)
    title.add_theme_font_size_override("font_size", 52)
    title.add_theme_color_override("font_color", Color("#e7d5ff"))
    menu.add_child(title)

    var subtitle := Label.new()
    subtitle.text = "CIDADE VIVA • 3D MOBILE"
    subtitle.position = Vector2(46, 190)
    subtitle.add_theme_font_size_override("font_size", 15)
    subtitle.add_theme_color_override("font_color", Color("#bca8d8"))
    menu.add_child(subtitle)

    var info := Label.new()
    info.text = "Conecte-se à cidade e entre no mundo NYX.\nPersonagem, veículos, empregos e multiplayer."
    info.position = Vector2(46, 230)
    info.size = Vector2(330, 90)
    info.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
    info.add_theme_font_size_override("font_size", 16)
    info.add_theme_color_override("font_color", Color("#eeeeee"))
    menu.add_child(info)

    server_edit = LineEdit.new()
    server_edit.placeholder_text = "IP do servidor"
    server_edit.text = DEFAULT_SERVER
    server_edit.position = Vector2(46, 350)
    server_edit.size = Vector2(330, 58)
    server_edit.add_theme_font_size_override("font_size", 18)
    menu.add_child(server_edit)

    var play := Button.new()
    play.text = "ENTRAR NA CIDADE"
    play.position = Vector2(46, 430)
    play.size = Vector2(330, 70)
    play.add_theme_font_size_override("font_size", 21)
    play.pressed.connect(_connect_to_server)
    menu.add_child(play)

    status_label = Label.new()
    status_label.text = "Offline • pronto para conectar"
    status_label.position = Vector2(46, 520)
    status_label.size = Vector2(330, 70)
    status_label.autowrap_mode = TextServer.AUTOWRAP_WORD_SMART
    status_label.add_theme_color_override("font_color", Color("#c9a7ff"))
    menu.add_child(status_label)

func _connect_to_server() -> void:
    var host := server_edit.text.strip_edges()
    if host.is_empty():
        host = DEFAULT_SERVER
    status_label.text = "Conectando a %s:%d..." % [host, SERVER_PORT]
    peer = ENetMultiplayerPeer.new()
    var err := peer.create_client(host, SERVER_PORT)
    if err != OK:
        status_label.text = "Falha ao iniciar conexão: %s" % err
        return
    multiplayer.multiplayer_peer = peer
    connected = true
    await get_tree().create_timer(0.5).timeout
    if multiplayer.get_unique_id() != 1:
        _create_local_player(multiplayer.get_unique_id())
        menu.visible = false
        _build_hud()

func _start_server() -> void:
    peer = ENetMultiplayerPeer.new()
    var err := peer.create_server(SERVER_PORT, 100)
    if err != OK:
        push_error("Servidor não iniciou: %s" % err)
        return
    multiplayer.multiplayer_peer = peer
    connected = true

func _on_peer_connected(id: int) -> void:
    if multiplayer.is_server():
        _server_spawn_for(id)

func _on_peer_disconnected(id: int) -> void:
    if players.has(id):
        players[id].queue_free()
        players.erase(id)

func _server_spawn_for(id: int) -> void:
    spawn_remote.rpc(id, Vector3(0, 1.2, 0))

@rpc("authority", "call_local", "reliable")
func spawn_remote(id: int, pos: Vector3) -> void:
    if players.has(id):
        return
    _create_player(id, pos)

func _create_local_player(id: int) -> void:
    if players.has(id):
        local_player = players[id]
        return
    _create_player(id, Vector3(0, 1.2, 20))

func _create_player(id: int, pos: Vector3) -> void:
    var p := CharacterBody3D.new()
    p.name = "Player_%d" % id
    p.position = pos
    add_child(p)
    var collider := CollisionShape3D.new()
    var capsule := CapsuleShape3D.new()
    capsule.radius = 0.42
    capsule.height = 1.8
    collider.shape = capsule
    collider.position.y = 0.9
    p.add_child(collider)
    var body := MeshInstance3D.new()
    var bm := CapsuleMesh.new()
    bm.radius = 0.42
    bm.height = 1.25
    body.mesh = bm
    body.position.y = 0.9
    body.material_override = _material(Color("#244f9a"), 0.78)
    p.add_child(body)
    var head := MeshInstance3D.new()
    var hm := SphereMesh.new()
    hm.radius = 0.31
    hm.height = 0.62
    head.mesh = hm
    head.position.y = 1.75
    head.material_override = _material(Color("#d69b76"), 0.9)
    p.add_child(head)
    var name_tag := Label3D.new()
    name_tag.text = "NYX_%d" % id
    name_tag.position.y = 2.35
    name_tag.modulate = Color("#ffffff")
    name_tag.outline_size = 8
    p.add_child(name_tag)
    players[id] = p
    if id == multiplayer.get_unique_id():
        local_player = p
        _add_camera(p)

func _add_camera(target: Node3D) -> void:
    var cam := Camera3D.new()
    cam.name = "ThirdPersonCamera"
    cam.position = Vector3(0, 4.8, 7.2)
    cam.rotation_degrees = Vector3(-13, 180, 0)
    target.add_child(cam)
    cam.current = true

func _update_camera(delta: float) -> void:
    if not local_player:
        return
    var cam := local_player.get_node_or_null("ThirdPersonCamera") as Camera3D
    if cam:
        cam.position = cam.position.lerp(Vector3(0, 4.8, 7.2), delta * 6.0)
        cam.look_at(local_player.global_position + Vector3(0, 1.0, 0), Vector3.UP)

func _update_player(delta: float) -> void:
    if not local_player:
        return
    var dir := Vector3.ZERO
    if input_state.forward: dir.z -= 1.0
    if input_state.back: dir.z += 1.0
    if input_state.left: dir.x -= 1.0
    if input_state.right: dir.x += 1.0
    if dir.length() > 0.1:
        dir = dir.normalized()
        local_player.velocity.x = dir.x * (7.0 if input_state.run else 4.0)
        local_player.velocity.z = dir.z * (7.0 if input_state.run else 4.0)
        local_player.rotation.y = lerp_angle(local_player.rotation.y, atan2(-dir.x, -dir.z), delta * 8.0)
    else:
        local_player.velocity.x = move_toward(local_player.velocity.x, 0, 18 * delta)
        local_player.velocity.z = move_toward(local_player.velocity.z, 0, 18 * delta)
    if not local_player.is_on_floor():
        local_player.velocity.y -= 22.0 * delta
    else:
        local_player.velocity.y = -0.5
    local_player.move_and_slide()

func _send_state() -> void:
    if local_player and connected:
        sync_state.rpc(local_player.global_position, local_player.rotation.y)

@rpc("any_peer", "unreliable")
func sync_state(pos: Vector3, rot: float) -> void:
    var sender := multiplayer.get_remote_sender_id()
    if sender == multiplayer.get_unique_id():
        return
    if players.has(sender):
        players[sender].global_position = pos
        players[sender].rotation.y = rot

func _build_hud() -> void:
    if hud:
        hud.queue_free()
    hud = Control.new()
    hud.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
    add_child(hud)
    var top := Label.new()
    top.text = "NYX ROLEPLAY    |    $ 1.500    |    BANCO $ 5.000    |    NX 100"
    top.position = Vector2(28, 24)
    top.add_theme_font_size_override("font_size", 20)
    top.add_theme_color_override("font_color", Color.WHITE)
    hud.add_child(top)
    var chat := Label.new()
    chat.text = "[Cidade] Bem-vindo à NYX. Pressione E perto de pontos de interação."
    chat.position = Vector2(28, 650)
    chat.add_theme_font_size_override("font_size", 16)
    chat.add_theme_color_override("font_color", Color("#d9d0e5"))
    hud.add_child(chat)
    _add_touch_button("◀", Vector2(36, 530), "left")
    _add_touch_button("▶", Vector2(170, 530), "right")
    _add_touch_button("▲", Vector2(103, 465), "forward")
    _add_touch_button("▼", Vector2(103, 595), "back")
    _add_touch_button("CORRER", Vector2(1040, 540), "run")
    _add_touch_button("PULAR", Vector2(1040, 455), "jump")

func _add_touch_button(text: String, pos: Vector2, action: String) -> void:
    var b := Button.new()
    b.text = text
    b.position = pos
    b.size = Vector2(112, 64)
    b.modulate = Color(1,1,1,0.78)
    b.add_theme_font_size_override("font_size", 16)
    b.button_down.connect(func(): _set_action(action, true))
    b.button_up.connect(func(): _set_action(action, false))
    hud.add_child(b)

func _set_action(action: String, pressed: bool) -> void:
    if action == "jump" and pressed and local_player and local_player.is_on_floor():
        local_player.velocity.y = 8.0
        return
    if input_state.has(action):
        input_state[action] = pressed

func _unhandled_input(event: InputEvent) -> void:
    if event is InputEventKey and event.pressed:
        if event.keycode == KEY_F1:
            _start_server()
        elif event.keycode == KEY_ESCAPE:
            menu.visible = true
    if event is InputEventScreenTouch:
        if event.pressed:
            touch_origin = event.position
            touch_active = true
        else:
            touch_active = false
            input_state.forward = false
            input_state.back = false
            input_state.left = false
            input_state.right = false
    if event is InputEventScreenDrag and touch_active:
        var d := event.position - touch_origin
        input_state.forward = d.y < -35
        input_state.back = d.y > 35
        input_state.left = d.x < -35
        input_state.right = d.x > 35

func _notification(what: int) -> void:
    if what == NOTIFICATION_WM_CLOSE_REQUEST:
        if multiplayer.multiplayer_peer:
            multiplayer.multiplayer_peer.close()
        get_tree().quit()
