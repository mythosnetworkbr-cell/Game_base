extends Node

## Small client-side session service. It stores only non-sensitive connection
## preferences and exposes a stable state object to HUD/native adapters.

const SAVE_PATH := "user://nyx_mobile.cfg"

var server_host := "127.0.0.1"
var server_port := 7777
var character_name := "Cidadão"
var last_job_id := -1
var last_organization_id := -1
var connected := false

func _ready() -> void:
    load_session()

func load_session() -> void:
    var cfg := ConfigFile.new()
    if cfg.load(SAVE_PATH) != OK:
        return
    server_host = str(cfg.get_value("connection", "host", server_host))
    server_port = int(cfg.get_value("connection", "port", server_port))
    character_name = str(cfg.get_value("player", "name", character_name))
    last_job_id = int(cfg.get_value("player", "job", last_job_id))
    last_organization_id = int(cfg.get_value("player", "organization", last_organization_id))

func save_session() -> void:
    var cfg := ConfigFile.new()
    cfg.set_value("connection", "host", server_host)
    cfg.set_value("connection", "port", server_port)
    cfg.set_value("player", "name", character_name)
    cfg.set_value("player", "job", last_job_id)
    cfg.set_value("player", "organization", last_organization_id)
    cfg.save(SAVE_PATH)

func set_connection(host: String, port: int) -> void:
    server_host = host.strip_edges()
    server_port = port
    save_session()

func set_character(name: String) -> void:
    var clean := name.strip_edges()
    character_name = clean if not clean.is_empty() else "Cidadão"
    save_session()

func set_rp_context(job_id: int, organization_id: int) -> void:
    last_job_id = job_id
    last_organization_id = organization_id
    save_session()
