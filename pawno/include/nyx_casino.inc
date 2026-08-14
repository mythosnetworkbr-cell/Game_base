#if defined _NYX_CASINO_INCLUDED
    #endinput
#endif
#define _NYX_CASINO_INCLUDED

// Cassino exclusivamente com dinheiro virtual da economia do servidor.
// Sem dinheiro real, pagamentos externos ou conversao de premios.
#define NYX_CASINO_MIN_BET 100
#define NYX_CASINO_MAX_BET 50000

new const NYX_CasinoGames[][] = {
    "Roleta NYX",
    "Blackjack NYX",
    "Slots NYX",
    "Dados NYX"
};

stock NYX_CasinoInfo(playerid)
{
    SendClientMessage(playerid, 0x8B5CF6FF, "=== CASINO NYX ===");
    SendClientMessage(playerid, 0xFFFFFFFF, "Jogos: Roleta, Blackjack, Slots e Dados.");
    SendClientMessage(playerid, 0xFFFFFFFF, "Apostas utilizam somente dinheiro virtual do servidor.");
    SendClientMessage(playerid, 0xFFB84DFF, "Limites: $100 a $50.000 por rodada.");
    return 1;
}
