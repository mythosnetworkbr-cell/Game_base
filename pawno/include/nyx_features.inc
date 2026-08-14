#if defined _NYX_FEATURES_INCLUDED
    #endinput
#endif
#define _NYX_FEATURES_INCLUDED

#define NYX_FEATURE_COUNT 8
#define NYX_ADMIN_NONE 0
#define NYX_ADMIN_MOD 1
#define NYX_ADMIN_ADMIN 2
#define NYX_ADMIN_MASTER 3

enum E_NYX_FEATURE_POINT { Float:FX, Float:FY, Float:FZ };
new const Float:NYX_Portal[3] = {1700.0,-1800.0,22.0};
new const Float:NYX_Bridge[3] = {1800.0,-1200.0,20.0};
new const Float:NYX_Statue[3] = {1950.0,-900.0,24.0};
new const Float:NYX_Mall[3] = {1250.0,-1350.0,14.0};
new const Float:NYX_Cinema[3] = {1300.0,-1350.0,14.0};
new const Float:NYX_Church[3] = {1400.0,-1450.0,14.0};

new const NYX_FeatureName[NYX_FEATURE_COUNT][] = {
    "Portal Monumental NYX",
    "Ponte da Ilha NYX",
    "Estatua da Liberdade NYX",
    "Shopping NYX",
    "Cinema NYX",
    "Igreja Central NYX",
    "Mercado de Peixes",
    "Central de Leiloes"
};

new bool:NYX_AdminInvisible[MAX_PLAYERS];
new NYX_AdminLevel[MAX_PLAYERS];
new NYX_ChatOrg[MAX_PLAYERS];
new bool:NYX_Fishing[MAX_PLAYERS];
new NYX_FishCount[MAX_PLAYERS];
new NYX_HouseOwner[MAX_PLAYERS];

stock NYX_InitFeatures()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        NYX_AdminInvisible[i]=false;
        NYX_AdminLevel[i]=NYX_ADMIN_NONE;
        NYX_ChatOrg[i]=-1;
        NYX_Fishing[i]=false;
        NYX_FishCount[i]=0;
        NYX_HouseOwner[i]=-1;
    }
    return 1;
}

stock NYX_CreateFeatureWorld()
{
    Create3DTextLabel("{8B5CF6}PORTAL MONUMENTAL NYX",0xFFFFFFFF,NYX_Portal[0],NYX_Portal[1],NYX_Portal[2],80.0,0,1);
    Create3DTextLabel("{8B5CF6}PONTE DA ILHA NYX\n{FFFFFF}Inspirada na paisagem de Florianopolis",0xFFFFFFFF,NYX_Bridge[0],NYX_Bridge[1],NYX_Bridge[2],80.0,0,1);
    Create3DTextLabel("{8B5CF6}ESTATUA DA LIBERDADE NYX",0xFFFFFFFF,NYX_Statue[0],NYX_Statue[1],NYX_Statue[2],80.0,0,1);
    Create3DTextLabel("{8B5CF6}SHOPPING NYX\n{FFFFFF}Moda, roupas e acessorios",0xFFFFFFFF,NYX_Mall[0],NYX_Mall[1],NYX_Mall[2],60.0,0,1);
    Create3DTextLabel("{8B5CF6}CINEMA NYX",0xFFFFFFFF,NYX_Cinema[0],NYX_Cinema[1],NYX_Cinema[2]+2.0,60.0,0,1);
    Create3DTextLabel("{8B5CF6}IGREJA CENTRAL NYX",0xFFFFFFFF,NYX_Church[0],NYX_Church[1],NYX_Church[2],60.0,0,1);
    return 1;
}

stock NYX_AdminSetInvisible(playerid,bool:state)
{
    NYX_AdminInvisible[playerid]=state;
    ShowPlayerNameTagForAll(playerid,!state);
    return 1;
}

stock NYX_AdminCan(playerid,level)
{
    return NYX_AdminLevel[playerid]>=level;
}

stock NYX_AdminMessage(message[])
{
    for(new i=0;i<MAX_PLAYERS;i++)
        if(IsPlayerConnected(i) && NYX_AdminLevel[i]>NYX_ADMIN_NONE)
            SendClientMessage(i,0xFF66CCFF,message);
    return 1;
}

stock NYX_OrgChat(playerid,message[])
{
    new org=NYX_ChatOrg[playerid];
    if(org<0) return 0;
    new name[MAX_PLAYER_NAME],out[144];
    GetPlayerName(playerid,name,sizeof name);
    format(out,sizeof out,"[ORG] %s: %s",name,message);
    for(new i=0;i<MAX_PLAYERS;i++)
        if(IsPlayerConnected(i) && NYX_ChatOrg[i]==org) SendClientMessage(i,0xB38CFFFF,out);
    return 1;
}

stock NYX_PlayerChat(playerid,message[])
{
    new name[MAX_PLAYER_NAME],out[144];
    GetPlayerName(playerid,name,sizeof name);
    format(out,sizeof out,"[PLAYER] %s: %s",name,message);
    for(new i=0;i<MAX_PLAYERS;i++)
        if(IsPlayerConnected(i) && !NYX_AdminInvisible[i]) SendClientMessage(i,0xFFFFFFFF,out);
    return 1;
}

stock NYX_StartFishing(playerid)
{
    if(NYX_Fishing[playerid]) return SendClientMessage(playerid,0xFFCC66FF,"Voce ja esta pescando.");
    NYX_Fishing[playerid]=true;
    NYX_FishCount[playerid]=0;
    return SendClientMessage(playerid,0x66CCFFFF,"Pesca iniciada! Aguarde o ponto de captura e use /pescar.");
}

stock NYX_CatchFish(playerid)
{
    if(!NYX_Fishing[playerid]) return SendClientMessage(playerid,0xFFCC66FF,"Use /pesca para iniciar.");
    new reward=250+random(750);
    NYX_FishCount[playerid]++;
    GivePlayerMoney(playerid,reward);
    new msg[128]; format(msg,sizeof msg,"Voce pescou um peixe! Venda da pesca: $%d | Peixes: %d",reward,NYX_FishCount[playerid]);
    return SendClientMessage(playerid,0x66CCFFFF,msg);
}

stock NYX_StopFishing(playerid)
{
    NYX_Fishing[playerid]=false;
    return SendClientMessage(playerid,0x66CCFFFF,"Pesca encerrada. Volte quando quiser.");
}

stock NYX_ShowPremiumFeatures(playerid)
{
    SendClientMessage(playerid,0x8B5CF6FF,"NYX DIFERENCIAIS: portal, ponte da ilha, estatua, shopping, cinema, igreja, pesca e leiloes.");
    return 1;
}
