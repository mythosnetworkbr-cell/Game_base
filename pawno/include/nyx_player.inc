#if defined _NYX_PLAYER_INCLUDED
    #endinput
#endif
#define _NYX_PLAYER_INCLUDED

#define NYX_FIRST_SKIN_ID 0
#define NYX_LAST_SKIN_ID 311

enum E_NYX_PLAYER {
    NYX_Logged,
    NYX_Skin,
    NYX_Money,
    NYX_NCoins,
    NYX_Org,
    NYX_OrgRank,
    NYX_Admin
};

new NYX_Player[MAX_PLAYERS][E_NYX_PLAYER];

stock NYX_ResetPlayer(playerid)
{
    NYX_Player[playerid][NYX_Logged] = 0;
    NYX_Player[playerid][NYX_Skin] = (playerid % 2) ? 2 : 1;
    NYX_Player[playerid][NYX_Money] = 5000;
    NYX_Player[playerid][NYX_NCoins] = 0;
    NYX_Player[playerid][NYX_Org] = -1;
    NYX_Player[playerid][NYX_OrgRank] = 0;
    NYX_Player[playerid][NYX_Admin] = 0;
    return 1;
}

stock NYX_IsValidSkin(skinid)
{
    return skinid >= NYX_FIRST_SKIN_ID && skinid <= NYX_LAST_SKIN_ID;
}

stock NYX_SetupSpawn(playerid)
{
    SetPlayerPos(playerid,1481.0,-1771.0,18.8);
    SetPlayerFacingAngle(playerid,0.0);
    SetPlayerSkin(playerid,NYX_Player[playerid][NYX_Skin]);
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid,NYX_Player[playerid][NYX_Money]);
    SetCameraBehindPlayer(playerid);
    return 1;
}
