extends CanvasLayer

## Native-feeling touch layer for the NYX mobile prototype.
## It drives the existing ui_* actions so the same player controller works on
## desktop and Android without duplicating movement logic.

const JOYSTICK_RADIUS := 86.0
const KNOB_RADIUS := 34.0

var joystick_center := Vector2.ZERO
var joystick_touch := -1
var sprint_touch := -1
var action_touch := -1
var knob: ColorRect
var joystick: ColorRect
var sprint_button: Button
var action_button: Button

func _ready() -> void:
    layer = 30
    _build()

func _build() -> void:
    var root := Control.new()
    root.set_anchors_and_offsets_preset(Control.PRESET_FULL_RECT)
    root.mouse_filter = Control.MOUSE_FILTER_IGNORE
    add_child(root)

    joystick = ColorRect.new()
    joystick.position = Vector2(54, 540)
    joystick.size = Vector2(172, 172)
    joystick.color = Color(0.12, 0.12, 0.16, 0.34)
    joystick.mouse_filter = Control.MOUSE_FILTER_IGNORE
    root.add_child(joystick)

    knob = ColorRect.new()
    knob.size = Vector2(KNOB_RADIUS * 2.0, KNOB_RADIUS * 2.0)
    knob.color = Color(0.72, 0.55, 1.0, 0.75)
    knob.mouse_filter = Control.MOUSE_FILTER_IGNORE
    joystick.add_child(knob)
    _reset_knob()

    sprint_button = _button(root, "CORRER", Vector2(1040, 570), Vector2(150, 68))
    action_button = _button(root, "AÇÃO", Vector2(1040, 650), Vector2(150, 56))

func _button(root: Control, text: String, pos: Vector2, size: Vector2) -> Button:
    var b := Button.new()
    b.text = text
    b.position = pos
    b.size = size
    b.modulate = Color(0.8, 0.7, 1.0, 0.88)
    b.mouse_default_cursor_shape = Control.CURSOR_POINTING_HAND
    root.add_child(b)
    if text == "CORRER":
        b.button_down.connect(func(): _set_action("run", true))
        b.button_up.connect(func(): _set_action("run", false))
    else:
        b.button_down.connect(func(): Input.action_press("ui_accept"))
        b.button_up.connect(func(): Input.action_release("ui_accept"))
    return b

func _set_action(name: String, pressed: bool) -> void:
    if pressed:
        Input.action_press("ui_%s" % name)
    else:
        Input.action_release("ui_%s" % name)

func _input(event: InputEvent) -> void:
    if event is InputEventScreenTouch:
        if event.pressed:
            if event.position.x < 300 and event.position.y > 470 and joystick_touch == -1:
                joystick_touch = event.index
                joystick_center = Vector2(140, 626)
                _update_joystick(event.position)
        elif event.index == joystick_touch:
            joystick_touch = -1
            _release_movement()
            _reset_knob()
    elif event is InputEventScreenDrag and event.index == joystick_touch:
        _update_joystick(event.position)

func _update_joystick(screen_pos: Vector2) -> void:
    var delta := screen_pos - joystick_center
    if delta.length() > JOYSTICK_RADIUS:
        delta = delta.normalized() * JOYSTICK_RADIUS
    knob.position = Vector2(86, 86) + delta - Vector2(KNOB_RADIUS, KNOB_RADIUS)
    var x := delta.x / JOYSTICK_RADIUS
    var y := delta.y / JOYSTICK_RADIUS
    _axis("ui_left", x < -0.25)
    _axis("ui_right", x > 0.25)
    _axis("ui_up", y < -0.25)
    _axis("ui_down", y > 0.25)

func _axis(action: String, pressed: bool) -> void:
    if pressed:
        Input.action_press(action)
    else:
        Input.action_release(action)

func _release_movement() -> void:
    for action in ["ui_left", "ui_right", "ui_up", "ui_down"]:
        Input.action_release(action)

func _reset_knob() -> void:
    knob.position = Vector2(86 - KNOB_RADIUS, 86 - KNOB_RADIUS)
