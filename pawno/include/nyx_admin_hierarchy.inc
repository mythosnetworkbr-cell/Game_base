#if defined _NYX_ADMIN_HIERARCHY_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_HIERARCHY_INCLUDED

// Hierarquia administrativa oficial fornecida para a NYX ROLEPLAY.
#define LEVEL_SUPORTE            1
#define LEVEL_ADMIN_ASSISTENTE   2
#define LEVEL_ADMIN_2            3
#define LEVEL_ADMIN_LIDER        4
#define LEVEL_MANAGER            5
#define LEVEL_ALL                6
#define LEVEL_STAFF              7
#define LEVEL_DEV                80
#define LEVEL_OWNER              100

stock bool:NYX_IsValidAdminLevel(level)
{
    switch(level)
    {
        case 0, LEVEL_SUPORTE, LEVEL_ADMIN_ASSISTENTE, LEVEL_ADMIN_2,
             LEVEL_ADMIN_LIDER, LEVEL_MANAGER, LEVEL_ALL, LEVEL_STAFF,
             LEVEL_DEV, LEVEL_OWNER: return true;
    }
    return false;
}

stock NYX_GetAdminRankName(level, name[], size)
{
    switch(level)
    {
        case 0: name = "Jogador";
        case LEVEL_SUPORTE: name = "Suporte";
        case LEVEL_ADMIN_ASSISTENTE: name = "Admin Assistente";
        case LEVEL_ADMIN_2: name = "Admin 2";
        case LEVEL_ADMIN_LIDER: name = "Admin Lider";
        case LEVEL_MANAGER: name = "Manager";
        case LEVEL_ALL: name = "ALL";
        case LEVEL_STAFF: name = "Staff";
        case LEVEL_DEV: name = "Dev";
        case LEVEL_OWNER: name = "Owner";
        default: name = "Desconhecido";
    }
    return 1;
}

// Somente Dev (80) e Owner (100) podem conceder/revogar cargos.
stock bool:NYX_CanManageAdminLevel(playerid)
{
    return NYX_AdminLevel(playerid) >= LEVEL_DEV;
}

#endif
