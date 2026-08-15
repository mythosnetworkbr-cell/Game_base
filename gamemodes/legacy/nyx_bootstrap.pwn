#include <a_samp>

#define NYX_COLOR 0x9B59B6FF

// Legacy bootstrap recovered from the launcher repository.
// The active production GameMode is gamemodes/nyx_roleplay.pwn.

main()
{
    print("========================================");
    print("          NYX ROLEPLAY BOOT");
    print("========================================");
}

public OnGameModeInit()
{
    SetGameModeText("NYX ROLEPLAY");
    ShowPlayerMarkers(PLAYER_MARKERS_MODE_OFF);
    ShowNameTags(1);
    UsePlayerPedAnims();
    SetWorldTime(12);
    SetWeather(10);

    AddPlayerClass(23, 1958.3783, 1343.1572, 15.3746, 269.1425, 0, 0, 0, 0, 0, 0);
    AddPlayerClass(29, 1958.3783, 1343.1572, 15.3746, 269.1425, 0, 0, 0, 0, 0, 0);
    AddPlayerClass(105, 1958.3783, 1343.1572, 15.3746, 269.1425, 0, 0, 0, 0, 0, 0);

    print("[NYX] GameMode carregada com sucesso.");
    return 1;
}

public OnGameModeExit()
{
    return 1;
}

public OnPlayerConnect(playerid)
{
    new name[MAX_PLAYER_NAME], msg[144];
    GetPlayerName(playerid, name, sizeof name);
    format(msg, sizeof msg, "[NYX] Bem-vindo, %s. NYX ROLEPLAY está online!", name);
    SendClientMessage(playerid, NYX_COLOR, msg);
    SendClientMessage(playerid, -1, "Use /ajuda para ver os comandos disponíveis.");
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    return 1;
}

public OnPlayerSpawn(playerid)
{
    SetPlayerPos(playerid, 1958.3783, 1343.1572, 15.3746);
    SetPlayerFacingAngle(playerid, 269.1425);
    SetCameraBehindPlayer(playerid);
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/ajuda", true))
    {
        SendClientMessage(playerid, NYX_COLOR, "=== NYX ROLEPLAY ===");
        SendClientMessage(playerid, -1, "/ajuda - comandos | /nyx - status do servidor");
        return 1;
    }

    if (!strcmp(cmdtext, "/nyx", true))
    {
        SendClientMessage(playerid, NYX_COLOR, "NYX ROLEPLAY | GameMode carregada e funcionando.");
        return 1;
    }

    return 0;
}
