#if defined _NYX_ADMIN_HIERARCHY_COMMANDS_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_HIERARCHY_COMMANDS_INCLUDED

// Envia uma mensagem administrativa para toda a Staff conectada.
stock SendAdminMessage(color, const message[])
{
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerConnected(i) && NYX_AdminLevel(i) > 0)
        {
            SendClientMessage(i, color, message);
        }
    }
    return 1;
}

// /daradmin - somente Dev (80) e Owner (100).
CMD:daradmin(playerid, params[])
{
    if(!NYX_CanManageAdminLevel(playerid))
        return NYX_AdminDeny(playerid);

    new targetid, level;
    if(sscanf(params, "ui", targetid, level))
    {
        SendClientMessage(playerid, COLOR_WARNING, "Uso: /daradmin [id/nome] [nivel]");
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

    // Guarda o cargo antigo para o registro.
    new oldLevel = NYX_AdminLevel(targetid);
    new actorLevel = NYX_AdminLevel(playerid);

    // Aplica e persiste o novo nivel.
    NYX_Player[targetid][NYX_Admin] = level;
    NYX_SaveAccount(targetid);

    new adminName[MAX_PLAYER_NAME], targetName[MAX_PLAYER_NAME];
    new oldRank[32], newRank[32], actorRank[32];
    new string[180];

    GetPlayerName(playerid, adminName, sizeof(adminName));
    GetPlayerName(targetid, targetName, sizeof(targetName));
    NYX_GetAdminRankName(oldLevel, oldRank, sizeof(oldRank));
    NYX_GetAdminRankName(level, newRank, sizeof(newRank));
    NYX_GetAdminRankName(actorLevel, actorRank, sizeof(actorRank));

    // Mensagem individual para o jogador afetado.
    format(string, sizeof(string), "[ADMIN]: O %s %s alterou seu cargo para: %s (Nivel %d).", oldRank, adminName, newRank, level);
    SendClientMessage(targetid, COLOR_SUCCESS, string);

    // Log global enviado para TODOS os admins/staff conectados.
    format(string, sizeof(string), "[ADM-LOG]: O %s %s definiu o cargo de %s para %s (Nivel %d).", actorRank, adminName, targetName, newRank, level);
    SendAdminMessage(COLOR_WARNING, string);

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
