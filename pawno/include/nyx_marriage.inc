#if defined _NYX_MARRIAGE
    #endinput
#endif
#define _NYX_MARRIAGE

#define NYX_MARRIAGE_NONE -1
new NYX_Spouse[MAX_PLAYERS];
new bool:NYX_Married[MAX_PLAYERS];

stock NYX_ResetMarriage(playerid)
{
    NYX_Spouse[playerid] = INVALID_PLAYER_ID;
    NYX_Married[playerid] = false;
    return 1;
}

stock NYX_MarriageCanPropose(playerid, targetid)
{
    if (playerid == targetid) return 0;
    if (!IsPlayerConnected(targetid)) return 0;
    if (!NYX_Player[playerid][NYX_Logged] || !NYX_Player[targetid][NYX_Logged]) return 0;
    if (NYX_Married[playerid] || NYX_Married[targetid]) return 0;
    return 1;
}

stock NYX_MarryPlayers(playerid, targetid)
{
    if (!NYX_MarriageCanPropose(playerid,targetid)) return 0;
    NYX_Married[playerid] = true;
    NYX_Married[targetid] = true;
    NYX_Spouse[playerid] = targetid;
    NYX_Spouse[targetid] = playerid;
    return 1;
}

stock NYX_Divorce(playerid)
{
    if (!NYX_Married[playerid]) return 0;
    new spouse = NYX_Spouse[playerid];
    NYX_ResetMarriage(playerid);
    if (IsPlayerConnected(spouse)) NYX_ResetMarriage(spouse);
    return 1;
}

stock NYX_MarriageSendProposal(playerid, targetid)
{
    if (!NYX_MarriageCanPropose(playerid, targetid))
        return SendClientMessage(playerid, COLOR_ERROR, "Nao e possivel enviar esta proposta.");
    new name[MAX_PLAYER_NAME], msg[160];
    GetPlayerName(playerid,name,sizeof name);
    format(msg,sizeof msg,"%s enviou uma proposta de casamento. Use /aceitarcasamento para aceitar.",name);
    SendClientMessage(targetid,COLOR_NYX,msg);
    SendClientMessage(playerid,COLOR_SUCCESS,"Proposta enviada.");
    return 1;
}

stock NYX_MarriageExplain(playerid)
{
    SendClientMessage(playerid,COLOR_NYX,"NYX CASAMENTO: qualquer personagem pode casar com qualquer outro personagem, sem restricao de genero.");
    SendClientMessage(playerid,COLOR_WHITE,"Use /casar ID para enviar proposta e /aceitarcasamento para aceitar a proposta recebida.");
    return 1;
}
