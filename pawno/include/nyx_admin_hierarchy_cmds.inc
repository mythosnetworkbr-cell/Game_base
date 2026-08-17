#if defined _NYX_ADMIN_HIERARCHY_COMMANDS_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_HIERARCHY_COMMANDS_INCLUDED

// /daradmin - somente Dev (80) e Owner (100).
CMD:daradmin(playerid, params[])
{
    if(!NYX_CanManageAdminLevel(playerid))
        return NYX_AdminDeny(playerid);

    new targetid, level;
    if(sscanf(params, "ui", targetid, level))
    {
        SendClientMessage(playerid, COLOR_WARNING, "Uso: /daradmin [id] [nivel]");
        SendClientMessage(playerid, COLOR_WHITE, "Niveis: 0 Jogador | 1 Suporte | 2 Admin Assistente | 3 Admin 2 | 4 Admin Lider | 5 Manager | 6 ALL | 7 Staff | 80 Dev | 100 Owner");
        return 1;
    }

    if(!IsPlayerConnected(targetid))
        return SendClientMessage(playerid, COLOR_ERROR, "Jogador nao encontrado ou desconectado.");

    if(!NYX_IsValidAdminLevel(level))
        return SendClientMessage(playerid, COLOR_ERROR, "Nivel administrativo invalido.");

    // Dev nao pode entregar cargo igual/superior ao proprio.
    // Owner pode gerenciar qualquer nivel da hierarquia.
    if(NYX_AdminLevel(playerid) < LEVEL_OWNER && level >= NYX_AdminLevel(playerid))
        return SendClientMessage(playerid, COLOR_ERROR, "Voce nao pode atribuir um cargo igual ou superior ao seu.");

    NYX_Player[targetid][NYX_Admin] = level;
    NYX_SaveAccount(targetid);

    new adminName[MAX_PLAYER_NAME], targetName[MAX_PLAYER_NAME];
    new rankName[32], string[180];
    GetPlayerName(playerid, adminName, sizeof(adminName));
    GetPlayerName(targetid, targetName, sizeof(targetName));
    NYX_GetAdminRankName(level, rankName, sizeof(rankName));

    format(string, sizeof(string), "[ADMIN] Voce definiu %s como %s (nivel %d).", targetName, rankName, level);
    SendClientMessage(playerid, COLOR_SUCCESS, string);

    format(string, sizeof(string), "[ADMIN] %s alterou seu cargo para %s (nivel %d).", adminName, rankName, level);
    SendClientMessage(targetid, COLOR_SUCCESS, string);
    return 1;
}

CMD:adminhierarquia(playerid, params[])
{
    if(!NYX_AdminHas(playerid, LEVEL_SUPORTE))
        return NYX_AdminDeny(playerid);

    SendClientMessage(playerid, COLOR_WHITE, "--- HIERARQUIA ADMINISTRATIVA NYX ---");
    SendClientMessage(playerid, COLOR_WHITE, "0 Jogador | 1 Suporte | 2 Admin Assistente | 3 Admin 2 | 4 Admin Lider");
    SendClientMessage(playerid, COLOR_WHITE, "5 Manager | 6 ALL | 7 Staff | 80 Dev | 100 Owner");
    return 1;
}

#endif
