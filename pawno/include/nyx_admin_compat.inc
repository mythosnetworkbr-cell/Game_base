#if defined _NYX_ADMIN_COMPAT_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_COMPAT_INCLUDED

// SA-MP/Pawn nao possui strncmp; os comandos migrados usam comparacao por prefixo.
#define strncmp(%0,%1,%2,%3) (strfind(%0,%1,%2,%3)==0)

// O SA-MP nao expoe SetPlayerAlpha nativo. A invisibilidade da NYX usa
// SetPlayerColor com canal alpha; este helper evita erro de compilacao nos
// trechos antigos que chamavam SetPlayerAlpha.
stock SetPlayerAlpha(playerid,alpha)
{
    if(alpha<=0) SetPlayerColor(playerid,0xFFFFFF00);
    else SetPlayerColor(playerid,0xFFFFFFFF);
    return 1;
}
