#if defined _NYX_MARRIAGE
    #endinput
#endif
#define _NYX_MARRIAGE

// Sistema de casamento RP: qualquer jogador pode casar com qualquer outro jogador.
// A GameMode nao usa genero como criterio de validacao.

#define NYX_MARRIAGE_NONE -1

stock NYX_MarriageCanPropose(playerid, targetid)
{
    if (playerid == targetid) return 0;
    if (!IsPlayerConnected(targetid)) return 0;
    if (!NYX_Player[playerid][NYX_Logged] || !NYX_Player[targetid][NYX_Logged]) return 0;
    return 1;
}

stock NYX_MarriageSendProposal(playerid, targetid)
{
    if (!NYX_MarriageCanPropose(playerid, targetid))
        return SendClientMessage(playerid, COLOR_ERROR, "Nao e possivel enviar esta proposta.");

    new name[MAX_PLAYER_NAME];
    GetPlayerName(playerid, name, sizeof name);
    new msg[160];
    format(msg, sizeof msg, "%s enviou uma proposta de casamento. O casamento no NYX e permitido entre quaisquer generos.", name);
    SendClientMessage(targetid, COLOR_NYX, msg);
    SendClientMessage(playerid, COLOR_SUCCESS, "Proposta de casamento enviada.");
    return 1;
}

stock NYX_MarriageExplain(playerid)
{
    SendClientMessage(playerid, COLOR_NYX, "NYX CASAMENTO: qualquer personagem pode se casar com qualquer outro personagem.");
    SendClientMessage(playerid, COLOR_WHITE, "O sistema considera apenas consentimento e estado civil do personagem, sem restricao por genero.");
    return 1;
}
