#if defined _NYX_COMPLETION_INCLUDED
    #endinput
#endif
#define _NYX_COMPLETION_INCLUDED

// Consolidated gameplay bridge. Features only count when they execute through
// the NYX command path and survive the Pawn build.

stock NYX_HandleCompletionCommand(playerid, cmdtext[])
{
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
    if(!strcmp(cmdtext,"/garagem",true)) return NYX_OpenGarage(playerid);
    if(!strcmp(cmdtext,"/pesca",true)) return NYX_StartFishing(playerid);
    if(!strcmp(cmdtext,"/pescar",true)) return NYX_CatchFish(playerid);
    if(!strcmp(cmdtext,"/pararpesca",true)) return NYX_StopFishing(playerid);
    if(!strcmp(cmdtext,"/diferenciais",true)) return NYX_ShowPremiumFeatures(playerid);

    if(strfind(cmdtext,"/guardarveiculo ",true)==0)
    {
        new model=strval(cmdtext[16]);
        return NYX_StoreGarageVehicle(playerid,model);
    }
    if(strfind(cmdtext,"/statuscompleto",true)==0)
    {
        new msg[220];
        format(msg,sizeof msg,"NYX | Fome %d | Sede %d | Stress %d | Passaporte %s | Mobile %s | Cooldown assalto %d",NYX_Hunger[playerid],NYX_Thirst[playerid],NYX_Stress[playerid],NYX_Passport[playerid]?"OK":"PENDENTE",NYX_MobileMode[playerid]?"ON":"OFF",NYX_RobberyCooldown[playerid]);
        return SendClientMessage(playerid,COLOR_WHITE,msg);
    }
    return 0;
}
