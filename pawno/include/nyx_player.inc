#if defined _NYX_PLAYER_INCLUDED
    #endinput
#endif
#define _NYX_PLAYER_INCLUDED

enum E_NYX_PLAYER {
    NYX_Logged,
    NYX_Skin,
    NYX_Money,
    NYX_Org,
    NYX_OrgRank,
    NYX_Admin
};

new NYX_Player[MAX_PLAYERS][E_NYX_PLAYER];

stock NYX_ResetPlayer(playerid)
{
    NYX_Player[playerid][NYX_Logged] = 0;
    NYX_Player[playerid][NYX_Skin] = 23;
    NYX_Player[playerid][NYX_Money] = 5000;
    NYX_Player[playerid][NYX_Org] = -1;
    NYX_Player[playerid][NYX_OrgRank] = 0;
    NYX_Player[playerid][NYX_Admin] = 0;
    return 1;
}

stock NYX_SetupSpawn(playerid)
{
    SetPlayerPos(playerid, 1481.0, -1771.0, 18.8);
    SetPlayerFacingAngle(playerid, 0.0);
    SetPlayerSkin(playerid, NYX_Player[playerid][NYX_Skin]);
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, NYX_Player[playerid][NYX_Money]);
    SetCameraBehindPlayer(playerid);
    return 1;
}
