#if defined _NYX_COMPLETION_INCLUDED
    #endinput
#endif
#define _NYX_COMPLETION_INCLUDED

stock NYX_HandleCompletionCommand(playerid, cmdtext[])
{
    if(NYX_PoliceCommands(playerid,cmdtext)) return 1;
    if(!strcmp(cmdtext,"/necessidades",true)) return NYX_ShowNeeds(playerid);
    if(!strcmp(cmdtext,"/comer",true)) return NYX_Eat(playerid,25);
    if(!strcmp(cmdtext,"/beber",true)) return NYX_Drink(playerid,25);
    if(!strcmp(cmdtext,"/passaporte",true)) return NYX_ShowPassport(playerid);
    if(!strcmp(cmdtext,"/emitirpassaporte",true)) return NYX_IssuePassport(playerid);
    if(!strcmp(cmdtext,"/mobile",true)) return NYX_SetMobileMode(playerid,true);
    if(!strcmp(cmdtext,"/mobileoff",true)) return NYX_SetMobileMode(playerid,false);
    if(!strcmp(cmdtext,"/negocios",true)) return NYX_ShowBusinesses(playerid);
    if(!strcmp(cmdtext,"/assaltar",true)) return NYX_TryRobbery(playerid);
    if(!strcmp(cmdtext,"/sistemas",true)) return NYX_ShowLegacySystems(playerid);
    if(!strcmp(cmdtext,"/garagem",true)) return NYX_VehicleList(playerid);
    if(!strcmp(cmdtext,"/veiculos",true)) return NYX_VehicleList(playerid);
    if(!strcmp(cmdtext,"/pesca",true)) return NYX_StartFishing(playerid);
    if(!strcmp(cmdtext,"/pescar",true)) return NYX_CatchFish(playerid);
    if(!strcmp(cmdtext,"/pararpesca",true)) return NYX_StopFishing(playerid);
    if(!strcmp(cmdtext,"/diferenciais",true)) return NYX_ShowPremiumFeatures(playerid);
    if(NYX_InventoryCommands(playerid,cmdtext)) return 1;
    if(strfind(cmdtext,"/guardarveiculo ",true)==0)
    {
        new model=strval(cmdtext[16]);
        return NYX_StoreGarageVehicle(playerid,model);
    }
    if(strfind(cmdtext,"/comprarcarro ",true)==0)
    {
        new slot=strval(cmdtext[14])-1;
        new model=0;
        new p=strfind(cmdtext," ",true,14);
        if(p!=-1) model=strval(cmdtext[p+1]);
        if(model<=0) return SendClientMessage(playerid,COLOR_WARNING,"Use /comprarcarro VAGA MODELO. Ex.: /comprarcarro 1 411");
        if(!NYX_VehicleBuyOwned(playerid,slot,model)) return SendClientMessage(playerid,COLOR_ERROR,"Nao foi possivel comprar: vaga ocupada, modelo invalido ou dinheiro insuficiente.");
        return SendClientMessage(playerid,COLOR_SUCCESS,"Veiculo comprado e salvo na garagem.");
    }
    if(strfind(cmdtext,"/retirarcarro ",true)==0)
    {
        new slot=strval(cmdtext[14])-1;
        if(!NYX_VehicleSpawnOwned(playerid,slot)) return SendClientMessage(playerid,COLOR_ERROR,"Vaga invalida ou sem veiculo.");
        return SendClientMessage(playerid,COLOR_SUCCESS,"Veiculo retirado da garagem.");
    }
    if(strfind(cmdtext,"/guardarcarro ",true)==0)
    {
        new slot=strval(cmdtext[14])-1;
        if(!NYX_VehicleStoreOwned(playerid,slot)) return SendClientMessage(playerid,COLOR_WARNING,"Nao ha veiculo dessa vaga ativo para guardar.");
        return SendClientMessage(playerid,COLOR_SUCCESS,"Veiculo guardado e estado salvo.");
    }
    if(!strcmp(cmdtext,"/statuscompleto",true))
    {
        new msg[220];
        format(msg,sizeof msg,"NYX | Fome %d | Sede %d | Stress %d | Passaporte %s | Mobile %s | Cooldown assalto %d",NYX_Hunger[playerid],NYX_Thirst[playerid],NYX_Stress[playerid],NYX_Passport[playerid]?"OK":"PENDENTE",NYX_MobileMode[playerid]?"ON":"OFF",NYX_RobberyCooldown[playerid]);
        return SendClientMessage(playerid,COLOR_WHITE,msg);
    }
    return 0;
}
