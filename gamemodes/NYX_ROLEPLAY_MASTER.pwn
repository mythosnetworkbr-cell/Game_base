#include <a_samp>
#include <nyx_config>
#include <nyx_player>
#include <nyx_accounts>
#include <nyx_orgs>
#include <nyx_jobs>
#include <nyx_marriage>
#include <nyx_ncoins>
#include <nyx_world>
#include <nyx_properties>
#include <nyx_graphics>

#define D_LOGIN 1000
#define D_REGISTER 1001

new NYX_PendingMarriage[MAX_PLAYERS];

public OnGameModeInit()
{
    SetGameModeText(NYX_SERVER_NAME);
    ShowPlayerMarkers(1);
    ShowNameTags(1);
    UsePlayerPedAnims();
    SetWorldTime(12);
    SetWeather(10);
    AddPlayerClass(NYX_DEFAULT_SKIN_MALE, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 0.0, 0,0,0,0,0,0);
    AddPlayerClass(NYX_DEFAULT_SKIN_FEMALE, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 0.0, 0,0,0,0,0,0);
    NYX_AccountsInit();
    print("[NYX] Master GameMode inicializada.");
    return 1;
}

public OnGameModeExit()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
        if (IsPlayerConnected(i) && NYX_Player[i][NYX_Logged]) NYX_SaveAccount(i);
    NYX_AccountsExit();
    return 1;
}

public OnPlayerConnect(playerid)
{
    NYX_ResetPlayer(playerid);
    NYX_ResetJob(playerid);
    NYX_ResetMarriage(playerid);
    NYX_ResetAccountState(playerid);
    NYX_PendingMarriage[playerid] = INVALID_PLAYER_ID;
    NYX_ApplyGraphicsProfile(playerid);
    ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | LOGIN", "Bem-vindo ao NYX ROLEPLAY.\n\nDigite sua senha para entrar.", "ENTRAR", "REGISTRAR");
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (NYX_AccountExists[playerid]) NYX_SaveAccount(playerid);
    if (NYX_JobVehicleId[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(NYX_JobVehicleId[playerid]);
    NYX_ResetJob(playerid);
    NYX_ResetMarriage(playerid);
    NYX_ResetAccountState(playerid);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    if (!NYX_Player[playerid][NYX_Logged]) return 1;
    NYX_ApplyGraphicsProfile(playerid);
    NYX_SetupSpawn(playerid);
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch (dialogid)
    {
        case D_LOGIN:
        {
            if (!response) return ShowPlayerDialog(playerid, D_REGISTER, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | REGISTRO", "Crie sua senha. Minimo 4 caracteres.", "CRIAR", "VOLTAR");
            if (strlen(inputtext) < 4) return ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | LOGIN", "Senha muito curta.", "ENTRAR", "REGISTRAR");
            if (!NYX_AccountLoaded[playerid]) NYX_LoadAccount(playerid);
            if (!NYX_AccountExists[playerid]) return ShowPlayerDialog(playerid, D_REGISTER, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | REGISTRO", "Esta conta ainda nao existe. Crie sua senha.", "CRIAR", "VOLTAR");
            if (!NYX_CheckPassword(playerid, inputtext)) return ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | LOGIN", "Senha incorreta.", "ENTRAR", "REGISTRAR");
            NYX_Player[playerid][NYX_Logged] = 1;
            SendClientMessage(playerid, COLOR_SUCCESS, "Login realizado. Bem-vindo ao NYX ROLEPLAY!");
            SpawnPlayer(playerid);
            return 1;
        }
        case D_REGISTER:
        {
            if (!response) return ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | LOGIN", "Digite sua senha.", "ENTRAR", "REGISTRAR");
            if (strlen(inputtext) < 4) return ShowPlayerDialog(playerid, D_REGISTER, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | REGISTRO", "Senha muito curta.", "CRIAR", "VOLTAR");
            if (!NYX_AccountLoaded[playerid]) NYX_LoadAccount(playerid);
            if (NYX_AccountExists[playerid]) return ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | LOGIN", "Esta conta ja existe.", "ENTRAR", "REGISTRAR");
            if (!NYX_CreateAccount(playerid, inputtext)) return SendClientMessage(playerid, COLOR_ERROR, "Nao foi possivel criar a conta.");
            NYX_Player[playerid][NYX_Logged] = 1;
            SendClientMessage(playerid, COLOR_SUCCESS, "Conta criada! Bem-vindo ao NYX ROLEPLAY!");
            SpawnPlayer(playerid);
            return 1;
        }
    }
    return 0;
}

public OnPlayerEnterCheckpoint(playerid)
{
    if (NYX_JobRunning[playerid])
    {
        DisablePlayerCheckpoint(playerid);
        SendClientMessage(playerid, COLOR_NYX, "Destino alcancado. Use /concluir para receber seu pagamento.");
    }
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!NYX_Player[playerid][NYX_Logged]) return 1;
    if (!strcmp(cmdtext, "/status", true))
    {
        new job[48], msg[192];
        NYX_GetJobName(NYX_PlayerJob[playerid], job, sizeof job);
        format(msg, sizeof msg, "NYX | Dinheiro: $%d | NCoins: %d | Skin: %d | Emprego: %s", GetPlayerMoney(playerid), NYX_Player[playerid][NYX_NCoins], NYX_Player[playerid][NYX_Skin], job);
        return SendClientMessage(playerid, COLOR_WHITE, msg);
    }
    if (!strcmp(cmdtext, "/empregos", true)) return NYX_ShowJobs(playerid);
    if (!strcmp(cmdtext, "/trabalhar", true)) return NYX_StartJob(playerid);
    if (!strcmp(cmdtext, "/concluir", true)) return NYX_ConcludeJob(playerid);
    if (!strcmp(cmdtext, "/ncoins", true))
    {
        new msg[64];
        format(msg, sizeof msg, "Seu saldo: %d NCoins.", NYX_Player[playerid][NYX_NCoins]);
        return SendClientMessage(playerid, COLOR_NYX, msg);
    }
    return 0;
}
