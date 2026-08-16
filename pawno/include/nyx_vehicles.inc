#if defined _NYX_VEHICLES_INCLUDED
    #endinput
#endif
#define _NYX_VEHICLES_INCLUDED

#define NYX_VEHICLE_COUNT 16
#define NYX_VEHICLE_SLOTS 3
#define NYX_VEHICLE_PRICE 25000

new const NYX_VehicleCatalogModel[NYX_VEHICLE_COUNT] = {
    400,401,404,410,411,415,418,420,421,426,429,445,458,462,496,516
};
new const NYX_VehicleName[NYX_VEHICLE_COUNT][] = {
    "Landstalker","Bravura","Perennial","Manana","Infernus","Cheetah","Moonbeam","Taxi",
    "Washington","Premier","Banshee","Admiral","Solair","Faggio","Blista Compact","Nebula"
};

new NYX_VehicleOwnedModel[MAX_PLAYERS][NYX_VEHICLE_SLOTS];
new NYX_VehicleColor1[MAX_PLAYERS][NYX_VEHICLE_SLOTS];
new NYX_VehicleColor2[MAX_PLAYERS][NYX_VEHICLE_SLOTS];
new Float:NYX_VehicleFuel[MAX_PLAYERS][NYX_VEHICLE_SLOTS];
new Float:NYX_VehicleHealth[MAX_PLAYERS][NYX_VEHICLE_SLOTS];
new NYX_VehicleWorldId[MAX_PLAYERS][NYX_VEHICLE_SLOTS];

stock NYX_GetVehicleModel(index)
{
    if(index<0 || index>=NYX_VEHICLE_COUNT) return 400;
    return NYX_VehicleCatalogModel[index];
}

stock NYX_VehicleReset(playerid)
{
    for(new slot=0;slot<NYX_VEHICLE_SLOTS;slot++)
    {
        NYX_VehicleOwnedModel[playerid][slot]=0;
        NYX_VehicleColor1[playerid][slot]=1;
        NYX_VehicleColor2[playerid][slot]=1;
        NYX_VehicleFuel[playerid][slot]=100.0;
        NYX_VehicleHealth[playerid][slot]=1000.0;
        NYX_VehicleWorldId[playerid][slot]=INVALID_VEHICLE_ID;
    }
    return 1;
}

stock NYX_VehicleDestroy(playerid,slot)
{
    if(slot<0 || slot>=NYX_VEHICLE_SLOTS) return 0;
    if(NYX_VehicleWorldId[playerid][slot]!=INVALID_VEHICLE_ID)
    {
        DestroyVehicle(NYX_VehicleWorldId[playerid][slot]);
        NYX_VehicleWorldId[playerid][slot]=INVALID_VEHICLE_ID;
    }
    return 1;
}

stock NYX_VehicleRegister(playerid,slot,modelid)
{
    if(slot<0 || slot>=NYX_VEHICLE_SLOTS || modelid<400 || modelid>611) return 0;
    NYX_VehicleOwnedModel[playerid][slot]=modelid;
    NYX_VehicleColor1[playerid][slot]=1;
    NYX_VehicleColor2[playerid][slot]=1;
    NYX_VehicleFuel[playerid][slot]=100.0;
    NYX_VehicleHealth[playerid][slot]=1000.0;
    return 1;
}

stock NYX_VehicleSpawn(playerid,slot)
{
    if(slot<0 || slot>=NYX_VEHICLE_SLOTS || NYX_VehicleOwnedModel[playerid][slot]<=0) return 0;
    NYX_VehicleDestroy(playerid,slot);
    new Float:x,Float:y,Float:z,Float:a;
    GetPlayerPos(playerid,x,y,z); GetPlayerFacingAngle(playerid,a);
    NYX_VehicleWorldId[playerid][slot]=CreateVehicle(NYX_VehicleOwnedModel[playerid][slot],x+3.0,y,z,a,NYX_VehicleColor1[playerid][slot],NYX_VehicleColor2[playerid][slot],300);
    if(NYX_VehicleWorldId[playerid][slot]==INVALID_VEHICLE_ID) return 0;
    SetVehicleHealth(NYX_VehicleWorldId[playerid][slot],NYX_VehicleHealth[playerid][slot]);
    return 1;
}

stock NYX_VehicleStore(playerid,slot)
{
    if(slot<0 || slot>=NYX_VEHICLE_SLOTS) return 0;
    if(NYX_VehicleWorldId[playerid][slot]==INVALID_VEHICLE_ID) return 0;
    new Float:health;
    GetVehicleHealth(NYX_VehicleWorldId[playerid][slot],health);
    NYX_VehicleHealth[playerid][slot]=health;
    NYX_VehicleDestroy(playerid,slot);
    return 1;
}

stock NYX_VehicleBuy(playerid,slot,modelid)
{
    if(slot<0 || slot>=NYX_VEHICLE_SLOTS || modelid<400 || modelid>611) return 0;
    if(NYX_VehicleOwnedModel[playerid][slot]>0) return 0;
    if(GetPlayerMoney(playerid)<NYX_VEHICLE_PRICE) return 0;
    GivePlayerMoney(playerid,-NYX_VEHICLE_PRICE);
    NYX_SyncMoney(playerid);
    if(!NYX_VehicleRegister(playerid,slot,modelid))
    {
        GivePlayerMoney(playerid,NYX_VEHICLE_PRICE);
        NYX_SyncMoney(playerid);
        return 0;
    }
    NYX_SaveAccount(playerid);
    return 1;
}

stock NYX_VehicleList(playerid)
{
    new msg[128];
    SendClientMessage(playerid,COLOR_NYX,"[NYX] GARAGEM | 3 vagas persistentes");
    for(new slot=0;slot<NYX_VEHICLE_SLOTS;slot++)
    {
        if(NYX_VehicleOwnedModel[playerid][slot]<=0)
            format(msg,sizeof msg,"Vaga %d: vazia",slot+1);
        else
            format(msg,sizeof msg,"Vaga %d: modelo %d | combustivel %.0f%% | saude %.0f",slot+1,NYX_VehicleOwnedModel[playerid][slot],NYX_VehicleFuel[playerid][slot],NYX_VehicleHealth[playerid][slot]);
        SendClientMessage(playerid,COLOR_WHITE,msg);
    }
    return 1;
}

stock NYX_VehicleTick(playerid)
{
    for(new slot=0;slot<NYX_VEHICLE_SLOTS;slot++)
    {
        new vehicleid=NYX_VehicleWorldId[playerid][slot];
        if(vehicleid==INVALID_VEHICLE_ID) continue;
        if(GetPlayerVehicleID(playerid)==vehicleid && GetPlayerState(playerid)==PLAYER_STATE_DRIVER)
        {
            if(NYX_VehicleFuel[playerid][slot]>0.0) NYX_VehicleFuel[playerid][slot]-=0.25;
            if(NYX_VehicleFuel[playerid][slot]<0.0) NYX_VehicleFuel[playerid][slot]=0.0;
            if(NYX_VehicleFuel[playerid][slot]<=0.0) SetVehicleVelocity(vehicleid,0.0,0.0,0.0);
        }
    }
    return 1;
}
