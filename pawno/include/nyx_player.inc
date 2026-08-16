#if defined _NYX_PLAYER_INCLUDED
    #endinput
#endif
#define _NYX_PLAYER_INCLUDED

#define NYX_FIRST_SKIN_ID 0
#define NYX_LAST_SKIN_ID 311
#define NYX_BANK_START 10000

#include <nyx_inventory>
#include <nyx_vehicles>

enum E_NYX_PLAYER {
    NYX_Logged,
    NYX_Skin,
    NYX_Money,
    NYX_PlayerNCoins,
    NYX_Org,
    NYX_OrgRank,
    NYX_Admin
};
#define NYX_NCoins NYX_PlayerNCoins

new NYX_Player[MAX_PLAYERS][E_NYX_PLAYER];
new NYX_Bank[MAX_PLAYERS];
new NYX_AccountPassword[MAX_PLAYERS][64];
new bool:NYX_AccountLoaded[MAX_PLAYERS];

stock NYX_AccountFile(playerid, path[], size)
{
    new name[MAX_PLAYER_NAME]; GetPlayerName(playerid,name,sizeof name);
    format(path,size,"scriptfiles/nyx_%s.dat",name); return 1;
}

stock NYX_AccountExists(playerid)
{
    new path[96]; NYX_AccountFile(playerid,path,sizeof path); return fexist(path);
}

stock NYX_ResetPlayer(playerid)
{
    NYX_Player[playerid][NYX_Logged]=0;
    NYX_Player[playerid][NYX_Skin]=(playerid%2)?2:1;
    NYX_Player[playerid][NYX_Money]=NYX_START_MONEY;
    NYX_Player[playerid][NYX_NCoins]=0;
    NYX_Player[playerid][NYX_Org]=-1;
    NYX_Player[playerid][NYX_OrgRank]=0;
    NYX_Player[playerid][NYX_Admin]=0;
    NYX_Bank[playerid]=NYX_BANK_START;
    NYX_AccountPassword[playerid][0]=EOS;
    NYX_AccountLoaded[playerid]=false;
    NYX_InventoryReset(playerid);
    NYX_LegacyInitPlayer(playerid);
    NYX_VehicleReset(playerid);
    NYX_PoliceReset(playerid);
    return 1;
}

stock NYX_SaveAccount(playerid)
{
    if(!NYX_AccountLoaded[playerid]) return 0;
    NYX_LegacyTick(playerid);
    new path[96],line[128],file; NYX_AccountFile(playerid,path,sizeof path);
    file=fopen(path,io_write); if(!file) return 0;
    fwrite(file,NYX_AccountPassword[playerid]);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_Money]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Bank[playerid]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_Skin]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_NCoins]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_Org]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_OrgRank]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_Admin]); fwrite(file,line);
    for(new i=0;i<NYX_INV_SLOTS;i++)
    {
        format(line,sizeof line,"%d",NYX_InvItem[playerid][i]); fwrite(file,line);
        format(line,sizeof line,"%d",NYX_InvAmount[playerid][i]); fwrite(file,line);
    }
    for(new slot=0;slot<NYX_VEHICLE_SLOTS;slot++)
    {
        format(line,sizeof line,"%d",NYX_VehicleOwnedModel[playerid][slot]); fwrite(file,line);
        format(line,sizeof line,"%d",NYX_VehicleColor1[playerid][slot]); fwrite(file,line);
        format(line,sizeof line,"%d",NYX_VehicleColor2[playerid][slot]); fwrite(file,line);
        format(line,sizeof line,"%.2f",NYX_VehicleFuel[playerid][slot]); fwrite(file,line);
        format(line,sizeof line,"%.2f",NYX_VehicleHealth[playerid][slot]); fwrite(file,line);
    }
    fclose(file); return 1;
}

stock NYX_LoadAccount(playerid,password[])
{
    new path[96],line[128],file; NYX_AccountFile(playerid,path,sizeof path);
    file=fopen(path,io_read); if(!file) return 0;
    fread(file,line,sizeof line); line[sizeof line-1]=EOS;
    if(strcmp(line,password,false)!=0){fclose(file);return -1;}
    format(NYX_AccountPassword[playerid],64,"%s",password);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_Money]=strval(line);
    fread(file,line,sizeof line); NYX_Bank[playerid]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_Skin]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_NCoins]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_Org]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_OrgRank]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_Admin]=strval(line);
    NYX_InventoryReset(playerid);
    for(new i=0;i<NYX_INV_SLOTS;i++)
    {
        if(!fread(file,line,sizeof line)) break; NYX_InvItem[playerid][i]=strval(line);
        if(!fread(file,line,sizeof line)) break; NYX_InvAmount[playerid][i]=strval(line);
    }
    NYX_VehicleReset(playerid);
    for(new slot=0;slot<NYX_VEHICLE_SLOTS;slot++)
    {
        if(!fread(file,line,sizeof line)) break; NYX_VehicleOwnedModel[playerid][slot]=strval(line);
        if(!fread(file,line,sizeof line)) break; NYX_VehicleColor1[playerid][slot]=strval(line);
        if(!fread(file,line,sizeof line)) break; NYX_VehicleColor2[playerid][slot]=strval(line);
        if(!fread(file,line,sizeof line)) break; NYX_VehicleFuel[playerid][slot]=floatstr(line);
        if(!fread(file,line,sizeof line)) break; NYX_VehicleHealth[playerid][slot]=floatstr(line);
    }
    fclose(file); NYX_AccountLoaded[playerid]=true; return 1;
}

stock NYX_CreateAccount(playerid,password[])
{
    if(NYX_AccountExists(playerid)) return 0;
    format(NYX_AccountPassword[playerid],64,"%s",password);
    NYX_Player[playerid][NYX_Money]=NYX_START_MONEY; NYX_Bank[playerid]=NYX_BANK_START;
    NYX_Player[playerid][NYX_NCoins]=0; NYX_Player[playerid][NYX_Org]=-1; NYX_Player[playerid][NYX_OrgRank]=0; NYX_Player[playerid][NYX_Admin]=0;
    NYX_AccountLoaded[playerid]=true;
    NYX_InventoryGiveStarter(playerid); NYX_IssuePassport(playerid);
    return NYX_SaveAccount(playerid);
}

stock NYX_IsValidSkin(skinid){ return skinid>=NYX_FIRST_SKIN_ID && skinid<=NYX_LAST_SKIN_ID; }

stock NYX_SetupSpawn(playerid)
{
    SetPlayerPos(playerid,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z); SetPlayerFacingAngle(playerid,0.0);
    SetPlayerSkin(playerid,NYX_Player[playerid][NYX_Skin]); ResetPlayerMoney(playerid); GivePlayerMoney(playerid,NYX_Player[playerid][NYX_Money]); SetCameraBehindPlayer(playerid); return 1;
}

stock NYX_SyncMoney(playerid){ NYX_Player[playerid][NYX_Money]=GetPlayerMoney(playerid); return 1; }

stock NYX_Deposit(playerid,amount)
{
    if(amount<=0 || amount>GetPlayerMoney(playerid)) return 0;
    GivePlayerMoney(playerid,-amount); NYX_Bank[playerid]+=amount; NYX_SyncMoney(playerid); NYX_SaveAccount(playerid); return 1;
}

stock NYX_Withdraw(playerid,amount)
{
    if(amount<=0 || amount>NYX_Bank[playerid]) return 0;
    GivePlayerMoney(playerid,amount); NYX_Bank[playerid]-=amount; NYX_SyncMoney(playerid); NYX_SaveAccount(playerid); return 1;
}

stock NYX_VehicleSpawnOwned(playerid,slot)
{
    if(slot<0 || slot>=NYX_VEHICLE_SLOTS || NYX_VehicleOwnedModel[playerid][slot]<=0) return 0;
    NYX_VehicleDestroy(playerid,slot);
    new Float:x,Float:y,Float:z,Float:a; GetPlayerPos(playerid,x,y,z); GetPlayerFacingAngle(playerid,a);
    NYX_VehicleWorldId[playerid][slot]=CreateVehicle(NYX_VehicleOwnedModel[playerid][slot],x+3.0,y,z,a,NYX_VehicleColor1[playerid][slot],NYX_VehicleColor2[playerid][slot],300);
    if(NYX_VehicleWorldId[playerid][slot]==INVALID_VEHICLE_ID) return 0;
    SetVehicleHealth(NYX_VehicleWorldId[playerid][slot],NYX_VehicleHealth[playerid][slot]); return 1;
}

stock NYX_VehicleStoreOwned(playerid,slot)
{
    if(slot<0 || slot>=NYX_VEHICLE_SLOTS || NYX_VehicleWorldId[playerid][slot]==INVALID_VEHICLE_ID) return 0;
    new Float:health; GetVehicleHealth(NYX_VehicleWorldId[playerid][slot],health); NYX_VehicleHealth[playerid][slot]=health;
    NYX_VehicleDestroy(playerid,slot); NYX_SaveAccount(playerid); return 1;
}

stock NYX_VehicleBuyOwned(playerid,slot,modelid)
{
    if(slot<0 || slot>=NYX_VEHICLE_SLOTS || modelid<400 || modelid>611) return 0;
    if(NYX_VehicleOwnedModel[playerid][slot]>0 || GetPlayerMoney(playerid)<NYX_VEHICLE_PRICE) return 0;
    GivePlayerMoney(playerid,-NYX_VEHICLE_PRICE); NYX_SyncMoney(playerid);
    NYX_VehicleRegister(playerid,slot,modelid); NYX_SaveAccount(playerid); return 1;
}

stock NYX_VehicleFuelTick(playerid)
{
    for(new slot=0;slot<NYX_VEHICLE_SLOTS;slot++)
    {
        new vehicleid=NYX_VehicleWorldId[playerid][slot]; if(vehicleid==INVALID_VEHICLE_ID) continue;
        if(GetPlayerVehicleID(playerid)==vehicleid && GetPlayerState(playerid)==PLAYER_STATE_DRIVER)
        {
            NYX_VehicleFuel[playerid][slot]-=0.25; if(NYX_VehicleFuel[playerid][slot]<0.0) NYX_VehicleFuel[playerid][slot]=0.0;
            if(NYX_VehicleFuel[playerid][slot]<=0.0) SetVehicleVelocity(vehicleid,0.0,0.0,0.0);
        }
    }
    return 1;
}