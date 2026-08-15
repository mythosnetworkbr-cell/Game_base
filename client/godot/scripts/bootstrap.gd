extends Node

func _ready() -> void:
    var root := get_parent()
    if root == null:
        return
    if "--server" in OS.get_cmdline_user_args() or "--server" in OS.get_cmdline_args():
        root.call("_start_server")
        var menu = root.get("menu")
        if menu:
            menu.visible = false
