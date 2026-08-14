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
    ShowPlayerMarkers(1);
    ShowNameTags(1);
    UsePlayerPedAnims();
    SetWorldTime(12);
    SetWeather(10);

    AddPlayerClass(NYX_DEFAULT_SKIN_MALE, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 0.0, 0,0,0,0,0,0);
    AddPlayerClass(NYX_DEFAULT_SKIN_FEMALE, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 0.0, 0,0,0,0,0,0);

    Create3DTextLabel("{8B5CF6}NYX ROLEPLAY\n{FFFFFF}Prefeitura / Centro", COLOR_WHITE, NYX_SPAWN_X, NYX_SPAWN_Y, 21.0, 30.0, 0, 1);
    Create3DTextLabel("{8B5CF6}HOSPITAL CENTRAL NYX", COLOR_WHITE, 1520.0, -1675.0, 15.0, 30.0, 0, 1);
    Create3DTextLabel("{8B5CF6}DELEGACIA CENTRAL NYX", COLOR_WHITE, 1550.0, -1600.0, 15.0, 30.0, 0, 1);
    Create3DTextLabel("{8B5CF6}CASSINO NYX", COLOR_WHITE, 2200.0, -1670.0, 16.0, 30.0, 0, 1);
    Create3DTextLabel("{8B5CF6}SEX SHOP NYX", COLOR_WHITE, 1350.0, -1740.0, 15.0, 30.0, 0, 1);

    print("========================================");
    printf("[NYX] %s v%s carregada.", NYX_SERVER_NAME, NYX_VERSION);
    print("[NYX] 15 empregos + organizacoes + NCoins + casamento.");
    print("[NYX] Idioma nativo: Portugues (Brasil).");
    print("========================================");
    return 1;
}

public OnPlayerConnect(playerid)
{
    NYX_ResetPlayer(playerid);
    NYX_ResetJob(playerid);
    NYX_ResetMarriage(playerid);
    NYX_PendingMarriage[playerid] = INVALID_PLAYER_ID;

    ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD,
        "NYX ROLEPLAY | LOGIN",
        "Bem-vindo ao NYX ROLEPLAY.\n\nDigite sua senha para entrar.\nSeu personagem inicia com a skin de mendigo.",
        "ENTRAR", "REGISTRAR");
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (NYX_JobVehicleId[playerid] != INVALID_VEHICLE_ID)
        DestroyVehicle(NYX_JobVehicleId[playerid]);
    NYX_ResetJob(playerid);
    NYX_ResetMarriage(playerid);
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    NYX_Player[playerid][NYX_Skin] = (classid == 0) ? NYX_DEFAULT_SKIN_MALE : NYX_DEFAULT_SKIN_FEMALE;
    SetPlayerSkin(playerid, NYX_Player[playerid][NYX_Skin]);
    SetPlayerCameraPos(playerid, 1488.0, -1757.0, 24.0);
    SetPlayerCameraLookAt(playerid, NYX_SPAWN_X, NYX_SPAWN_Y, 19.0);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    if (!NYX_Player[playerid][NYX_Logged]) return 1;
    NYX_SetupSpawn(playerid);
    return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
    if (!NYX_JobRunning[playerid]) return 1;
    DisablePlayerCheckpoint(playerid);
    SendClientMessage(playerid, COLOR_NYX, "Entrega concluida. Use /concluir para receber o pagamento.");
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch (dialogid)
    {
        case D_LOGIN:
        {
            if (!response)
                return ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX | REGISTRO","Crie sua senha. Minimo 4 caracteres.","CRIAR","VOLTAR");
            if (strlen(inputtext) < 4)
                return ShowPlayerDialog(playerid,D_LOGIN,DIALOG_STYLE_PASSWORD,"NYX | LOGIN","Senha muito curta.","ENTRAR","REGISTRAR");
            NYX_Player[playerid][NYX_Logged] = 1;
            NYX_Player[playerid][NYX_Money] = NYX_START_MONEY;
            SendClientMessage(playerid,COLOR_SUCCESS,"Login realizado. Bem-vindo a NYX!");
            SendClientMessage(playerid,COLOR_NYX,"Voce inicia como mendigo. Use /skin ID para trocar sua skin.");
            return 1;
        }
        case D_REGISTER:
        {
            if (!response) return ShowPlayerDialog(playerid,D_LOGIN,DIALOG_STYLE_PASSWORD,"NYX | LOGIN","Digite sua senha.","ENTRAR","REGISTRAR");
            if (strlen(inputtext) < 4) return ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX | REGISTRO","Senha muito curta.","CRIAR","VOLTAR");
            NYX_Player[playerid][NYX_Logged] = 1;
            NYX_Player[playerid][NYX_Money] = NYX_START_MONEY;
            SendClientMessage(playerid,COLOR_SUCCESS,"Conta criada com sucesso!");
            return 1;
        }
        case D_SKINS:
        {
            if (!response) return 1;
            new skinid = strval(inputtext);
            if (!NYX_IsValidSkin(skinid)) return SendClientMessage(playerid,COLOR_ERROR,"Skin invalida. Use 0 ate 311.");
            NYX_Player[playerid][NYX_Skin] = skinid;
            SetPlayerSkin(playerid,skinid);
            return SendClientMessage(playerid,COLOR_SUCCESS,"Skin equipada com sucesso.");
        }
        case D_ORGS:
        {
            if (!response || !NYX_IsValidOrg(listitem)) return 1;
            new info[256];
            format(info,sizeof info,"Organizacao: %s\nStatus: %s\nCargos: 1-%d\n\nUma unica estrutura de organizacao atua em todo o mapa.",
                NYX_OrgName[listitem], NYX_OrgActive[listitem] ? "ATIVA" : "OFFLINE", NYX_MAX_RANKS);
            ShowPlayerDialog(playerid,D_HELP,DIALOG_STYLE_MSGBOX,"NYX | ORGANIZACAO",info,"OK","");
            return 1;
        }
        case D_JOBS:
        {
            if (!response) return 1;
            if (listitem < 0 || listitem >= NYX_JOB_COUNT) return 1;
            NYX_PlayerJob[playerid] = listitem;
            new msg[128];
            format(msg,sizeof msg,"Trabalho selecionado: %s. Use /trabalhar no ponto do emprego.",NYX_JobName[listitem]);
            SendClientMessage(playerid,COLOR_SUCCESS,msg);
            return 1;
        }
        case D_STORE:
        {
            if (!response) return 1;
            SendClientMessage(playerid,COLOR_NYX,"Loja NCoins: skins premium ficam vinculadas ao saldo premium do personagem.");
            return 1;
        }
    }
    return 1;
}

stock NYX_ShowJobs(playerid)
{
    new list[2048], line[128]; list[0] = EOS;
    for (new i=0;i<NYX_JOB_COUNT;i++)
    {
        format(line,sizeof line,"%d. %s | $%d por entrega\n",i,NYX_JobName[i],NYX_JobPay[i]);
        strcat(list,line,sizeof list);
    }
    return ShowPlayerDialog(playerid,D_JOBS,DIALOG_STYLE_LIST,"NYX | CENTRAL DE EMPREGOS",list,"ESCOLHER","FECHAR");
}

stock NYX_StartJob(playerid)
{
    new job = NYX_PlayerJob[playerid];
    if (job <= 0 || job >= NYX_JOB_COUNT) return SendClientMessage(playerid,COLOR_WARNING,"Escolha um emprego em /empregos primeiro.");
    if (NYX_JobRunning[playerid]) return SendClientMessage(playerid,COLOR_WARNING,"Voce ja esta trabalhando.");

    NYX_JobRunning[playerid] = true;
    if (NYX_JobVehicle[job] > 0)
    {
        new vehicle = CreateVehicle(NYX_JobVehicle[job],NYX_JobPoint[job][0],NYX_JobPoint[job][1],NYX_JobPoint[job][2],0.0,-1,-1,300);
        NYX_JobVehicleId[playerid] = vehicle;
        PutPlayerInVehicle(playerid,vehicle,0);
    }
    SetPlayerCheckpoint(playerid,NYX_JobPoint[job][0]+25.0,NYX_JobPoint[job][1]+25.0,NYX_JobPoint[job][2],5.0);
    new msg[160]; format(msg,sizeof msg,"Trabalho iniciado: %s. Va ate o destino marcado no GPS.",NYX_JobName[job]);
    SendClientMessage(playerid,COLOR_SUCCESS,msg);
    return 1;
}

stock NYX_ConcludeJob(playerid)
{
    if (!NYX_JobRunning[playerid]) return SendClientMessage(playerid,COLOR_WARNING,"Nenhum trabalho em andamento.");
    new job = NYX_PlayerJob[playerid];
    new pay = NYX_JobPay[job];
    GivePlayerMoney(playerid,pay);
    NYX_Player[playerid][NYX_Money] = GetPlayerMoney(playerid);
    NYX_JobRunning[playerid] = false;
    DisablePlayerCheckpoint(playerid);
    if (NYX_JobVehicleId[playerid] != INVALID_VEHICLE_ID)
    {
        RemovePlayerFromVehicle(playerid);
        DestroyVehicle(NYX_JobVehicleId[playerid]);
        NYX_JobVehicleId[playerid] = INVALID_VEHICLE_ID;
    }
    new msg[128]; format(msg,sizeof msg,"Servico concluido! Voce recebeu $%d.",pay);
    return SendClientMessage(playerid,COLOR_SUCCESS,msg);
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!NYX_Player[playerid][NYX_Logged]) return 1;

    if (!strcmp(cmdtext,"/orgs",true))
    {
        new list[1024],line[96]; list[0]=EOS;
        for(new i=0;i<NYX_ORG_COUNT;i++){format(line,sizeof line,"[%s] %s\n",NYX_OrgActive[i]?"ATIVA":"OFFLINE",NYX_OrgName[i]);strcat(list,line,sizeof list);}
        return ShowPlayerDialog(playerid,D_ORGS,DIALOG_STYLE_LIST,"NYX | ORGANIZACOES",list,"VER","FECHAR");
    }
    if (!strcmp(cmdtext,"/empregos",true)) return NYX_ShowJobs(playerid);
    if (!strcmp(cmdtext,"/trabalhar",true)) return NYX_StartJob(playerid);
    if (!strcmp(cmdtext,"/concluir",true)) return NYX_ConcludeJob(playerid);
    if (!strcmp(cmdtext,"/skin",true)) return ShowPlayerDialog(playerid,D_SKINS,DIALOG_STYLE_INPUT,"NYX | SKINS","Digite o ID da skin nativa (0-311).\n1 = mendigo masculino\n2 = mendigo feminino","USAR","FECHAR");
    if (!strcmp(cmdtext,"/status",true))
    {
        new job[48]; NYX_GetJobName(NYX_PlayerJob[playerid],job,sizeof job);
        new org[48]="Civil"; if(NYX_IsValidOrg(NYX_Player[playerid][NYX_Org])) format(org,sizeof org,"%s",NYX_OrgName[NYX_Player[playerid][NYX_Org]]);
        new msg[256]; format(msg,sizeof msg,"NYX | Dinheiro: $%d | Skin: %d | Emprego: %s | Org: %s",GetPlayerMoney(playerid),NYX_Player[playerid][NYX_Skin],job,org);
        return SendClientMessage(playerid,COLOR_WHITE,msg);
    }
    if (!strcmp(cmdtext,"/mundo",true)) return NYX_ShowWorldInfo(playerid);
    if (!strcmp(cmdtext,"/casamento",true)) return NYX_MarriageExplain(playerid);
    if (!strcmp(cmdtext,"/loja",true)) return ShowPlayerDialog(playerid,D_STORE,DIALOG_STYLE_MSGBOX,"NYX | NCOINS","Loja premium NYX\n\nNCoins sao uma moeda premium separada do dinheiro do RP.\nSkins premium podem ser adquiridas com NCoins.\nCompras reais devem ser processadas pelo backend oficial.","OK","");
    if (!strcmp(cmdtext,"/gps",true)){SetPlayerCheckpoint(playerid,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z,4.0);return SendClientMessage(playerid,COLOR_SUCCESS,"GPS marcado: Prefeitura / Centro NYX.");}
    if (!strcmp(cmdtext,"/ajuda",true))
    {
        return ShowPlayerDialog(playerid,D_HELP,DIALOG_STYLE_MSGBOX,"NYX | AJUDA","/empregos - escolher trabalho\n/trabalhar - iniciar servico\n/concluir - receber pagamento\n/orgs - organizacoes\n/status - personagem\n/skin - trocar skin\n/loja - NCoins\n/casamento - regras de casamento\n/mundo - cidades e servicos\n/gps - centro da cidade","FECHAR","");
    }
    return 0;
}
