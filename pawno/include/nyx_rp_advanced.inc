#if defined _NYX_RP_ADVANCED_INCLUDED
    #endinput
#endif
#define _NYX_RP_ADVANCED_INCLUDED

#define NYX_ADV_BUSINESS_COUNT 8
#define NYX_ADV_BUSINESS_MAX_CASH 2000000000
#define NYX_ADV_JAIL_X 1550.0
#define NYX_ADV_JAIL_Y -1675.0
#define NYX_ADV_JAIL_Z 15.0
#define NYX_ADV_RELEASE_SECONDS 120
#define NYX_ADV_MEDICAL_SECONDS 15
#define NYX_ADV_ORG_LEADER_RANK 5
#define NYX_ADV_ORG_COMMAND_RANK 3

new NYX_AdvBusinessOwner[NYX_ADV_BUSINESS_COUNT][MAX_PLAYER_NAME];
new NYX_AdvBusinessCash[NYX_ADV_BUSINESS_COUNT];
new NYX_AdvJailTime[MAX_PLAYERS];
new NYX_AdvMedicalTimer[MAX_PLAYERS];
new NYX_AdvOrgInvite[MAX_PLAYERS];
new bool:NYX_AdvSamuDuty[MAX_PLAYERS];
new NYX_AdvSamuCalls[MAX_PLAYERS];
new bool:NYX_AdvEngineOn[MAX_PLAYERS];

new const NYX_AdvBusinessName[NYX_ADV_BUSINESS_COUNT][] = {
    "Mercado NYX", "Oficina NYX", "Hospital NYX", "Posto NYX",
    "Restaurante NYX", "Mercado Negro NYX", "Fabrica NYX", "Prefeitura NYX"
};
new const NYX_AdvBusinessPrice[NYX_ADV_BUSINESS_COUNT] = {
    25000, 45000, 90000, 60000, 35000, 120000, 150000, 100000
};

forward NYX_AdvReleaseJail(playerid);
forward NYX_AdvMedicalRespawn(playerid);

stock NYX_AdvBusinessFile(path[], size)
{
    format(path,size,"scriptfiles/nyx_businesses.dat");
    return 1;
}

stock NYX_AdvBusinessReset()
{
    for(new i=0;i<NYX_ADV_BUSINESS_COUNT;i++)
    {
        NYX_AdvBusinessOwner[i][0]=EOS;
        NYX_AdvBusinessCash[i]=0;
    }
    return 1;
}

stock NYX_AdvBusinessSave()
{
    new path[96],line[128],file;
    NYX_AdvBusinessFile(path,sizeof path);
    file=fopen(path,io_write);
    if(!file) return 0;
    for(new i=0;i<NYX_ADV_BUSINESS_COUNT;i++)
    {
        fwrite(file,NYX_AdvBusinessOwner[i]);
        format(line,sizeof line,"%d",NYX_AdvBusinessCash[i]);
        fwrite(file,line);
    }
    fclose(file);
    return 1;
}

stock NYX_AdvBusinessLoad()
{
    new path[96],line[128],file;
    NYX_AdvBusinessReset();
    NYX_AdvBusinessFile(path,sizeof path);
    if(!fexist(path)) return NYX_AdvBusinessSave();
    file=fopen(path,io_read);
    if(!file) return 0;
    for(new i=0;i<NYX_ADV_BUSINESS_COUNT;i++)
    {
        if(!fread(file,line,sizeof line)) break;
        format(NYX_AdvBusinessOwner[i],MAX_PLAYER_NAME,"%s",line);
        if(!fread(file,line,sizeof line)) break;
        NYX_AdvBusinessCash[i]=strval(line);
    }
    fclose(file);
    return 1;
}

stock NYX_AdvInitPlayer(playerid)
{
    NYX_AdvJailTime[playerid]=0;
    NYX_AdvMedicalTimer[playerid]=INVALID_TIMER;
    NYX_AdvOrgInvite[playerid]=-1;
    NYX_AdvSamuDuty[playerid]=false;
    NYX_AdvSamuCalls[playerid]=0;
    NYX_AdvEngineOn[playerid]=false;
    return 1;
}

stock NYX_AdvPlayerTick(playerid)
{
    if(NYX_AdvJailTime[playerid]>0)
    {
        NYX_AdvJailTime[playerid]--;
        if(NYX_AdvJailTime[playerid]<=0) NYX_AdvReleaseJail(playerid);
        else if(NYX_AdvJailTime[playerid]%30==0)
        {
            new msg[96];
            format(msg,sizeof msg,"[NYX] Tempo restante de prisao: %d segundos.",NYX_AdvJailTime[playerid]);
            SendClientMessage(playerid,COLOR_WARNING,msg);
        }
    }
    if(NYX_AdvSamuCalls[playerid]>0 && !NYX_AdvSamuDuty[playerid]) NYX_AdvSamuCalls[playerid]=0;
    return 1;
}

stock bool:NYX_AdvIsLeader(playerid)
{
    return NYX_IsValidOrg(NYX_Player[playerid][NYX_Org]) && NYX_Player[playerid][NYX_OrgRank]>=NYX_ADV_ORG_LEADER_RANK;
}

stock NYX_AdvOrgList(playerid)
{
    if(!NYX_IsValidOrg(NYX_Player[playerid][NYX_Org])) return SendClientMessage(playerid,COLOR_WARNING,"Voce esta sem organizacao. Use /orgs para consultar as organizacoes disponiveis.");
    new msg[144];
    format(msg,sizeof msg,"[ORG] %s | Cargo %d/%d | Lider: %s",NYX_OrgName[NYX_Player[playerid][NYX_Org]],NYX_Player[playerid][NYX_OrgRank],NYX_MAX_RANKS,NYX_AdvIsLeader(playerid)?"SIM":"NAO");
    SendClientMessage(playerid,COLOR_NYX,msg);
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(!IsPlayerConnected(i)||!NYX_Player[i][NYX_Logged]) continue;
        if(NYX_Player[i][NYX_Org]!=NYX_Player[playerid][NYX_Org]) continue;
        format(msg,sizeof msg,"ID %d | Cargo %d | %s",i,NYX_Player[i][NYX_OrgRank],NYX_Player[i][NYX_OrgRank]>=NYX_ADV_ORG_LEADER_RANK?"LIDER":"MEMBRO");
        SendClientMessage(playerid,COLOR_WHITE,msg);
    }
    return 1;
}

stock NYX_AdvOrgInvite(playerid,target)
{
    if(!NYX_AdvIsLeader(playerid)) return SendClientMessage(playerid,COLOR_ERROR,"Apenas lideres podem recrutar membros.");
    if(target==playerid || !IsPlayerConnected(target) || !NYX_Player[target][NYX_Logged]) return SendClientMessage(playerid,COLOR_ERROR,"Jogador invalido.");
    NYX_AdvOrgInvite[target]=NYX_Player[playerid][NYX_Org];
    new msg[144];
    format(msg,sizeof msg,"Convite enviado para %s.",NYX_OrgName[NYX_Player[playerid][NYX_Org]]);
    SendClientMessage(playerid,COLOR_SUCCESS,msg);
    format(msg,sizeof msg,"Voce recebeu convite para %s. Use /orgaceitar ou /orgrecusar.",NYX_OrgName[NYX_Player[playerid][NYX_Org]]);
    return SendClientMessage(target,COLOR_NYX,msg);
}

stock NYX_AdvOrgAccept(playerid)
{
    new org=NYX_AdvOrgInvite[playerid];
    if(!NYX_IsValidOrg(org)) return SendClientMessage(playerid,COLOR_WARNING,"Voce nao possui convite de organizacao.");
    NYX_Player[playerid][NYX_Org]=org;
    NYX_Player[playerid][NYX_OrgRank]=1;
    NYX_AdvOrgInvite[playerid]=-1;
    NYX_SaveAccount(playerid);
    new msg[128]; format(msg,sizeof msg,"Voce entrou na organizacao %s como cargo 1.",NYX_OrgName[org]);
    return SendClientMessage(playerid,COLOR_SUCCESS,msg);
}

stock NYX_AdvOrgLeave(playerid)
{
    if(!NYX_IsValidOrg(NYX_Player[playerid][NYX_Org])) return SendClientMessage(playerid,COLOR_WARNING,"Voce nao possui organizacao.");
    if(NYX_AdvIsLeader(playerid)) return SendClientMessage(playerid,COLOR_WARNING,"O lider deve transferir a lideranca antes de sair.");
    NYX_Player[playerid][NYX_Org]=-1; NYX_Player[playerid][NYX_OrgRank]=0; NYX_SaveAccount(playerid);
    return SendClientMessage(playerid,COLOR_SUCCESS,"Voce saiu da organizacao.");
}

stock NYX_AdvOrgSetRank(playerid,target,rank)
{
    if(!NYX_AdvIsLeader(playerid)) return SendClientMessage(playerid,COLOR_ERROR,"Apenas lideres podem alterar cargos.");
    if(target==playerid || !IsPlayerConnected(target) || !NYX_Player[target][NYX_Logged]) return SendClientMessage(playerid,COLOR_ERROR,"Jogador invalido.");
    if(NYX_Player[target][NYX_Org]!=NYX_Player[playerid][NYX_Org]) return SendClientMessage(playerid,COLOR_ERROR,"O jogador nao pertence a sua organizacao.");
    if(rank<1 || rank>NYX_MAX_RANKS || rank>=NYX_Player[playerid][NYX_OrgRank]) return SendClientMessage(playerid,COLOR_ERROR,"Cargo invalido ou superior ao seu.");
    NYX_Player[target][NYX_OrgRank]=rank; NYX_SaveAccount(target);
    new msg[128]; format(msg,sizeof msg,"Cargo de %d alterado para %d.",target,rank); SendClientMessage(playerid,COLOR_SUCCESS,msg);
    format(msg,sizeof msg,"Seu cargo na organizacao foi alterado para %d.",rank); return SendClientMessage(target,COLOR_NYX,msg);
}

stock NYX_AdvOrgKick(playerid,target)
{
    if(!NYX_AdvIsLeader(playerid)) return SendClientMessage(playerid,COLOR_ERROR,"Apenas lideres podem remover membros.");
    if(target==playerid || !IsPlayerConnected(target)) return SendClientMessage(playerid,COLOR_ERROR,"Jogador invalido.");
    if(NYX_Player[target][NYX_Org]!=NYX_Player[playerid][NYX_Org]) return SendClientMessage(playerid,COLOR_ERROR,"O jogador nao pertence a sua organizacao.");
    if(NYX_Player[target][NYX_OrgRank]>=NYX_Player[playerid][NYX_OrgRank]) return SendClientMessage(playerid,COLOR_ERROR,"Voce nao pode remover cargo igual ou superior.");
    NYX_Player[target][NYX_Org]=-1; NYX_Player[target][NYX_OrgRank]=0; NYX_SaveAccount(target);
    SendClientMessage(target,COLOR_WARNING,"Voce foi removido da organizacao."); return SendClientMessage(playerid,COLOR_SUCCESS,"Membro removido.");
}

stock NYX_AdvBusinessInfo(playerid)
{
    new msg[192]; SendClientMessage(playerid,COLOR_NYX,"[NEGOCIOS] Empresas NYX:");
    for(new i=0;i<NYX_ADV_BUSINESS_COUNT;i++)
    {
        format(msg,sizeof msg,"ID %d | %s | $%d | Dono: %s | Caixa: $%d",i+1,NYX_AdvBusinessName[i],NYX_AdvBusinessPrice[i],NYX_AdvBusinessOwner[i][0]?NYX_AdvBusinessOwner[i]:"SEM DONO",NYX_AdvBusinessCash[i]);
        SendClientMessage(playerid,COLOR_WHITE,msg);
    }
    return 1;
}

stock NYX_AdvBusinessBuy(playerid,bizid)
{
    bizid--; if(bizid<0||bizid>=NYX_ADV_BUSINESS_COUNT) return SendClientMessage(playerid,COLOR_ERROR,"Negocio invalido.");
    if(NYX_AdvBusinessOwner[bizid][0]) return SendClientMessage(playerid,COLOR_WARNING,"Este negocio ja possui proprietario.");
    if(GetPlayerMoney(playerid)<NYX_AdvBusinessPrice[bizid]) return SendClientMessage(playerid,COLOR_ERROR,"Dinheiro insuficiente.");
    new name[MAX_PLAYER_NAME]; GetPlayerName(playerid,name,sizeof name); format(NYX_AdvBusinessOwner[bizid],MAX_PLAYER_NAME,"%s",name);
    GivePlayerMoney(playerid,-NYX_AdvBusinessPrice[bizid]); NYX_SyncMoney(playerid); NYX_SaveAccount(playerid); NYX_AdvBusinessSave();
    new msg[144]; format(msg,sizeof msg,"Negocio comprado: %s.",NYX_AdvBusinessName[bizid]); return SendClientMessage(playerid,COLOR_SUCCESS,msg);
}

stock bool:NYX_AdvBusinessOwnerCheck(playerid,bizid)
{
    if(bizid<0||bizid>=NYX_ADV_BUSINESS_COUNT) return false;
    new name[MAX_PLAYER_NAME]; GetPlayerName(playerid,name,sizeof name); return !strcmp(NYX_AdvBusinessOwner[bizid],name,true);
}

stock NYX_AdvBusinessDeposit(playerid,bizid,amount)
{
    bizid--; if(bizid<0||bizid>=NYX_ADV_BUSINESS_COUNT) return SendClientMessage(playerid,COLOR_ERROR,"Negocio invalido.");
    if(!NYX_AdvBusinessOwnerCheck(playerid,bizid)) return SendClientMessage(playerid,COLOR_ERROR,"Voce nao e proprietario.");
    if(amount<=0||amount>GetPlayerMoney(playerid)) return SendClientMessage(playerid,COLOR_ERROR,"Valor invalido ou dinheiro insuficiente.");
    if(NYX_AdvBusinessCash[bizid]>NYX_ADV_BUSINESS_MAX_CASH-amount) return SendClientMessage(playerid,COLOR_ERROR,"Limite de caixa atingido.");
    GivePlayerMoney(playerid,-amount); NYX_AdvBusinessCash[bizid]+=amount; NYX_SyncMoney(playerid); NYX_SaveAccount(playerid); NYX_AdvBusinessSave();
    return SendClientMessage(playerid,COLOR_SUCCESS,"Valor depositado no caixa.");
}

stock NYX_AdvBusinessWithdraw(playerid,bizid,amount)
{
    bizid--; if(bizid<0||bizid>=NYX_ADV_BUSINESS_COUNT) return SendClientMessage(playerid,COLOR_ERROR,"Negocio invalido.");
    if(!NYX_AdvBusinessOwnerCheck(playerid,bizid)) return SendClientMessage(playerid,COLOR_ERROR,"Voce nao e proprietario.");
    if(amount<=0||amount>NYX_AdvBusinessCash[bizid]) return SendClientMessage(playerid,COLOR_ERROR,"Caixa insuficiente.");
    NYX_AdvBusinessCash[bizid]-=amount; GivePlayerMoney(playerid,amount); NYX_SyncMoney(playerid); NYX_SaveAccount(playerid); NYX_AdvBusinessSave();
    return SendClientMessage(playerid,COLOR_SUCCESS,"Valor retirado do caixa.");
}

stock NYX_AdvPoliceDistance(playerid,target)
{
    if(target==INVALID_PLAYER_ID||!IsPlayerConnected(target)) return 0;
    new Float:x,Float:y,Float:z; GetPlayerPos(playerid,x,y,z); return IsPlayerInRangeOfPoint(target,5.0,x,y,z);
}

stock NYX_AdvPoliceArrest(playerid,target)
{
    if(!NYX_IsPolice(playerid)) return SendClientMessage(playerid,COLOR_ERROR,"Acesso restrito a policia.");
    if(!NYX_AdvPoliceDistance(playerid,target)) return SendClientMessage(playerid,COLOR_WARNING,"Aproxime-se do suspeito.");
    if(NYX_WantedLevel[target]<=0&&!NYX_PoliceCuffed[target]) return SendClientMessage(playerid,COLOR_WARNING,"O suspeito nao esta procurado nem algemado.");
    NYX_PoliceCuffed[target]=0; TogglePlayerControllable(target,1); SetPlayerSpecialAction(target,SPECIAL_ACTION_NONE); SetPlayerWantedLevel(target,0); NYX_WantedLevel[target]=0;
    NYX_AdvJailTime[target]=NYX_ADV_RELEASE_SECONDS; SetPlayerPos(target,NYX_ADV_JAIL_X,NYX_ADV_JAIL_Y,NYX_ADV_JAIL_Z); SetPlayerInterior(target,0);
    SendClientMessage(playerid,COLOR_SUCCESS,"Prisao efetuada."); SendClientMessage(target,COLOR_WARNING,"Voce foi preso. Use /pena para consultar o tempo."); NYX_SaveAccount(target); return 1;
}

public NYX_AdvReleaseJail(playerid)
{
    if(!IsPlayerConnected(playerid)) return 1;
    NYX_AdvJailTime[playerid]=0; SetPlayerPos(playerid,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z); TogglePlayerControllable(playerid,1);
    return SendClientMessage(playerid,COLOR_SUCCESS,"Sua pena terminou. Voce foi liberado.");
}

stock NYX_AdvMedicalDeath(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    NYX_MedicalDead[playerid]=true; NYX_MedicalInjury[playerid]=100;
    NYX_AdvMedicalTimer[playerid]=SetTimerEx("NYX_AdvMedicalRespawn",NYX_ADV_MEDICAL_SECONDS*1000,false,"i",playerid);
    return SendClientMessage(playerid,COLOR_WARNING,"Voce esta inconsciente. Aguarde o atendimento medico.");
}

public NYX_AdvMedicalRespawn(playerid)
{
    if(!IsPlayerConnected(playerid)) return 1;
    NYX_AdvMedicalTimer[playerid]=INVALID_TIMER; NYX_MedicalDead[playerid]=false; NYX_MedicalInjury[playerid]=0;
    SpawnPlayer(playerid); SetPlayerPos(playerid,NYX_MEDICAL_HOSPITAL_X,NYX_MEDICAL_HOSPITAL_Y,NYX_MEDICAL_HOSPITAL_Z); SetPlayerHealth(playerid,NYX_MEDICAL_MAX_HEALTH);
    return SendClientMessage(playerid,COLOR_SUCCESS,"Atendimento concluido. Voce acordou no Hospital NYX.");
}

stock NYX_AdvSamuCall(playerid)
{
    NYX_AdvSamuCalls[playerid]=1;
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(!IsPlayerConnected(i)||!NYX_Player[i][NYX_Logged]) continue;
        if(NYX_Player[i][NYX_Org]==9) { NYX_AdvSamuCalls[i]=1; SendClientMessage(i,COLOR_NYX,"[SAMU] Nova chamada. Use /samuatender ID."); }
    }
    return SendClientMessage(playerid,COLOR_SUCCESS,"Chamado SAMU enviado.");
}

stock NYX_AdvSamuAttend(playerid,target)
{
    if(NYX_Player[playerid][NYX_Org]!=9) return SendClientMessage(playerid,COLOR_ERROR,"Acesso restrito ao SAMU.");
    if(target==INVALID_PLAYER_ID||!IsPlayerConnected(target)) return SendClientMessage(playerid,COLOR_ERROR,"Paciente invalido.");
    if(!NYX_AdvPoliceDistance(playerid,target)) return SendClientMessage(playerid,COLOR_WARNING,"Aproxime-se do paciente.");
    SetPlayerHealth(target,NYX_MEDICAL_MAX_HEALTH); NYX_MedicalDead[target]=false; NYX_MedicalInjury[target]=0; NYX_MedicalCooldown[target]=10; NYX_AdvSamuCalls[target]=0;
    GivePlayerMoney(playerid,250); NYX_SyncMoney(playerid); NYX_SaveAccount(playerid); SendClientMessage(target,COLOR_SUCCESS,"O SAMU realizou seu atendimento.");
    return SendClientMessage(playerid,COLOR_SUCCESS,"Atendimento concluido. Pagamento: $250.");
}

stock NYX_AdvVehicleToggle(playerid)
{
    if(GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return SendClientMessage(playerid,COLOR_WARNING,"Voce precisa estar dirigindo.");
    new vehicleid=GetPlayerVehicleID(playerid); if(vehicleid==INVALID_VEHICLE_ID) return SendClientMessage(playerid,COLOR_WARNING,"Veiculo invalido.");
    NYX_AdvEngineOn[playerid]=!NYX_AdvEngineOn[playerid]; if(!NYX_AdvEngineOn[playerid]) SetVehicleVelocity(vehicleid,0.0,0.0,0.0);
    return SendClientMessage(playerid,NYX_AdvEngineOn[playerid]?COLOR_SUCCESS:COLOR_WARNING,NYX_AdvEngineOn[playerid]?"Motor ligado.":"Motor desligado.");
}

stock NYX_AdvHelp(playerid)
{
    SendClientMessage(playerid,COLOR_NYX,"[NYX RP] /orglista /orgconvidar ID /orgaceitar /orgrecusar /orgsair /orgcargo ID CARGO /orgexpulsar ID");
    SendClientMessage(playerid,COLOR_NYX,"[NYX RP] /negocios /comprarnegocio ID /caixanegocio ID /depositonegocio ID VALOR /saquenegocio ID VALOR");
    SendClientMessage(playerid,COLOR_NYX,"[NYX RP] /chamarsamu /samuatender ID /pena /motor /statuscompleto");
    return 1;
}

stock NYX_AdvCommands(playerid,cmdtext[])
{
    if(!strcmp(cmdtext,"/rpajuda",true)) return NYX_AdvHelp(playerid);
    if(!strcmp(cmdtext,"/orglista",true)) return NYX_AdvOrgList(playerid);
    if(strfind(cmdtext,"/orgconvidar ",true)==0) return NYX_AdvOrgInvite(playerid,strval(cmdtext[13]));
    if(!strcmp(cmdtext,"/orgaceitar",true)) return NYX_AdvOrgAccept(playerid);
    if(!strcmp(cmdtext,"/orgrecusar",true)){NYX_AdvOrgInvite[playerid]=-1;return SendClientMessage(playerid,COLOR_WHITE,"Convite recusado.");}
    if(!strcmp(cmdtext,"/orgsair",true)) return NYX_AdvOrgLeave(playerid);
    if(strfind(cmdtext,"/orgcargo ",true)==0)
    {
        new rest[64],target,rank; format(rest,sizeof rest,"%s",cmdtext[10]); target=strval(rest); new p=strfind(rest," ",true);
        if(p<0) return SendClientMessage(playerid,COLOR_WARNING,"Uso: /orgcargo ID CARGO"); rank=strval(rest[p+1]); return NYX_AdvOrgSetRank(playerid,target,rank);
    }
    if(strfind(cmdtext,"/orgexpulsar ",true)==0) return NYX_AdvOrgKick(playerid,strval(cmdtext[13]));
    if(!strcmp(cmdtext,"/negocios",true)) return NYX_AdvBusinessInfo(playerid);
    if(strfind(cmdtext,"/comprarnegocio ",true)==0) return NYX_AdvBusinessBuy(playerid,strval(cmdtext[16]));
    if(strfind(cmdtext,"/caixanegocio ",true)==0)
    {
        new id=strval(cmdtext[14])-1; if(id<0||id>=NYX_ADV_BUSINESS_COUNT) return SendClientMessage(playerid,COLOR_ERROR,"Negocio invalido.");
        if(!NYX_AdvBusinessOwnerCheck(playerid,id)) return SendClientMessage(playerid,COLOR_ERROR,"Voce nao e proprietario.");
        new msg[96]; format(msg,sizeof msg,"Caixa de %s: $%d",NYX_AdvBusinessName[id],NYX_AdvBusinessCash[id]); return SendClientMessage(playerid,COLOR_SUCCESS,msg);
    }
    if(strfind(cmdtext,"/depositonegocio ",true)==0)
    {
        new rest[64],id,amount; format(rest,sizeof rest,"%s",cmdtext[17]); id=strval(rest); new p=strfind(rest," ",true); if(p<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /depositonegocio ID VALOR"); amount=strval(rest[p+1]); return NYX_AdvBusinessDeposit(playerid,id,amount);
    }
    if(strfind(cmdtext,"/saquenegocio ",true)==0)
    {
        new rest[64],id,amount; format(rest,sizeof rest,"%s",cmdtext[14]); id=strval(rest); new p=strfind(rest," ",true); if(p<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /saquenegocio ID VALOR"); amount=strval(rest[p+1]); return NYX_AdvBusinessWithdraw(playerid,id,amount);
    }
    if(!strcmp(cmdtext,"/chamarsamu",true)) return NYX_AdvSamuCall(playerid);
    if(strfind(cmdtext,"/samuatender ",true)==0) return NYX_AdvSamuAttend(playerid,strval(cmdtext[13]));
    if(!strcmp(cmdtext,"/motor",true)) return NYX_AdvVehicleToggle(playerid);
    if(!strcmp(cmdtext,"/pena",true)){new msg[96];format(msg,sizeof msg,"Tempo restante de prisao: %d segundos.",NYX_AdvJailTime[playerid]);return SendClientMessage(playerid,COLOR_WHITE,msg);}
    return 0;
}
