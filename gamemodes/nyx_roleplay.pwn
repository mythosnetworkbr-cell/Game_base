#include <a_samp>
#include <nyx_config>
#include <nyx_player>
#include <nyx_orgs>
#include <nyx_jobs>
#include <nyx_marriage>
#include <nyx_ncoins>
#include <nyx_world>

#define D_LOGIN 1000
#define D_REGISTER 1001
#define D_SKINS 1002
#define D_ORGS 1003
#define D_HELP 1004
#define D_JOBS 1005
#define D_STORE 1006

new NYX_PendingMarriage[MAX_PLAYERS];

public OnGameModeInit()
{
    SetGameModeText(NYX_SERVER_NAME);
    ShowPlayerMarkers(1); ShowNameTags(1); UsePlayerPedAnims();
    SetWorldTime(12); SetWeather(10);
    AddPlayerClass(1,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z,0.0,0,0,0,0,0,0);
    AddPlayerClass(2,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z,0.0,0,0,0,0,0,0);
    Create3DTextLabel("{8B5CF6}NYX ROLEPLAY\n{FFFFFF}Prefeitura / Centro",COLOR_WHITE,NYX_SPAWN_X,NYX_SPAWN_Y,21.0,30.0,0,1);
    Create3DTextLabel("{8B5CF6}HOSPITAL CENTRAL NYX",COLOR_WHITE,1520.0,-1675.0,15.0,30.0,0,1);
    Create3DTextLabel("{8B5CF6}DELEGACIA CENTRAL NYX",COLOR_WHITE,1550.0,-1600.0,15.0,30.0,0,1);
    Create3DTextLabel("{8B5CF6}CASSINO NYX",COLOR_WHITE,2200.0,-1670.0,16.0,30.0,0,1);
    Create3DTextLabel("{8B5CF6}SEX SHOP NYX",COLOR_WHITE,1350.0,-1740.0,15.0,30.0,0,1);
    print("[NYX] GameMode inicializada: 15 empregos, 24 organizacoes, NCoins, casamento e servicos.");
    return 1;
}

public OnPlayerConnect(playerid)
{
    NYX_ResetPlayer(playerid); NYX_ResetJob(playerid); NYX_ResetMarriage(playerid);
    NYX_PendingMarriage[playerid]=INVALID_PLAYER_ID;
    ShowPlayerDialog(playerid,D_LOGIN,DIALOG_STYLE_PASSWORD,"NYX ROLEPLAY | LOGIN","Bem-vindo ao NYX ROLEPLAY.\n\nDigite sua senha para entrar.\nSeu personagem inicia como mendigo.","ENTRAR","REGISTRAR");
    return 1;
}

public OnPlayerDisconnect(playerid,reason)
{
    if(NYX_JobVehicleId[playerid]!=INVALID_VEHICLE_ID) DestroyVehicle(NYX_JobVehicleId[playerid]);
    NYX_ResetJob(playerid); NYX_ResetMarriage(playerid); return 1;
}

public OnPlayerRequestClass(playerid,classid)
{
    NYX_Player[playerid][NYX_Skin]=(classid==0)?1:2;
    SetPlayerSkin(playerid,NYX_Player[playerid][NYX_Skin]);
    SetPlayerCameraPos(playerid,1488.0,-1757.0,24.0);
    SetPlayerCameraLookAt(playerid,NYX_SPAWN_X,NYX_SPAWN_Y,19.0); return 1;
}

public OnPlayerSpawn(playerid)
{
    if(!NYX_Player[playerid][NYX_Logged]) return 1;
    NYX_SetupSpawn(playerid); return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
    if(NYX_JobRunning[playerid])
    {
        DisablePlayerCheckpoint(playerid);
        SendClientMessage(playerid,COLOR_NYX,"Destino alcancado. Use /concluir para receber seu pagamento.");
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
            NYX_Player[playerid][NYX_Logged]=1;
            SendClientMessage(playerid,COLOR_SUCCESS,"Login realizado. Bem-vindo ao NYX ROLEPLAY!");
            return 1;
        }
        case D_REGISTER:
        {
            if(!response) return ShowPlayerDialog(playerid,D_LOGIN,DIALOG_STYLE_PASSWORD,"NYX | LOGIN","Digite sua senha.","ENTRAR","REGISTRAR");
            if(strlen(inputtext)<4) return ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX | REGISTRO","Senha muito curta.","CRIAR","VOLTAR");
            NYX_Player[playerid][NYX_Logged]=1; NYX_Player[playerid][NYX_Money]=NYX_START_MONEY;
            SendClientMessage(playerid,COLOR_SUCCESS,"Conta criada! Bem-vindo ao NYX ROLEPLAY!"); return 1;
        }
        case D_SKINS:
        {
            if(!response) return 1;
            new skinid=strval(inputtext);
            if(!NYX_IsValidSkin(skinid)) return SendClientMessage(playerid,COLOR_ERROR,"Skin invalida. Use ID 0-311.");
            NYX_Player[playerid][NYX_Skin]=skinid; SetPlayerSkin(playerid,skinid);
            return SendClientMessage(playerid,COLOR_SUCCESS,"Skin equipada.");
        }
        case D_ORGS:
        {
            if(!response||!NYX_IsValidOrg(listitem)) return 1;
            new info[320];
            format(info,sizeof info,"Organizacao: %s\nStatus: %s\nCargos: 1-%d\n\nA mesma organizacao atua em todo o mapa.\nLideranca, recrutamento e promocoes sao controlados pelo sistema.",NYX_OrgName[listitem],NYX_OrgActive[listitem]?"ATIVA":"OFFLINE",NYX_MAX_RANKS);
            return ShowPlayerDialog(playerid,D_HELP,DIALOG_STYLE_MSGBOX,"NYX | ORGANIZACAO",info,"OK","");
        }
        case D_JOBS:
        {
            if(!response||listitem<1||listitem>=NYX_JOB_COUNT) return 1;
            NYX_PlayerJob[playerid]=listitem;
            new msg[128]; format(msg,sizeof msg,"Emprego escolhido: %s | Salario por servico: $%d. Use /trabalhar.",NYX_JobName[listitem],NYX_JobPay[listitem]);
            return SendClientMessage(playerid,COLOR_SUCCESS,msg);
        }
        case D_STORE:
        {
            if(!response) return 1;
            return SendClientMessage(playerid,COLOR_NYX,"NCoins sao moeda premium. Compras com dinheiro real devem passar pelo backend oficial.");
        }
    }
    return 1;
}

stock NYX_ShowJobs(playerid)
{
    new list[2048],line[128]; list[0]=EOS;
    for(new i=1;i<NYX_JOB_COUNT;i++){format(line,sizeof line,"%d. %s | $%d\n",i,NYX_JobName[i],NYX_JobPay[i]);strcat(list,line,sizeof list);}
    return ShowPlayerDialog(playerid,D_JOBS,DIALOG_STYLE_LIST,"NYX | CENTRAL DE EMPREGOS",list,"ESCOLHER","FECHAR");
}

stock NYX_StartJob(playerid)
{
    new job=NYX_PlayerJob[playerid];
    if(job<1||job>=NYX_JOB_COUNT) return SendClientMessage(playerid,COLOR_WARNING,"Escolha um emprego em /empregos.");
    if(NYX_JobRunning[playerid]) return SendClientMessage(playerid,COLOR_WARNING,"Voce ja esta trabalhando.");
    NYX_JobRunning[playerid]=true;
    if(NYX_JobVehicle[job]>0)
    {
        new v=CreateVehicle(NYX_JobVehicle[job],NYX_JobPoint[job][0],NYX_JobPoint[job][1],NYX_JobPoint[job][2],0.0,-1,-1,300);
        NYX_JobVehicleId[playerid]=v; PutPlayerInVehicle(playerid,v,0);
    }
    SetPlayerCheckpoint(playerid,NYX_JobPoint[job][0]+25.0,NYX_JobPoint[job][1]+25.0,NYX_JobPoint[job][2],5.0);
    new msg[128]; format(msg,sizeof msg,"Trabalho iniciado: %s. Siga o GPS.",NYX_JobName[job]);
    return SendClientMessage(playerid,COLOR_SUCCESS,msg);
}

stock NYX_ConcludeJob(playerid)
{
    if(!NYX_JobRunning[playerid]) return SendClientMessage(playerid,COLOR_WARNING,"Nenhum servico em andamento.");
    new job=NYX_PlayerJob[playerid],pay=NYX_JobPay[job];
    GivePlayerMoney(playerid,pay); NYX_Player[playerid][NYX_Money]=GetPlayerMoney(playerid);
    NYX_JobRunning[playerid]=false; DisablePlayerCheckpoint(playerid);
    if(NYX_JobVehicleId[playerid]!=INVALID_VEHICLE_ID){RemovePlayerFromVehicle(playerid);DestroyVehicle(NYX_JobVehicleId[playerid]);NYX_JobVehicleId[playerid]=INVALID_VEHICLE_ID;}
    new msg[96]; format(msg,sizeof msg,"Servico concluido! +$%d",pay); return SendClientMessage(playerid,COLOR_SUCCESS,msg);
}

public OnPlayerCommandText(playerid,cmdtext[])
{
    if(!NYX_Player[playerid][NYX_Logged]) return 1;
    if(!strcmp(cmdtext,"/orgs",true))
    {
        new list[2048],line[96];list[0]=EOS;
        for(new i=0;i<NYX_ORG_COUNT;i++){format(line,sizeof line,"[%s] %s\n",NYX_OrgActive[i]?"ATIVA":"OFFLINE",NYX_OrgName[i]);strcat(list,line,sizeof list);}
        return ShowPlayerDialog(playerid,D_ORGS,DIALOG_STYLE_LIST,"NYX | ORGANIZACOES",list,"VER","FECHAR");
    }
    if(!strcmp(cmdtext,"/empregos",true)) return NYX_ShowJobs(playerid);
    if(!strcmp(cmdtext,"/trabalhar",true)) return NYX_StartJob(playerid);
    if(!strcmp(cmdtext,"/concluir",true)) return NYX_ConcludeJob(playerid);
    if(!strcmp(cmdtext,"/skin",true)) return ShowPlayerDialog(playerid,D_SKINS,DIALOG_STYLE_INPUT,"NYX | SKINS","Digite o ID da skin (0-311).\n1 = mendigo masculino\n2 = mendigo feminino","USAR","FECHAR");
    if(!strcmp(cmdtext,"/status",true))
    {
        new job[48],org[64];NYX_GetJobName(NYX_PlayerJob[playerid],job,sizeof job);format(org,sizeof org,"Civil");
        if(NYX_IsValidOrg(NYX_Player[playerid][NYX_Org]))format(org,sizeof org,"%s",NYX_OrgName[NYX_Player[playerid][NYX_Org]]);
        new msg[256];format(msg,sizeof msg,"NYX | Dinheiro: $%d | NCoins: %d | Skin: %d | Emprego: %s | Org: %s",GetPlayerMoney(playerid),NYX_Player[playerid][NYX_NCoins],NYX_Player[playerid][NYX_Skin],job,org);
        return SendClientMessage(playerid,COLOR_WHITE,msg);
    }
    if(!strcmp(cmdtext,"/ncoins",true)){new msg[96];format(msg,sizeof msg,"Seu saldo: %d NCoins.",NYX_Player[playerid][NYX_NCoins]);return SendClientMessage(playerid,COLOR_NYX,msg);}
    if(!strcmp(cmdtext,"/mundo",true)) return NYX_ShowWorldInfo(playerid);
    if(!strcmp(cmdtext,"/casamento",true)) return NYX_MarriageExplain(playerid);
    if(!strncmp(cmdtext,"/casar ",7,true))
    {
        new target=strval(cmdtext[7]);
        if(!NYX_MarriageCanPropose(playerid,target))return SendClientMessage(playerid,COLOR_ERROR,"Nao e possivel casar com este jogador.");
        NYX_PendingMarriage[target]=playerid; return NYX_MarriageSendProposal(playerid,target);
    }
    if(!strcmp(cmdtext,"/aceitarcasamento",true))
    {
        new proposer=NYX_PendingMarriage[playerid];
        if(proposer==INVALID_PLAYER_ID||!NYX_MarriageCanPropose(proposer,playerid))return SendClientMessage(playerid,COLOR_WARNING,"Nenhuma proposta valida.");
        NYX_MarryPlayers(proposer,playerid);NYX_PendingMarriage[playerid]=INVALID_PLAYER_ID;NYX_PendingMarriage[proposer]=INVALID_PLAYER_ID;
        SendClientMessage(proposer,COLOR_SUCCESS,"Casamento realizado! Felicidades ao casal.");return SendClientMessage(playerid,COLOR_SUCCESS,"Casamento realizado! Felicidades ao casal.");
    }
    if(!strcmp(cmdtext,"/divorcio",true)){if(!NYX_Divorce(playerid))return SendClientMessage(playerid,COLOR_WARNING,"Voce nao esta casado.");return SendClientMessage(playerid,COLOR_SUCCESS,"Divorcio realizado.");}
    if(!strcmp(cmdtext,"/loja",true))return ShowPlayerDialog(playerid,D_STORE,DIALOG_STYLE_MSGBOX,"NYX | NCOINS","Moeda premium NYX.\nSkins premium podem usar NCoins.\nCompras reais devem ser processadas pelo backend oficial.","OK","");
    if(!strcmp(cmdtext,"/gps",true)){SetPlayerCheckpoint(playerid,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z,4.0);return SendClientMessage(playerid,COLOR_SUCCESS,"GPS marcado: Centro / Prefeitura NYX.");}
    if(!strcmp(cmdtext,"/ajuda",true))return ShowPlayerDialog(playerid,D_HELP,DIALOG_STYLE_MSGBOX,"NYX | AJUDA","/empregos /trabalhar /concluir\n/orgs /status /skin\n/ncoins /loja\n/casar ID /aceitarcasamento /divorcio\n/mundo /gps","FECHAR","");
    return 0;
}
