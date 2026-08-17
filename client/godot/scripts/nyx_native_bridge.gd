extends Node

## Adapter boundary between Godot and the Android/native client bridge.
## It intentionally does not implement SA-MP/RakNet. A native plugin can call
## apply_* methods with the same state contract defined in NYX_MOBILE_PROTOCOL_V1.

signal player_state_received(state: Dictionary)
signal vehicle_state_received(state: Dictionary)
signal vehicle_created(state: Dictionary)
signal vehicle_removed(vehicle_id: int)
signal chat_received(message: String)
signal connection_changed(connected: bool, reason: String)

var available := false

func _ready() -> void:
    # A real Android plugin can expose an Engine singleton named NyxNativeBridge.
    available = Engine.has_singleton("NyxNativeBridge")

func is_available() -> bool:
    return available

func apply_player_state(state: Dictionary) -> void:
    player_state_received.emit(state)

func apply_vehicle_state(state: Dictionary) -> void:
    vehicle_state_received.emit(state)

func apply_vehicle_created(state: Dictionary) -> void:
    vehicle_created.emit(state)

func apply_vehicle_removed(vehicle_id: int) -> void:
    vehicle_removed.emit(vehicle_id)

func apply_chat(message: String) -> void:
    chat_received.emit(message)

func apply_connection(connected: bool, reason: String = "") -> void:
    connection_changed.emit(connected, reason)
