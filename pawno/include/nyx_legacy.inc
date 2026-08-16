#if defined _NYX_LEGACY_INCLUDED
    #endinput
#endif
#define _NYX_LEGACY_INCLUDED

#define NYX_NEED_MAX 100
#define NYX_LEGACY_BUSINESS_COUNT 8

new NYX_Hunger[MAX_PLAYERS];
new NYX_Thirst[MAX_PLAYERS];
new NYX_Stress[MAX_PLAYERS];
new NYX_Passport[MAX_PLAYERS];
new NYX_GarageVehicle[MAX_PLAYERS];
new NYX_GarageModel[MAX_PLAYERS];
new NYX_RobberyCooldown[MAX_PLAYERS];
new NYX_MobileMode[MAX_PLAYERS];
new bool:NYX_Fishing[MAX_PLAYERS];
new NYX_FishCooldown[MAX_PLAYERS];

new const NYX_LegacyBusinessName[NYX_LEGACY_BUSINESS_COUNT][] = {
    "Mercado NYX", "Oficina NYX", "Hospital NYX", "Posto NYX",
    "Restaurante NYX", "Mercado Negro NYX", "Fabrica NYX", "Prefeitura NYX"
};
new const NYX_LegacyBusinessPrice[NYX_LEGACY_BUSINESS_COUNT] = {
    25000, 45000, 90000, 60000, 35000, 120000, 150000, 100000
};

stock NYX_LegacyInitPlayer(playerid)
{
    NYX_Hunger[playerid]=100;
    NYX_Thirst[playerid]=100;
    NYX_Stress[playerid]=0;
    NYX_Passport[playerid]=0;
    NYX_GarageVehicle[playerid]=INVALID_VEHICLE_ID;
    NYX_GarageModel[playerid]=0;
    NYX_RobberyCooldown[playerid]=0;
    NYX_MobileMode[playerid]=0;
    NYX_Fishing[playerid]=false;
    NYX_FishCooldown[playerid]=0;
    return 1;
}

stock NYX_LegacyTick(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    if(NYX_Hunger[playerid]>0) NYX_Hunger[playerid]--;
    if(NYX_Thirst[playerid]>0) NYX_Thirst[playerid]--;
    if(NYX_Stress[playerid]>0) NYX_Stress[playerid]--;
    if(NYX_RobberyCooldown[playerid]>0) NYX_RobberyCooldown[playerid]--;
    if(NYX_FishCooldown[playerid]>0) NYX_FishCooldown[playerid]--;
    if(NYX_Hunger[playerid] < 20 || NYX_Thirst[playerid] < 20)
        SendClientMessage(playerid,0xFFFF66FF,"[NYX] Suas necessidades estao baixas. Procure uma lanchonete ou mercado.");
    return 1;
}

stock NYX_Eat(playerid, amount=25)
{
    if(amount<1) amount=1;
    NYX_Hunger[playerid]+=amount;
    if(NYX_Hunger[playerid]>100) NYX_Hunger[playerid]=100;
    return SendClientMessage(playerid,0x66CCFFFF,"[NYX] Alimentacao realizada.");
}

stock NYX_Drink(playerid, amount=25)
{
    if(amount<1) amount=1;
    NYX_Thirst[playerid]+=amount;
    if(NYX_Thirst[playerid]>100) NYX_Thirst[playerid]=100;
    return SendClientMessage(playerid,0x66CCFFFF,"[NYX] Hidratacao realizada.");
}

stock NYX_ShowNeeds(playerid)
{
    new msg[128];
    format(msg,sizeof msg,"NYX | Fome: %d/100 | Sede: %d/100 | Estresse: %d/100",NYX_Hunger[playerid],NYX_Thirst[playerid],NYX_Stress[playerid]);
    return SendClientMessage(playerid,0xFFFFFFFF,msg);
}

stock NYX_IssuePassport(playerid)
{
    NYX_Passport[playerid]=1;
    return SendClientMessage(playerid,0x66CCFFFF,"[NYX] Passaporte emitido e vinculado ao personagem.");
}

stock NYX_ShowPassport(playerid)
{
    new name[MAX_PLAYER_NAME],msg[160];
    GetPlayerName(playerid,name,sizeof name);
    format(msg,sizeof msg,"NYX | PASSAPORTE\nNome: %s\nStatus: %s\nModo mobile: %s",name,NYX_Passport[playerid]?"EMITIDO":"PENDENTE",NYX_MobileMode[playerid]?"ATIVO":"PADRAO");
    return SendClientMessage(playerid,0xFFFFFFFF,msg);
}

stock NYX_SetMobileMode(playerid,bool:enabled)
{
    NYX_MobileMode[playerid]=enabled?1:0;
    return SendClientMessage(playerid,0x66CCFFFF,enabled?"[NYX] Perfil mobile ativado.":"[NYX] Perfil mobile desativado.");
}

stock NYX_ShowBusinesses(playerid)
{
    new msg[192];
    format(msg,sizeof msg,"NYX | Negocios: %s ($%d) | %s ($%d) | %s ($%d) | %s ($%d)",
        NYX_LegacyBusinessName[0],NYX_LegacyBusinessPrice[0],NYX_LegacyBusinessName[1],NYX_LegacyBusinessPrice[1],
        NYX_LegacyBusinessName[2],NYX_LegacyBusinessPrice[2],NYX_LegacyBusinessName[3],NYX_LegacyBusinessPrice[3]);
    return SendClientMessage(playerid,0xFFFFFFFF,msg);
}

stock NYX_OpenGarage(playerid)
{
    if(NYX_GarageModel[playerid]<=0) return SendClientMessage(playerid,0xFFFF66FF,"[NYX] Sua garagem ainda nao possui veiculo.");
    if(NYX_GarageVehicle[playerid]!=INVALID_VEHICLE_ID) DestroyVehicle(NYX_GarageVehicle[playerid]);
    new Float:x,Float:y,Float:z; GetPlayerPos(playerid,x,y,z);
    NYX_GarageVehicle[playerid]=CreateVehicle(NYX_GarageModel[playerid],x+3.0,y,z,0.0,-1,-1,300);
    return SendClientMessage(playerid,0x66CCFFFF,"[NYX] Veiculo retirado da garagem.");
}

stock NYX_StoreGarageVehicle(playerid,modelid)
{
    if(modelid<400 || modelid>611) return SendClientMessage(playerid,0xFF4D6DFF,"[NYX] Modelo de veiculo invalido.");
    NYX_GarageModel[playerid]=modelid;
    return SendClientMessage(playerid,0x66CCFFFF,"[NYX] Veiculo registrado na garagem.");
}

stock NYX_TryRobbery(playerid)
{
    if(NYX_RobberyCooldown[playerid]>0) return SendClientMessage(playerid,0xFFFF66FF,"[NYX] Aguarde antes de realizar outro assalto.");
    NYX_RobberyCooldown[playerid]=120;
    new reward=500+random(1500);
    GivePlayerMoney(playerid,reward);
    new msg[96]; format(msg,sizeof msg,"[NYX] Assalto concluido. Recompensa: $%d",reward);
    return SendClientMessage(playerid,0x66CCFFFF,msg);
}

stock NYX_ShowLegacySystems(playerid)
{
    SendClientMessage(playerid,0x8B5CF6FF,"[NYX] Sistemas integrados: necessidades, passaporte, garagem, negocios, assalto e modo mobile.");
    return 1;
}

stock NYX_StartFishing(playerid)
{
    if(NYX_Fishing[playerid]) return SendClientMessage(playerid,0xFFFF66FF,"[NYX] Voce ja esta pescando.");
    if(NYX_FishCooldown[playerid]>0) return SendClientMessage(playerid,0xFFFF66FF,"[NYX] Aguarde antes de pescar novamente.");
    NYX_Fishing[playerid]=true;
    SendClientMessage(playerid,0x66CCFFFF,"[NYX] Pescaria iniciada. Use /pescar para tentar fisgar um peixe.");
    return 1;
}

stock NYX_CatchFish(playerid)
{
    if(!NYX_Fishing[playerid]) return SendClientMessage(playerid,0xFFFF66FF,"[NYX] Voce nao esta pescando. Use /pesca.");
    if(random(100)<30) return SendClientMessage(playerid,0xFFFF66FF,"[NYX] Nada fisgou desta vez. Tente novamente.");
    new reward=150+random(351);
    GivePlayerMoney(playerid,reward);
    NYX_Fishing[playerid]=false;
    new msg[96]; format(msg,sizeof msg,"[NYX] Peixe capturado! Venda do peixe: +$%d",reward);
    return SendClientMessage(playerid,0x66CCFFFF,msg);
}

stock NYX_StopFishing(playerid)
{
    if(!NYX_Fishing[playerid]) return SendClientMessage(playerid,0xFFFF66FF,"[NYX] Voce nao esta pescando.");
    NYX_Fishing[playerid]=false;
    return SendClientMessage(playerid,0xFFFFFFFF,"[NYX] Pescaria encerrada.");
}

stock NYX_ShowPremiumFeatures(playerid)
{
    return SendClientMessage(playerid,0x8B5CF6FF,"[NYX PREMIUM] Loja: /loja | Skins e recursos premium sao controlados pelo saldo da conta.");
}
