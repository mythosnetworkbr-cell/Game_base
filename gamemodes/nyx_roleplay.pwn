#include <a_samp>
#include <nyx_config>
#include <nyx_player>
#include <nyx_medical>
#include <nyx_orgs>
#include <nyx_jobs>
#include <nyx_marriage>
#include <nyx_ncoins>
#include <nyx_world>
#include <nyx_properties>
#include <nyx_graphics>
#include <nyx_admin>
#include <nyx_admin_hierarchy>
#include <nyx_admin_hierarchy_cmds>
#include <nyx_rp_complete>

#define D_LOGIN 1000
#define D_REGISTER 1001
#define D_SKINS 1002
#define D_ORGS 1003
#define D_HELP 1004
#define D_JOBS 1005
#define D_STORE 1006
#define D_BANK 1007
#define D_PROFILE 1008

new NYX_PendingMarriage[MAX_PLAYERS];
forward NYX_AutoSave();

public OnGameModeInit()
{
    SetGameModeText(NYX_SERVER_NAME);
    ShowPlayerMarkers(1); ShowNameTags(1); UsePlayerPedAnims();
    SetWorldTime(12); SetWeather(10);
    AddPlayerClass(NYX_DEFAULT_SKIN_MALE,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z,0.0,0,0,0,0,0,0);
    AddPlayerClass(NYX_DEFAULT_SKIN_FEMALE,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z,0.0,0,0,0,0,0,0);
    NYX_InitProperties();
    NYX2_Init();
    SetTimer("NYX_AutoSave",60000,true);
    Create3DTextLabel("{8B5CF6}NYX ROLEPLAY\n{FFFFFF}Prefeitura / Centro",COLOR_WHITE,NYX_SPAWN_X,NYX_SPAWN_Y,21.0,30.0,0,1);
    Create3DTextLabel("{8B5CF6}HOSPITAL CENTRAL NYX",COLOR_WHITE,1520.0,-1675.0,15.0,30.0,0,1);
    Create3DTextLabel("{8B5CF6}DELEGACIA CENTRAL NYX",COLOR_WHITE,1550.0,-1600.0,15.0,30.0,0,1);
    Create3DTextLabel("{8B5CF6}CASSINO NYX",COLOR_WHITE,2200.0,-1670.0,16.0,30.0,0,1);
    Create3DTextLabel("{8B5CF6}SEX SHOP NYX",COLOR_WHITE,1350.0,-1740.0,15.0,30.0,0,1);
    Create3DTextLabel("{8B5CF6}IGREJA CENTRAL NYX",COLOR_WHITE,1420.0,-1710.0,15.0,30.0,0,1);
    print("[NYX] Core RP online: contas, banco, empregos, orgs, casamento, propriedades, NCoins e sistemas avancados.");
    return 1;
}

public NYX_AutoSave()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(IsPlayerConnected(i) && NYX_Player[i][NYX_Logged])
        {
            NYX_LegacyTick(i);
            NYX_MedicalTick(i);
            NYX2_Tick(i);
            NYX_SyncMoney(i);
            NYX_SaveAccount(i);
        }
    }
    NYX2_SaveBusinesses();
    return 1;
}

public OnPlayerConnect(playerid)
{
    NYX_ResetPlayer(playerid); NYX_ResetJob(playerid); NYX_ResetMarriage(playerid); NYX2_InitPlayer(playerid);
    NYX_PendingMarriage[playerid]=INVALID_PLAYER_ID;
    NYX_ApplyGraphicsProfile(playerid);
    if(NYX_AccountExists(playerid))
        ShowPlayerDialog(playerid,D_LOGIN,DIALOG_STYLE_PASSWORD,"NYX ROLEPLAY | LOGIN","Conta encontrada.\n\nDigite sua senha para entrar.","ENTRAR","REGISTRAR");
    else
        ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX ROLEPLAY | REGISTRO","Nova conta NYX.\n\nCrie uma senha com pelo menos 4 caracteres.","CRIAR","SAIR");
    return 1;
}

public OnPlayerDisconnect(playerid,reason)
{
    if(NYX_Player[playerid][NYX_Logged]) NYX_SaveAccount(playerid);
    if(NYX2_MedicalTimer[playerid]!=NYX2_INVALID_TIMER) KillTimer(NYX2_MedicalTimer[playerid]);
    if(NYX_JobVehicleId[playerid]!=INVALID_VEHICLE_ID) DestroyVehicle(NYX_JobVehicleId[playerid]);
    return 1;
}
