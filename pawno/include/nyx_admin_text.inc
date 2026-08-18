#if defined _NYX_ADMIN_TEXT_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_TEXT_INCLUDED
#include <sscanf2>

stock NYX_AdminDeny(playerid)
{
    SendClientMessage(playerid, COLOR_ERROR, "NYX: voce nao possui permissao para este comando.");
    return 1;
}

stock bool:NYX_AdminGetTarget(const params[], &targetid)
{
    targetid = INVALID_PLAYER_ID;
    if (sscanf(params, "u", targetid)) return false;
    return IsPlayerConnected(targetid);
}

stock NYX_AdminCommandText(playerid, cmdtext[])
{
    if (!NYX_Player[playerid][NYX_Logged]) return 0;

    new cmd[32], params[128];
    cmd[0] = EOS;
    params[0] = EOS;
    sscanf(cmdtext, "p< >s[32]S()[128]", cmd, params);
    if (cmd[0] == '/') strdel(cmd, 0, 1);

    // Nivel 1
    if (!strcmp(cmd, "tbadm", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        SendClientMessage(playerid, COLOR_SUCCESS, "NYX STAFF: painel administrativo nivel 1.");
        return 1;
    }
    if (!strcmp(cmd, "limparchat", true) || !strcmp(cmd, "cc", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        for (new i = 0; i < 100; i++) SendClientMessageToAll(-1, " ");
        SendClientMessageToAll(COLOR_SUCCESS, "NYX STAFF: chat limpo pela administracao.");
        return 1;
    }
    if (!strcmp(cmd, "respawncars", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        for (new v = 1; v < MAX_VEHICLES; v++) SetVehicleToRespawn(v);
        SendClientMessage(playerid, COLOR_SUCCESS, "Veiculos respawnados.");
        return 1;
    }
    if (!strcmp(cmd, "reviver", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        new id;
        if (!NYX_AdminGetTarget(params, id)) return SendClientMessage(playerid, COLOR_ERROR, "Uso: /reviver [id/nome]"), 1;
        SetPlayerHealth(id, 100.0);
        SendClientMessage(id, COLOR_SUCCESS, "Voce foi revivido pela administracao.");
        SendClientMessage(playerid, COLOR_SUCCESS, "Jogador revivido.");
        return 1;
    }
    if (!strcmp(cmd, "invisivel", true) || !strcmp(cmd, "inv", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        SetPlayerColor(playerid, 0xFFFFFF00);
        SetPlayerVirtualWorld(playerid, 999);
        SendClientMessage(playerid, COLOR_SUCCESS, "Modo invisivel ativado.");
        return 1;
    }
    if (!strcmp(cmd, "visivel", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        SetPlayerColor(playerid, 0xFFFFFFFF);
        SetPlayerVirtualWorld(playerid, 0);
        SendClientMessage(playerid, COLOR_SUCCESS, "Modo invisivel desativado.");
        return 1;
    }
    if (!strcmp(cmd, "desarmar", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        new id;
        if (!NYX_AdminGetTarget(params, id)) return SendClientMessage(playerid, COLOR_ERROR, "Uso: /desarmar [id/nome]"), 1;
        ResetPlayerWeapons(id);
        SendClientMessage(id, COLOR_ERROR, "Suas armas foram removidas pela administracao.");
        SendClientMessage(playerid, COLOR_SUCCESS, "Jogador desarmado.");
        return 1;
    }
    if (!strcmp(cmd, "voar", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        if (GetPVarType(playerid, "NYX_FLY"))
        {
            DeletePVar(playerid, "NYX_FLY");
            SendClientMessage(playerid, COLOR_SUCCESS, "Modo voo desativado.");
        }
        else
        {
            SetPVarInt(playerid, "NYX_FLY", 1);
            SendClientMessage(playerid, COLOR_SUCCESS, "Modo voo ativado.");
        }
        return 1;
    }

    // Nivel 2+
    if (!strcmp(cmd, "aviso", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 2) return NYX_AdminDeny(playerid), 1;
        if (!strlen(params)) return SendClientMessage(playerid, COLOR_ERROR, "Uso: /aviso [mensagem]"), 1;
        new msg[144]; format(msg, sizeof msg, "[STAFF] %s", params);
        SendClientMessageToAll(COLOR_ERROR, msg);
        return 1;
    }
    if (!strcmp(cmd, "setskin", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 3) return NYX_AdminDeny(playerid), 1;
        new id, skin;
        if (sscanf(params, "ui", id, skin) || !IsPlayerConnected(id) || !NYX_IsValidSkin(skin))
            return SendClientMessage(playerid, COLOR_ERROR, "Uso: /setskin [id/nome] [skin]"), 1;
        NYX_Player[id][NYX_Skin] = skin;
        SetPlayerSkin(id, skin);
        SendClientMessage(playerid, COLOR_SUCCESS, "Skin alterada.");
        return 1;
    }
    if (!strcmp(cmd, "dargrana", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 5) return NYX_AdminDeny(playerid), 1;
        new id, amount;
        if (sscanf(params, "ui", id, amount) || !IsPlayerConnected(id) || amount < 0)
            return SendClientMessage(playerid, COLOR_ERROR, "Uso: /dargrana [id/nome] [valor]"), 1;
        GivePlayerMoney(id, amount);
        NYX_Player[id][NYX_Money] = GetPlayerMoney(id);
        SendClientMessage(playerid, COLOR_SUCCESS, "Dinheiro entregue.");
        return 1;
    }

    // Nivel 10
    if (!strcmp(cmd, "setadm", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 10) return NYX_AdminDeny(playerid), 1;
        new id, level;
        if (sscanf(params, "ui", id, level) || !IsPlayerConnected(id) || level < 0 || level > 10)
            return SendClientMessage(playerid, COLOR_ERROR, "Uso: /setadm [id/nome] [0-10]"), 1;
        NYX_Player[id][NYX_Admin] = level;
        SendClientMessage(playerid, COLOR_SUCCESS, "Nivel administrativo alterado.");
        return 1;
    }
    if (!strcmp(cmd, "darcoins", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 10) return NYX_AdminDeny(playerid), 1;
        new id, amount;
        if (sscanf(params, "ui", id, amount) || !IsPlayerConnected(id) || amount < 0)
            return SendClientMessage(playerid, COLOR_ERROR, "Uso: /darcoins [id/nome] [quantidade]"), 1;
        NYX_Player[id][NYX_NCoins] += amount;
        SendClientMessage(playerid, COLOR_SUCCESS, "NCoins entregues.");
        return 1;
    }
    if (!strcmp(cmd, "sethora", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 10) return NYX_AdminDeny(playerid), 1;
        new h;
        if (sscanf(params, "i", h) || h < 0 || h > 23) return SendClientMessage(playerid, COLOR_ERROR, "Uso: /sethora [0-23]"), 1;
        SetWorldTime(h);
        return SendClientMessage(playerid, COLOR_SUCCESS, "Hora alterada."), 1;
    }
    if (!strcmp(cmd, "setclima", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 10) return NYX_AdminDeny(playerid), 1;
        new weather;
        if (sscanf(params, "i", weather) || weather < 0 || weather > 255) return SendClientMessage(playerid, COLOR_ERROR, "Uso: /setclima [0-255]"), 1;
        SetWeather(weather);
        return SendClientMessage(playerid, COLOR_SUCCESS, "Clima alterado."), 1;
    }
    if (!strcmp(cmd, "ir", true) || !strcmp(cmd, "goto", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        new id;
        if (!NYX_AdminGetTarget(params, id)) return SendClientMessage(playerid, COLOR_ERROR, "Uso: /ir [id/nome]"), 1;
        new Float:x, Float:y, Float:z;
        GetPlayerPos(id, x, y, z);
        SetPlayerInterior(playerid, GetPlayerInterior(id));
        SetPlayerVirtualWorld(playerid, GetPlayerVirtualWorld(id));
        SetPlayerPos(playerid, x + 1.0, y, z);
        SendClientMessage(playerid, COLOR_SUCCESS, "Teleportado ate o jogador.");
        return 1;
    }
    if (!strcmp(cmd, "trazer", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 10) return NYX_AdminDeny(playerid), 1;
        new id;
        if (!NYX_AdminGetTarget(params, id)) return SendClientMessage(playerid, COLOR_ERROR, "Uso: /trazer [id/nome]"), 1;
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        SetPlayerInterior(id, GetPlayerInterior(playerid));
        SetPlayerVirtualWorld(id, GetPlayerVirtualWorld(playerid));
        SetPlayerPos(id, x + 1.0, y, z);
        SendClientMessage(id, COLOR_SUCCESS, "Voce foi trazido pela administracao.");
        return 1;
    }
    if (!strcmp(cmd, "godmode", true) || !strcmp(cmd, "god", true))
    {
        if (NYX_Player[playerid][NYX_Admin] < 1) return NYX_AdminDeny(playerid), 1;
        if (GetPVarType(playerid, "NYX_GOD"))
        {
            DeletePVar(playerid, "NYX_GOD");
            SetPlayerHealth(playerid, 100.0);
            SendClientMessage(playerid, COLOR_SUCCESS, "God Mode desativado.");
        }
        else
        {
            SetPVarInt(playerid, "NYX_GOD", 1);
            SetPlayerHealth(playerid, 99999.0);
            SendClientMessage(playerid, COLOR_SUCCESS, "God Mode ativado.");
        }
        return 1;
    }

    return 0;
}
