#if defined _NYX_POLICE_INCLUDED
    #endinput
#endif
#define _NYX_POLICE_INCLUDED

#define NYX_POLICE_MIN_ORG 0
#define NYX_POLICE_MAX_ORG 8
#define NYX_POLICE_FINE_MAX 50000
#define NYX_POLICE_ARREST_DISTANCE 4.0

new NYX_WantedLevel[MAX_PLAYERS];
new NYX_PoliceCuffed[MAX_PLAYERS];

stock NYX_IsPolice(playerid)
{
    new org = NYX_Player[playerid][NYX_Org];
    return org >= NYX_POLICE_MIN_ORG && org <= NYX_POLICE_MAX_ORG;
}

stock NYX_PoliceReset(playerid)
{
    NYX_WantedLevel[playerid] = 0;
    NYX_PoliceCuffed[playerid] = 0;
    return 1;
}

stock NYX_PoliceTarget(playerid)
{
    new target = INVALID_PLAYER_ID;
    new Float:best = NYX_POLICE_ARREST_DISTANCE + 0.1;
    for(new i=0; i<MAX_PLAYERS; i++)
    {
        if(i == playerid || !IsPlayerConnected(i) || !NYX_Player[i][NYX_Logged]) continue;
        if(IsPlayerInRangeOfPoint(i, NYX_POLICE_ARREST_DISTANCE, 0.0, 0.0, 0.0)) continue;
        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);
        if(IsPlayerInRangeOfPoint(i, NYX_POLICE_ARREST_DISTANCE, x, y, z))
        {
            new Float:tx, Float:ty, Float:tz;
            GetPlayerPos(i, tx, ty, tz);
            new Float:dx = x - tx, Float:dy = y - ty, Float:dz = z - tz;
            new Float:dist = floatsqroot((dx * dx) + (dy * dy) + (dz * dz));
            if(dist < best)
            {
                best = dist;
                target = i;
            }
        }
    }
    return target;
}

stock NYX_PoliceSetWanted(playerid, target, level)
{
    if(!NYX_IsPolice(playerid)) return SendClientMessage(playerid, COLOR_ERROR, "Voce nao pertence a uma organizacao policial.");
    if(target == INVALID_PLAYER_ID || !IsPlayerConnected(target)) return SendClientMessage(playerid, COLOR_ERROR, "Jogador invalido.");
    if(level < 0) level = 0;
    if(level > 6) level = 6;
    NYX_WantedLevel[target] = level;
    SetPlayerWantedLevel(target, level);
    new msg[128];
    format(msg, sizeof msg, "[POLICIA] Nivel de procurado de %d definido para %d.", target, level);
    SendClientMessage(playerid, COLOR_SUCCESS, msg);
    format(msg, sizeof msg, "[POLICIA] Voce recebeu nivel de procurado %d.", level);
    SendClientMessage(target, COLOR_WARNING, msg);
    return 1;
}

stock NYX_PoliceCuff(playerid, target)
{
    if(!NYX_IsPolice(playerid)) return SendClientMessage(playerid, COLOR_ERROR, "Voce nao pertence a uma organizacao policial.");
    if(target == INVALID_PLAYER_ID || !IsPlayerConnected(target)) return SendClientMessage(playerid, COLOR_ERROR, "Jogador invalido.");
    if(!IsPlayerInRangeOfPoint(target, NYX_POLICE_ARREST_DISTANCE, 0.0, 0.0, 0.0)) { }
    NYX_PoliceCuffed[target] = !NYX_PoliceCuffed[target];
    TogglePlayerControllable(target, NYX_PoliceCuffed[target] ? 0 : 1);
    SetPlayerSpecialAction(target, NYX_PoliceCuffed[target] ? SPECIAL_ACTION_HANDSUP : SPECIAL_ACTION_NONE);
    if(NYX_PoliceCuffed[target])
    {
        SendClientMessage(playerid, COLOR_SUCCESS, "Jogador algemado.");
        SendClientMessage(target, COLOR_WARNING, "Voce foi algemado pela policia.");
    }
    else
    {
        SendClientMessage(playerid, COLOR_SUCCESS, "Jogador liberado das algemas.");
        SendClientMessage(target, COLOR_SUCCESS, "Voce foi liberado das algemas.");
    }
    return 1;
}

stock NYX_PoliceArrest(playerid, target)
{
    if(!NYX_IsPolice(playerid)) return SendClientMessage(playerid, COLOR_ERROR, "Voce nao pertence a uma organizacao policial.");
    if(target == INVALID_PLAYER_ID || !IsPlayerConnected(target)) return SendClientMessage(playerid, COLOR_ERROR, "Jogador invalido.");
    if(!NYX_PoliceCuffed[target] && NYX_WantedLevel[target] <= 0) return SendClientMessage(playerid, COLOR_WARNING, "O jogador nao esta algemado nem procurado.");
    NYX_PoliceCuffed[target] = 0;
    TogglePlayerControllable(target, 1);
    SetPlayerSpecialAction(target, SPECIAL_ACTION_NONE);
    SetPlayerWantedLevel(target, 0);
    NYX_WantedLevel[target] = 0;
    SetPlayerPos(target, 1550.0, -1675.0, 15.0);
    SendClientMessage(playerid, COLOR_SUCCESS, "Priso realizada. Suspeito encaminhado a delegacia.");
    SendClientMessage(target, COLOR_WARNING, "Voce foi preso e encaminhado a delegacia.");
    return 1;
}

stock NYX_PoliceFine(playerid, target, amount)
{
    if(!NYX_IsPolice(playerid)) return SendClientMessage(playerid, COLOR_ERROR, "Voce nao pertence a uma organizacao policial.");
    if(target == INVALID_PLAYER_ID || !IsPlayerConnected(target)) return SendClientMessage(playerid, COLOR_ERROR, "Jogador invalido.");
    if(amount <= 0 || amount > NYX_POLICE_FINE_MAX) return SendClientMessage(playerid, COLOR_ERROR, "Multa invalida. Limite: $50000.");
    if(GetPlayerMoney(target) < amount) amount = GetPlayerMoney(target);
    if(amount <= 0) return SendClientMessage(playerid, COLOR_WARNING, "O jogador nao possui dinheiro em especie.");
    GivePlayerMoney(target, -amount);
    NYX_SyncMoney(target);
    NYX_SaveAccount(target);
    new msg[128];
    format(msg, sizeof msg, "[POLICIA] Multa aplicada: $%d.", amount);
    SendClientMessage(playerid, COLOR_SUCCESS, msg);
    SendClientMessage(target, COLOR_WARNING, msg);
    return 1;
}

stock NYX_PoliceCommands(playerid, cmdtext[])
{
    if(!strcmp(cmdtext, "/policia", true))
    {
        if(!NYX_IsPolice(playerid)) return SendClientMessage(playerid, COLOR_ERROR, "Acesso restrito as organizacoes policiais.");
        SendClientMessage(playerid, COLOR_NYX, "[POLICIA] /algemar ID | /desalgemar ID | /procurado ID NIVEL | /prender ID | /multa ID VALOR");
        return 1;
    }
    if(strfind(cmdtext, "/algemar ", true) == 0 || strfind(cmdtext, "/desalgemar ", true) == 0)
    {
        if(!NYX_IsPolice(playerid)) return SendClientMessage(playerid, COLOR_ERROR, "Acesso restrito as organizacoes policiais.");
        new target = INVALID_PLAYER_ID;
        if(strfind(cmdtext, "/algemar ", true) == 0) target = strval(cmdtext[9]);
        else target = strval(cmdtext[12]);
        return NYX_PoliceCuff(playerid, target);
    }
    if(strfind(cmdtext, "/procurado ", true) == 0)
    {
        new rest[64], target, level;
        format(rest, sizeof rest, "%s", cmdtext[11]);
        target = strval(rest);
        new p = strfind(rest, " ", true);
        if(p == -1) return SendClientMessage(playerid, COLOR_WARNING, "Uso: /procurado ID NIVEL");
        level = strval(rest[p + 1]);
        return NYX_PoliceSetWanted(playerid, target, level);
    }
    if(strfind(cmdtext, "/prender ", true) == 0)
        return NYX_PoliceArrest(playerid, strval(cmdtext[9]));
    if(strfind(cmdtext, "/multa ", true) == 0)
    {
        new rest[64], target, amount;
        format(rest, sizeof rest, "%s", cmdtext[7]);
        target = strval(rest);
        new p = strfind(rest, " ", true);
        if(p == -1) return SendClientMessage(playerid, COLOR_WARNING, "Uso: /multa ID VALOR");
        amount = strval(rest[p + 1]);
        return NYX_PoliceFine(playerid, target, amount);
    }
    if(!strcmp(cmdtext, "/procurados", true))
    {
        if(!NYX_IsPolice(playerid)) return SendClientMessage(playerid, COLOR_ERROR, "Acesso restrito as organizacoes policiais.");
        new msg[144];
        SendClientMessage(playerid, COLOR_NYX, "[POLICIA] Jogadores procurados online:");
        for(new i=0; i<MAX_PLAYERS; i++)
        {
            if(IsPlayerConnected(i) && NYX_Player[i][NYX_Logged] && NYX_WantedLevel[i] > 0)
            {
                format(msg, sizeof msg, "ID %d | Nivel %d", i, NYX_WantedLevel[i]);
                SendClientMessage(playerid, COLOR_WHITE, msg);
            }
        }
        return 1;
    }
    return 0;
}
