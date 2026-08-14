#if defined _NYX_NCOINS
    #endinput
#endif
#define _NYX_NCOINS

/* NCoins: moeda premium da NYX. Compra externa deve ser processada por backend/loja oficial. */
#define NYX_NCOINS_START 0
#define NYX_NCOINS_MAX 2147483647
#define NYX_SKIN_FREE 0
#define NYX_SKIN_PREMIUM 1

enum E_NYX_SKIN_STORE
{
    NCSkinModel,
    NCSkinPrice,
    NCSkinPremium
};

new const NYX_SkinStore[][E_NYX_SKIN_STORE] =
{
    {1, 0, NYX_SKIN_FREE},
    {2, 0, NYX_SKIN_FREE},
    {23, 0, NYX_SKIN_FREE},
    {280, 500, NYX_SKIN_PREMIUM},
    {281, 750, NYX_SKIN_PREMIUM},
    {282, 1000, NYX_SKIN_PREMIUM},
    {283, 1200, NYX_SKIN_PREMIUM},
    {284, 1500, NYX_SKIN_PREMIUM},
    {285, 1800, NYX_SKIN_PREMIUM}
};

/* Regras: NCoins nao entram no cassino, nao sao apostaveis e nao sao convertidos em dinheiro do jogo. */
