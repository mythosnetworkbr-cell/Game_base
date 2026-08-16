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
    NYX_ResetJob(playerid); NYX_ResetMarriage(playerid); return 1;
}

public OnPlayerRequestClass(playerid,classid)
{
    if(!NYX_Player[playerid][NYX_Logged]) return 1;
    SetPlayerSkin(playerid,NYX_Player[playerid][NYX_Skin]);
    SetPlayerCameraPos(playerid,1488.0,-1757.0,24.0);
    SetPlayerCameraLookAt(playerid,NYX_SPAWN_X,NYX_SPAWN_Y,19.0); return 1;
}

public OnPlayerSpawn(playerid)
{
    if(!NYX_Player[playerid][NYX_Logged]) return 1;
    NYX_ApplyGraphicsProfile(playerid); NYX_SetupSpawn(playerid);
    if(NYX2_Jail[playerid]>0) SetPlayerPos(playerid,NYX2_JAIL_X,NYX2_JAIL_Y,NYX2_JAIL_Z);
    return 1;
}

public OnPlayerDeath(playerid,killerid,reason)
{
    NYX_MedicalSetInjury(playerid); NYX2_MedicalDeath(playerid); return 1;
}

public OnPlayerTakeDamage(playerid,issuerid,Float:amount,weaponid,bodypart)
{
    NYX_MedicalSetInjury(playerid); return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
    if(NYX_JobRunning[playerid])
    {
        DisablePlayerCheckpoint(playerid); SendClientMessage(playerid,COLOR_NYX,"Destino alcancado. Use /concluir para receber seu pagamento.");
    }
    return 1;
}

public OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
    switch(dialogid)
    {
        case D_LOGIN:
        {
            if(!response) return ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX | REGISTRO","Crie sua senha. Minimo 4 caracteres.","CRIAR","VOLTAR");
            if(strlen(inputtext)<4) return ShowPlayerDialog(playerid,D_LOGIN,DIALOG_STYLE_PASSWORD,"NYX | LOGIN","Senha muito curta.","ENTRAR","REGISTRAR");
            new auth=NYX_LoadAccount(playerid,inputtext);
            if(auth==1){NYX_Player[playerid][NYX_Logged]=1;ResetPlayerMoney(playerid);GivePlayerMoney(playerid,NYX_Player[playerid][NYX_Money]);return SendClientMessage(playerid,COLOR_SUCCESS,"Login realizado. Seus dados foram carregados.");}
            if(auth==-1) return ShowPlayerDialog(playerid,D_LOGIN,DIALOG_STYLE_PASSWORD,"NYX | LOGIN","Senha incorreta.","ENTRAR","REGISTRAR");
            return ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX | REGISTRO","Conta nao pode ser aberta agora. Tente novamente.","CRIAR","VOLTAR");
        }
        case D_REGISTER:
        {
            if(!response)return 1;if(strlen(inputtext)<4)return ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX | REGISTRO","Senha muito curta. Minimo 4 caracteres.","CRIAR","SAIR");
            if(!NYX_CreateAccount(playerid,inputtext))return ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX | REGISTRO","Esta conta ja existe.","CRIAR","SAIR");NYX_Player[playerid][NYX_Logged]=1;return SendClientMessage(playerid,COLOR_SUCCESS,"Conta criada e salva. Bem-vindo ao NYX ROLEPLAY!");
        }
        case D_SKINS:
        {
            if(!response)return 1;new skinid=strval(inputtext);if(!NYX_IsValidSkin(skinid))return SendClientMessage(playerid,COLOR_ERROR,"Skin invalida. Use ID 0-311.");NYX_Player[playerid][NYX_Skin]=skinid;SetPlayerSkin(playerid,skinid);NYX_SaveAccount(playerid);return SendClientMessage(playerid,COLOR_SUCCESS,"Skin equipada e salva.");
        }
        case D_ORGS:
        {
            if(!response||!NYX_IsValidOrg(listitem))return 1;new info[320];format(info,sizeof info,"Organizacao: %s\nStatus: %s\nCargos: 1-%d\n\nUse /orglista para membros e os comandos de gestao.",NYX_OrgName[listitem],NYX_OrgActive[listitem]?"ATIVA":"OFFLINE",NYX_MAX_RANKS);return ShowPlayerDialog(playerid,D_HELP,DIALOG_STYLE_MSGBOX,"NYX | ORGANIZACAO",info,"OK","");
        }
        case D_JOBS:
        {
            if(!response||listitem<1||listitem>=NYX_JOB_COUNT)return 1;NYX_PlayerJob[playerid]=listitem;new msg[128];format(msg,sizeof msg,"Emprego escolhido: %s | $%d por servico. Use /trabalhar.",NYX_JobName[listitem],NYX_JobPay[listitem]);return SendClientMessage(playerid,COLOR_SUCCESS,msg);
        }
        case D_STORE:return 1;
        case D_BANK:return 1;
    }
    return 1;
}

stock NYX_ShowJobs(playerid)
{
    new list[2048],line[128];list[0]=EOS;for(new i=1;i<NYX_JOB_COUNT;i++){format(line,sizeof line,"%d. %s | $%d\n",i,NYX_JobName[i],NYX_JobPay[i]);strcat(list,line,sizeof list);}return ShowPlayerDialog(playerid,D_JOBS,DIALOG_STYLE_LIST,"NYX | CENTRAL DE EMPREGOS",list,"ESCOLHER","FECHAR");
}

stock NYX_StartJob(playerid)
{
    new job=NYX_PlayerJob[playerid];if(job<1||job>=NYX_JOB_COUNT)return SendClientMessage(playerid,COLOR_WARNING,"Escolha um emprego em /empregos.");if(NYX_JobRunning[playerid])return SendClientMessage(playerid,COLOR_WARNING,"Voce ja esta trabalhando.");NYX_JobRunning[playerid]=true;if(NYX_JobVehicle[job]>0){new v=CreateVehicle(NYX_JobVehicle[job],NYX_JobPoint[job][0],NYX_JobPoint[job][1],NYX_JobPoint[job][2],0.0,-1,-1,300);NYX_JobVehicleId[playerid]=v;PutPlayerInVehicle(playerid,v,0);}SetPlayerCheckpoint(playerid,NYX_JobPoint[job][0]+25.0,NYX_JobPoint[job][1]+25.0,NYX_JobPoint[job][2],5.0);return SendClientMessage(playerid,COLOR_SUCCESS,"Trabalho iniciado. Siga o GPS.");
}

stock NYX_ConcludeJob(playerid)
{
    if(!NYX_JobRunning[playerid])return SendClientMessage(playerid,COLOR_WARNING,"Nenhum servico em andamento.");new job=NYX_PlayerJob[playerid],pay=NYX_JobPay[job];GivePlayerMoney(playerid,pay);NYX_SyncMoney(playerid);NYX_SaveAccount(playerid);NYX_JobRunning[playerid]=false;DisablePlayerCheckpoint(playerid);if(NYX_JobVehicleId[playerid]!=INVALID_VEHICLE_ID){RemovePlayerFromVehicle(playerid);DestroyVehicle(NYX_JobVehicleId[playerid]);NYX_JobVehicleId[playerid]=INVALID_VEHICLE_ID;}new msg[96];format(msg,sizeof msg,"Servico concluido! +$%d",pay);return SendClientMessage(playerid,COLOR_SUCCESS,msg);
}

stock NYX_ShowBank(playerid)
{
    new list[256];format(list,sizeof list,"Consultar saldo\nDepositar dinheiro\nSacar dinheiro");return ShowPlayerDialog(playerid,D_BANK,DIALOG_STYLE_LIST,"NYX | BANCO",list,"SELECIONAR","FECHAR");
}

public OnPlayerCommandText(playerid,cmdtext[])
{
    if(!NYX_Player[playerid][NYX_Logged])return 1;
    if(NYX_HandleCompletionCommand(playerid,cmdtext))return 1;
    if(NYX_MedicalCommands(playerid,cmdtext))return 1;
    if(NYX2_Commands(playerid,cmdtext))return 1;
    if(!strcmp(cmdtext,"/orgs",true)){new list[2048],line[96];list[0]=EOS;for(new i=0;i<NYX_ORG_COUNT;i++){format(line,sizeof line,"[%s] %s\n",NYX_OrgActive[i]?"ATIVA":"OFFLINE",NYX_OrgName[i]);strcat(list,line,sizeof list);}return ShowPlayerDialog(playerid,D_ORGS,DIALOG_STYLE_LIST,"NYX | ORGANIZACOES",list,"VER","FECHAR");}
    if(!strcmp(cmdtext,"/empregos",true))return NYX_ShowJobs(playerid);
    if(!strcmp(cmdtext,"/trabalhar",true))return NYX_StartJob(playerid);
    if(!strcmp(cmdtext,"/concluir",true))return NYX_ConcludeJob(playerid);
    if(!strcmp(cmdtext,"/banco",true))return NYX_ShowBank(playerid);
    if(strfind(cmdtext,"/depositar ",true)==0){new amount=strval(cmdtext[11]);if(!NYX_Deposit(playerid,amount))return SendClientMessage(playerid,COLOR_ERROR,"Deposito invalido ou saldo insuficiente.");return SendClientMessage(playerid,COLOR_SUCCESS,"Deposito realizado.");}
    if(strfind(cmdtext,"/sacar ",true)==0){new amount=strval(cmdtext[7]);if(!NYX_Withdraw(playerid,amount))return SendClientMessage(playerid,COLOR_ERROR,"Saque invalido ou saldo insuficiente.");return SendClientMessage(playerid,COLOR_SUCCESS,"Saque realizado.");}
    if(!strcmp(cmdtext,"/conta",true)){new name[MAX_PLAYER_NAME],msg[192];GetPlayerName(playerid,name,sizeof name);format(msg,sizeof msg,"NYX | Conta: %s | Dinheiro: $%d | Banco: $%d | NCoins: %d | Admin: %d",name,GetPlayerMoney(playerid),NYX_Bank[playerid],NYX_Player[playerid][NYX_NCoins],NYX_Player[playerid][NYX_Admin]);return SendClientMessage(playerid,COLOR_WHITE,msg);}
    if(!strcmp(cmdtext,"/salvar",true)){NYX_SyncMoney(playerid);NYX_SaveAccount(playerid);return SendClientMessage(playerid,COLOR_SUCCESS,"Conta salva.");}
    if(!strcmp(cmdtext,"/lanchonetes",true))return NYX_ShowFood(playerid);
    if(!strcmp(cmdtext,"/imoveis",true))return NYX_ShowProperties(playerid);
    if(!strcmp(cmdtext,"/leilao",true))return NYX_ShowAuction(playerid);
    if(!strcmp(cmdtext,"/familias",true))return NYX_ShowFamilies(playerid);
    if(!strcmp(cmdtext,"/skin",true))return ShowPlayerDialog(playerid,D_SKINS,DIALOG_STYLE_INPUT,"NYX | SKINS","Digite o ID da skin (0-311).","USAR","FECHAR");
    if(!strcmp(cmdtext,"/status",true)){new job[48],org[64];NYX_GetJobName(NYX_PlayerJob[playerid],job,sizeof job);format(org,sizeof org,"Civil");if(NYX_IsValidOrg(NYX_Player[playerid][NYX_Org]))format(org,sizeof org,"%s",NYX_OrgName[NYX_Player[playerid][NYX_Org]]);new msg[256];format(msg,sizeof msg,"NYX | Dinheiro: $%d | Banco: $%d | NCoins: %d | Skin: %d | Emprego: %s | Org: %s",GetPlayerMoney(playerid),NYX_Bank[playerid],NYX_Player[playerid][NYX_NCoins],NYX_Player[playerid][NYX_Skin],job,org);return SendClientMessage(playerid,COLOR_WHITE,msg);}
    if(!strcmp(cmdtext,"/ncoins",true)){new msg[96];format(msg,sizeof msg,"Seu saldo: %d NCoins.",NYX_Player[playerid][NYX_NCoins]);return SendClientMessage(playerid,COLOR_NYX,msg);}
    if(!strcmp(cmdtext,"/mundo",true))return NYX_ShowWorldInfo(playerid);
    if(!strcmp(cmdtext,"/casamento",true))return NYX_MarriageExplain(playerid);
    if(strfind(cmdtext,"/casar ",true)==0){new target=strval(cmdtext[7]);if(!NYX_MarriageCanPropose(playerid,target))return SendClientMessage(playerid,COLOR_ERROR,"Nao e possivel casar com este jogador.");NYX_PendingMarriage[target]=playerid;return NYX_MarriageSendProposal(playerid,target);}
    if(!strcmp(cmdtext,"/aceitarcasamento",true)){new proposer=NYX_PendingMarriage[playerid];if(proposer==INVALID_PLAYER_ID||!NYX_MarriageCanPropose(proposer,playerid))return SendClientMessage(playerid,COLOR_WARNING,"Nenhuma proposta valida.");NYX_MarryPlayers(proposer,playerid);NYX_PendingMarriage[playerid]=INVALID_PLAYER_ID;NYX_PendingMarriage[proposer]=INVALID_PLAYER_ID;SendClientMessage(proposer,COLOR_SUCCESS,"Casamento realizado!");return SendClientMessage(playerid,COLOR_SUCCESS,"Casamento realizado!");}
    if(!strcmp(cmdtext,"/divorcio",true)){if(!NYX_Divorce(playerid))return SendClientMessage(playerid,COLOR_WARNING,"Voce nao esta casado.");return SendClientMessage(playerid,COLOR_SUCCESS,"Divorcio realizado.");}
    if(!strcmp(cmdtext,"/loja",true))return ShowPlayerDialog(playerid,D_STORE,DIALOG_STYLE_MSGBOX,"NYX | NCOINS","Moeda premium NYX. Compras reais devem passar pelo backend oficial.","OK","");
    if(!strcmp(cmdtext,"/gps",true)){SetPlayerCheckpoint(playerid,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z,4.0);return SendClientMessage(playerid,COLOR_SUCCESS,"GPS marcado: Centro / Prefeitura NYX.");}
    if(!strcmp(cmdtext,"/ajuda",true))return ShowPlayerDialog(playerid,D_HELP,DIALOG_STYLE_MSGBOX,"NYX | AJUDA","/necessidades /comer /beber /passaporte /negocios\n/garagem /guardarveiculo ID /assaltar /pesca /pescar\n/banco /depositar /sacar /conta /salvar\n/empregos /trabalhar /concluir /orgs /orglista\n/negocios /comprarnegocio ID /chamarsamu /samuatender ID\n/hospital /estado /curar /pena /rpajuda\n/ncoins /loja /casar ID /aceitarcasamento /divorcio\n/mundo /gps","FECHAR","");
    return 0;
}
