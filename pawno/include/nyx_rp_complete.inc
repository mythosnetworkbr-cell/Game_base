#if defined _NYX_RP_COMPLETE_INCLUDED
    #endinput
#endif
#define _NYX_RP_COMPLETE_INCLUDED

#define NYX2_BIZ_COUNT 8
#define NYX2_JAIL_SECONDS 120
#define NYX2_MEDICAL_SECONDS 15
#define NYX2_LEADER_RANK 5
#define NYX2_INVALID_TIMER -1

new NYX2_BizOwner[NYX2_BIZ_COUNT][MAX_PLAYER_NAME];
new NYX2_BizCash[NYX2_BIZ_COUNT];
new NYX2_Jail[MAX_PLAYERS];
new NYX2_MedicalTimer[MAX_PLAYERS];
new NYX2_Invite[MAX_PLAYERS];
new bool:NYX2_SamuDuty[MAX_PLAYERS];
new bool:NYX2_Engine[MAX_PLAYERS];

new const NYX2_BizName[NYX2_BIZ_COUNT][] = {"Mercado NYX","Oficina NYX","Hospital NYX","Posto NYX","Restaurante NYX","Mercado Negro NYX","Fabrica NYX","Prefeitura NYX"};
new const NYX2_BizPrice[NYX2_BIZ_COUNT] = {25000,45000,90000,60000,35000,120000,150000,100000};

forward NYX2_ReleaseJail(playerid);
forward NYX2_MedicalRespawn(playerid);

stock NYX2_Init()
{
    new path[96],File:file;format(path,sizeof path,"scriptfiles/nyx_businesses.dat");
    if(fexist(path))
    {
        file=fopen(path,io_read);
        if(file)
        {
            new line[64];
            for(new i=0;i<NYX2_BIZ_COUNT;i++)
            {
                if(!fread(file,line,sizeof line))break;format(NYX2_BizOwner[i],MAX_PLAYER_NAME,"%s",line);
                if(!fread(file,line,sizeof line))break;NYX2_BizCash[i]=strval(line);
            }
            fclose(file);
        }
    }
    return 1;
}

stock NYX2_SaveBusinesses()
{
    new path[96],line[64],File:file;format(path,sizeof path,"scriptfiles/nyx_businesses.dat");file=fopen(path,io_write);if(!file)return 0;
    for(new i=0;i<NYX2_BIZ_COUNT;i++){fwrite(file,NYX2_BizOwner[i]);format(line,sizeof line,"%d",NYX2_BizCash[i]);fwrite(file,line);}fclose(file);return 1;
}

stock NYX2_InitPlayer(playerid)
{
    NYX2_Jail[playerid]=0;NYX2_MedicalTimer[playerid]=NYX2_INVALID_TIMER;NYX2_Invite[playerid]=-1;NYX2_SamuDuty[playerid]=false;NYX2_Engine[playerid]=false;return 1;
}

stock NYX2_Tick(playerid)
{
    if(NYX2_Jail[playerid]>0){NYX2_Jail[playerid]--;if(NYX2_Jail[playerid]<=0)NYX2_ReleaseJail(playerid);}
    return 1;
}

stock bool:NYX2_Leader(playerid){return NYX_IsValidOrg(NYX_Player[playerid][NYX_Org])&&NYX_Player[playerid][NYX_OrgRank]>=NYX2_LEADER_RANK;}

stock NYX2_OrgList(playerid)
{
    if(!NYX_IsValidOrg(NYX_Player[playerid][NYX_Org]))return SendClientMessage(playerid,COLOR_WARNING,"Voce nao possui organizacao.");
    new text[128];format(text,sizeof text,"[ORG] %s | Cargo %d/%d",NYX_OrgName[NYX_Player[playerid][NYX_Org]],NYX_Player[playerid][NYX_OrgRank],NYX_MAX_RANKS);SendClientMessage(playerid,COLOR_NYX,text);
    for(new i=0;i<MAX_PLAYERS;i++)if(IsPlayerConnected(i)&&NYX_Player[i][NYX_Logged]&&NYX_Player[i][NYX_Org]==NYX_Player[playerid][NYX_Org]){format(text,sizeof text,"ID %d | Cargo %d",i,NYX_Player[i][NYX_OrgRank]);SendClientMessage(playerid,COLOR_WHITE,text);}return 1;
}

stock NYX2_OrgInvitePlayer(playerid,target)
{
    if(!NYX2_Leader(playerid))return SendClientMessage(playerid,COLOR_ERROR,"Apenas o lider pode recrutar.");
    if(target==playerid||!IsPlayerConnected(target)||!NYX_Player[target][NYX_Logged])return SendClientMessage(playerid,COLOR_ERROR,"Jogador invalido.");
    NYX2_Invite[target]=NYX_Player[playerid][NYX_Org];new text[128];format(text,sizeof text,"Convite enviado para %s.",NYX_OrgName[NYX_Player[playerid][NYX_Org]]);SendClientMessage(playerid,COLOR_SUCCESS,text);format(text,sizeof text,"Convite para %s. Use /orgaceitar.",NYX_OrgName[NYX_Player[playerid][NYX_Org]]);return SendClientMessage(target,COLOR_NYX,text);
}

stock NYX2_OrgAccept(playerid)
{
    new org=NYX2_Invite[playerid];if(!NYX_IsValidOrg(org))return SendClientMessage(playerid,COLOR_WARNING,"Nenhum convite valido.");NYX_Player[playerid][NYX_Org]=org;NYX_Player[playerid][NYX_OrgRank]=1;NYX2_Invite[playerid]=-1;NYX_SaveAccount(playerid);return SendClientMessage(playerid,COLOR_SUCCESS,"Voce entrou na organizacao como cargo 1.");
}

stock NYX2_OrgLeave(playerid)
{
    if(!NYX_IsValidOrg(NYX_Player[playerid][NYX_Org]))return SendClientMessage(playerid,COLOR_WARNING,"Voce nao possui organizacao.");if(NYX2_Leader(playerid))return SendClientMessage(playerid,COLOR_WARNING,"O lider nao pode sair sem transferir a lideranca.");NYX_Player[playerid][NYX_Org]=-1;NYX_Player[playerid][NYX_OrgRank]=0;NYX_SaveAccount(playerid);return SendClientMessage(playerid,COLOR_SUCCESS,"Voce saiu da organizacao.");
}

stock NYX2_OrgRank(playerid,target,rank)
{
    if(!NYX2_Leader(playerid))return SendClientMessage(playerid,COLOR_ERROR,"Apenas o lider pode alterar cargos.");if(target==playerid||!IsPlayerConnected(target)||NYX_Player[target][NYX_Org]!=NYX_Player[playerid][NYX_Org])return SendClientMessage(playerid,COLOR_ERROR,"Membro invalido.");if(rank<1||rank>=NYX_Player[playerid][NYX_OrgRank]||rank>NYX_MAX_RANKS)return SendClientMessage(playerid,COLOR_ERROR,"Cargo invalido.");NYX_Player[target][NYX_OrgRank]=rank;NYX_SaveAccount(target);return SendClientMessage(playerid,COLOR_SUCCESS,"Cargo alterado.");
}

stock NYX2_OrgKick(playerid,target)
{
    if(!NYX2_Leader(playerid))return SendClientMessage(playerid,COLOR_ERROR,"Apenas o lider pode remover.");if(target==playerid||!IsPlayerConnected(target)||NYX_Player[target][NYX_Org]!=NYX_Player[playerid][NYX_Org]||NYX_Player[target][NYX_OrgRank]>=NYX_Player[playerid][NYX_OrgRank])return SendClientMessage(playerid,COLOR_ERROR,"Operacao nao permitida.");NYX_Player[target][NYX_Org]=-1;NYX_Player[target][NYX_OrgRank]=0;NYX_SaveAccount(target);return SendClientMessage(playerid,COLOR_SUCCESS,"Membro removido.");
}

stock NYX2_BusinessList(playerid)
{
    new text[160];for(new i=0;i<NYX2_BIZ_COUNT;i++){format(text,sizeof text,"ID %d | %s | $%d | Dono: %s | Caixa: $%d",i+1,NYX2_BizName[i],NYX2_BizPrice[i],NYX2_BizOwner[i][0]?NYX2_BizOwner[i]:"SEM DONO",NYX2_BizCash[i]);SendClientMessage(playerid,COLOR_WHITE,text);}return 1;
}

stock bool:NYX2_BusinessOwner(playerid,id)
{
    if(id<0||id>=NYX2_BIZ_COUNT)return false;new name[MAX_PLAYER_NAME];GetPlayerName(playerid,name,sizeof name);return !strcmp(name,NYX2_BizOwner[id],true);
}

stock NYX2_BusinessBuy(playerid,id)
{
    id--;if(id<0||id>=NYX2_BIZ_COUNT)return SendClientMessage(playerid,COLOR_ERROR,"Negocio invalido.");if(NYX2_BizOwner[id][0])return SendClientMessage(playerid,COLOR_WARNING,"Negocio ja possui dono.");if(GetPlayerMoney(playerid)<NYX2_BizPrice[id])return SendClientMessage(playerid,COLOR_ERROR,"Dinheiro insuficiente.");new name[MAX_PLAYER_NAME];GetPlayerName(playerid,name,sizeof name);format(NYX2_BizOwner[id],MAX_PLAYER_NAME,"%s",name);GivePlayerMoney(playerid,-NYX2_BizPrice[id]);NYX_SyncMoney(playerid);NYX_SaveAccount(playerid);NYX2_SaveBusinesses();return SendClientMessage(playerid,COLOR_SUCCESS,"Negocio comprado e salvo.");
}

stock NYX2_BusinessDeposit(playerid,id,amount)
{
    id--;if(id<0||id>=NYX2_BIZ_COUNT||!NYX2_BusinessOwner(playerid,id))return SendClientMessage(playerid,COLOR_ERROR,"Negocio invalido ou sem permissao.");if(amount<=0||amount>GetPlayerMoney(playerid))return SendClientMessage(playerid,COLOR_ERROR,"Valor invalido.");NYX2_BizCash[id]+=amount;GivePlayerMoney(playerid,-amount);NYX_SyncMoney(playerid);NYX_SaveAccount(playerid);NYX2_SaveBusinesses();return SendClientMessage(playerid,COLOR_SUCCESS,"Valor depositado no caixa.");
}

stock NYX2_BusinessWithdraw(playerid,id,amount)
{
    id--;if(id<0||id>=NYX2_BIZ_COUNT||!NYX2_BusinessOwner(playerid,id))return SendClientMessage(playerid,COLOR_ERROR,"Negocio invalido ou sem permissao.");if(amount<=0||amount>NYX2_BizCash[id])return SendClientMessage(playerid,COLOR_ERROR,"Caixa insuficiente.");NYX2_BizCash[id]-=amount;GivePlayerMoney(playerid,amount);NYX_SyncMoney(playerid);NYX_SaveAccount(playerid);NYX2_SaveBusinesses();return SendClientMessage(playerid,COLOR_SUCCESS,"Valor retirado do caixa.");
}

stock NYX2_Arrest(playerid,target)
{
    if(!NYX_IsPolice(playerid))return SendClientMessage(playerid,COLOR_ERROR,"Acesso restrito a policia.");if(target==INVALID_PLAYER_ID||!IsPlayerConnected(target))return SendClientMessage(playerid,COLOR_ERROR,"Jogador invalido.");new Float:x,Float:y,Float:z;GetPlayerPos(playerid,x,y,z);if(!IsPlayerInRangeOfPoint(target,5.0,x,y,z))return SendClientMessage(playerid,COLOR_WARNING,"Aproxime-se do suspeito.");if(NYX_WantedLevel[target]<=0&&!NYX_PoliceCuffed[target])return SendClientMessage(playerid,COLOR_WARNING,"O suspeito nao esta procurado nem algemado.");NYX_PoliceCuffed[target]=0;TogglePlayerControllable(target,1);SetPlayerWantedLevel(target,0);NYX_WantedLevel[target]=0;NYX2_Jail[target]=NYX2_JAIL_SECONDS;SetPlayerPos(target,1550.0,-1675.0,15.0);SendClientMessage(target,COLOR_WARNING,"Voce foi preso. Use /pena.");return SendClientMessage(playerid,COLOR_SUCCESS,"Prisao efetuada.");
}

public NYX2_ReleaseJail(playerid)
{
    if(!IsPlayerConnected(playerid))return 1;NYX2_Jail[playerid]=0;SetPlayerPos(playerid,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z);TogglePlayerControllable(playerid,1);return SendClientMessage(playerid,COLOR_SUCCESS,"Sua pena terminou. Voce foi liberado.");
}

stock NYX2_MedicalDeath(playerid)
{
    NYX_MedicalDead[playerid]=true;NYX_MedicalInjury[playerid]=100;NYX2_MedicalTimer[playerid]=SetTimerEx("NYX2_MedicalRespawn",NYX2_MEDICAL_SECONDS*1000,false,"i",playerid);return SendClientMessage(playerid,COLOR_WARNING,"Voce esta inconsciente. Aguarde o atendimento medico.");
}

public NYX2_MedicalRespawn(playerid)
{
    if(!IsPlayerConnected(playerid))return 1;NYX2_MedicalTimer[playerid]=NYX2_INVALID_TIMER;NYX_MedicalDead[playerid]=false;NYX_MedicalInjury[playerid]=0;SpawnPlayer(playerid);SetPlayerPos(playerid,NYX_MEDICAL_HOSPITAL_X,NYX_MEDICAL_HOSPITAL_Y,NYX_MEDICAL_HOSPITAL_Z);SetPlayerHealth(playerid,100.0);return SendClientMessage(playerid,COLOR_SUCCESS,"Voce acordou no Hospital NYX.");
}

stock NYX2_SamuCall(playerid)
{
    for(new i=0;i<MAX_PLAYERS;i++)if(IsPlayerConnected(i)&&NYX_Player[i][NYX_Logged]&&NYX_Player[i][NYX_Org]==9)SendClientMessage(i,COLOR_NYX,"[SAMU] Nova chamada de emergencia. Use /samuatender ID.");return SendClientMessage(playerid,COLOR_SUCCESS,"Chamado SAMU enviado.");
}

stock NYX2_SamuAttend(playerid,target)
{
    if(NYX_Player[playerid][NYX_Org]!=9)return SendClientMessage(playerid,COLOR_ERROR,"Acesso restrito ao SAMU.");if(target==INVALID_PLAYER_ID||!IsPlayerConnected(target))return SendClientMessage(playerid,COLOR_ERROR,"Paciente invalido.");new Float:x,Float:y,Float:z;GetPlayerPos(playerid,x,y,z);if(!IsPlayerInRangeOfPoint(target,5.0,x,y,z))return SendClientMessage(playerid,COLOR_WARNING,"Aproxime-se do paciente.");SetPlayerHealth(target,100.0);NYX_MedicalDead[target]=false;NYX_MedicalInjury[target]=0;GivePlayerMoney(playerid,250);NYX_SyncMoney(playerid);return SendClientMessage(playerid,COLOR_SUCCESS,"Atendimento concluido. +$250.");
}

stock NYX2_Commands(playerid,cmdtext[])
{
    if(!strcmp(cmdtext,"/rpajuda",true)){SendClientMessage(playerid,COLOR_NYX,"[NYX RP] /orglista /orgconvidar ID /orgaceitar /orgsair /orgcargo ID CARGO /orgexpulsar ID");SendClientMessage(playerid,COLOR_NYX,"[NYX RP] /negocios /comprarnegocio ID /depositonegocio ID VALOR /saquenegocio ID VALOR");SendClientMessage(playerid,COLOR_NYX,"[NYX RP] /chamarsamu /samuatender ID /pena");return 1;}
    if(!strcmp(cmdtext,"/orglista",true))return NYX2_OrgList(playerid);
    if(strfind(cmdtext,"/orgconvidar ",true)==0)return NYX2_OrgInvitePlayer(playerid,strval(cmdtext[13]));
    if(!strcmp(cmdtext,"/orgaceitar",true))return NYX2_OrgAccept(playerid);
    if(!strcmp(cmdtext,"/orgrecusar",true)){NYX2_Invite[playerid]=-1;return SendClientMessage(playerid,COLOR_WHITE,"Convite recusado.");}
    if(!strcmp(cmdtext,"/orgsair",true))return NYX2_OrgLeave(playerid);
    if(strfind(cmdtext,"/orgcargo ",true)==0){new rest[64],id,rank;format(rest,sizeof rest,"%s",cmdtext[10]);id=strval(rest);new p=strfind(rest," ",true);if(p<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /orgcargo ID CARGO");rank=strval(rest[p+1]);return NYX2_OrgRank(playerid,id,rank);}
    if(strfind(cmdtext,"/orgexpulsar ",true)==0)return NYX2_OrgKick(playerid,strval(cmdtext[13]));
    if(!strcmp(cmdtext,"/negocios",true))return NYX2_BusinessList(playerid);
    if(strfind(cmdtext,"/comprarnegocio ",true)==0)return NYX2_BusinessBuy(playerid,strval(cmdtext[16]));
    if(strfind(cmdtext,"/depositonegocio ",true)==0){new rest[64],id,amount;format(rest,sizeof rest,"%s",cmdtext[17]);id=strval(rest);new p=strfind(rest," ",true);if(p<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /depositonegocio ID VALOR");amount=strval(rest[p+1]);return NYX2_BusinessDeposit(playerid,id,amount);}
    if(strfind(cmdtext,"/saquenegocio ",true)==0){new rest[64],id,amount;format(rest,sizeof rest,"%s",cmdtext[14]);id=strval(rest);new p=strfind(rest," ",true);if(p<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /saquenegocio ID VALOR");amount=strval(rest[p+1]);return NYX2_BusinessWithdraw(playerid,id,amount);}
    if(strfind(cmdtext,"/prender ",true)==0)return NYX2_Arrest(playerid,strval(cmdtext[9]));
    if(!strcmp(cmdtext,"/chamarsamu",true))return NYX2_SamuCall(playerid);
    if(strfind(cmdtext,"/samuatender ",true)==0)return NYX2_SamuAttend(playerid,strval(cmdtext[13]));
    if(!strcmp(cmdtext,"/pena",true)){new text[80];format(text,sizeof text,"Tempo restante de prisao: %d segundos.",NYX2_Jail[playerid]);return SendClientMessage(playerid,COLOR_WHITE,text);}
    return 0;
}
