#include <a_samp>

#define NYX_NAME "NYX ROLEPLAY"
#define MAX_ORGS 8

enum E_PLAYER { bool:Logged, Skin, Money, Org, Rank, Admin };
new Player[MAX_PLAYERS][E_PLAYER];

new const OrgName[MAX_ORGS][] = {
    "Policia Militar", "Policia Civil", "SAMU", "Mecanicos",
    "Governo", "Jornal NYX", "Ballas", "Families"
};
new bool:OrgActive[MAX_ORGS] = {true,true,true,true,true,true,true,true};

enum { D_LOGIN = 1000, D_REGISTER, D_SKINS, D_ORGS, D_HELP };

public OnGameModeInit()
{
    SetGameModeText("NYX ROLEPLAY");
    ShowPlayerMarkers(1);
    ShowNameTags(1);
    UsePlayerPedAnims();
    SetWorldTime(12);
    SetWeather(10);
    AddPlayerClass(23, 1481.0, -1771.0, 18.8, 0.0, 0,0,0,0,0,0);
    Create3DTextLabel("{8B5CF6}NYX ROLEPLAY\n{FFFFFF}Centro da Cidade", 0xFFFFFFFF, 1481.0, -1771.0, 21.0, 30.0, 0, 1);
    print("[NYX] Gamemode inicializada.");
    return 1;
}

public OnPlayerConnect(playerid)
{
    Player[playerid][Logged] = false;
    Player[playerid][Skin] = 23;
    Player[playerid][Money] = 5000;
    Player[playerid][Org] = -1;
    Player[playerid][Rank] = 0;
    Player[playerid][Admin] = 0;
    ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | LOGIN", "Digite sua senha para entrar.", "ENTRAR", "REGISTRAR");
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    SetPlayerCameraPos(playerid, 1488.0, -1757.0, 24.0);
    SetPlayerCameraLookAt(playerid, 1481.0, -1771.0, 19.0);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    if (!Player[playerid][Logged]) return 1;
    SetPlayerPos(playerid, 1481.0, -1771.0, 18.8);
    SetPlayerSkin(playerid, Player[playerid][Skin]);
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, Player[playerid][Money]);
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    if (dialogid == D_LOGIN)
    {
        if (response)
        {
            if (strlen(inputtext) < 4) return ShowPlayerDialog(playerid,D_LOGIN,DIALOG_STYLE_PASSWORD,"NYX ROLEPLAY | LOGIN","Senha invalida. Digite novamente.","ENTRAR","SAIR");
            Player[playerid][Logged] = true;
            SendClientMessage(playerid, 0x6EE7B7FF, "Login realizado. Bem-vindo ao NYX ROLEPLAY!");
            ShowPlayerDialog(playerid,D_SKINS,DIALOG_STYLE_LIST,"NYX | CRIACAO DO PERSONAGEM","Civil Masculino\nCivil Feminino\nPolicial\nMedico\nMecanico\nGoverno\nGangster\nGangster 2","ESCOLHER","SAIR");
            return 1;
        }
        ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX ROLEPLAY | REGISTRO","Crie a senha da sua conta.","CRIAR","VOLTAR");
        return 1;
    }
    if (dialogid == D_REGISTER)
    {
        if (!response) return ShowPlayerDialog(playerid,D_LOGIN,DIALOG_STYLE_PASSWORD,"NYX ROLEPLAY | LOGIN","Digite sua senha para entrar.","ENTRAR","REGISTRAR");
        if (strlen(inputtext) < 4) return ShowPlayerDialog(playerid,D_REGISTER,DIALOG_STYLE_PASSWORD,"NYX ROLEPLAY | REGISTRO","A senha precisa ter pelo menos 4 caracteres.","CRIAR","VOLTAR");
        Player[playerid][Logged] = true;
        SendClientMessage(playerid,0x6EE7B7FF,"Conta criada com sucesso!");
        ShowPlayerDialog(playerid,D_SKINS,DIALOG_STYLE_LIST,"NYX | CRIACAO DO PERSONAGEM","Civil Masculino\nCivil Feminino\nPolicial\nMedico\nMecanico\nGoverno\nGangster\nGangster 2","ESCOLHER","SAIR");
        return 1;
    }
    if (dialogid == D_SKINS && response)
    {
        new skins[8] = {23,93,280,274,50,147,102,104};
        if (listitem >= 0 && listitem < 8) Player[playerid][Skin] = skins[listitem];
        SetPlayerSkin(playerid,Player[playerid][Skin]);
        SendClientMessage(playerid,0x8B5CF6FF,"Personagem configurado. Use /orgs e /ajuda.");
        SpawnPlayer(playerid);
        return 1;
    }
    if (dialogid == D_ORGS && response)
    {
        new msg[256];
        format(msg,sizeof msg,"Organizacao: %s | Status: %s | Cargos: 1-10",OrgName[listitem],OrgActive[listitem] ? "ATIVA" : "OFFLINE");
        ShowPlayerDialog(playerid,D_HELP,DIALOG_STYLE_MSGBOX,"NYX | ORGANIZACAO",msg,"OK","");
        return 1;
    }
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!Player[playerid][Logged]) return 1;
    if (!strcmp(cmdtext,"/orgs",true))
    {
        new list[1024], line[100]; list[0] = EOS;
        for(new i=0;i<MAX_ORGS;i++) { format(line,sizeof line,"[%s] %s\n",OrgActive[i] ? "ATIVA" : "OFFLINE",OrgName[i]); strcat(list,line); }
        ShowPlayerDialog(playerid,D_ORGS,DIALOG_STYLE_LIST,"NYX | ORGANIZACOES",list,"VER","FECHAR");
        return 1;
    }
    if (!strcmp(cmdtext,"/org",true))
    {
        if(Player[playerid][Org] == -1) return SendClientMessage(playerid,0xFFFFFFFF,"Voce esta como Civil.");
        new msg[160]; format(msg,sizeof msg,"Organizacao: %s | Rank: %d",OrgName[Player[playerid][Org]],Player[playerid][Rank]); SendClientMessage(playerid,0x8B5CF6FF,msg); return 1;
    }
    if (!strcmp(cmdtext,"/status",true))
    {
        new msg[200]; format(msg,sizeof msg,"NYX | Dinheiro: $%d | Skin: %d | Organizacao: %s",GetPlayerMoney(playerid),Player[playerid][Skin],Player[playerid][Org] == -1 ? "Civil" : OrgName[Player[playerid][Org]]); SendClientMessage(playerid,0xFFFFFFFF,msg); return 1;
    }
    if (!strcmp(cmdtext,"/gps",true))
    {
        SetPlayerCheckpoint(playerid,1481.0,-1771.0,18.8,4.0); SendClientMessage(playerid,0x6EE7B7FF,"GPS marcado no centro de NYX."); return 1;
    }
    if (!strcmp(cmdtext,"/ajuda",true))
    {
        ShowPlayerDialog(playerid,D_HELP,DIALOG_STYLE_MSGBOX,"NYX | AJUDA","/orgs - organizacoes\n/org - sua organizacao\n/status - status\n/gps - marcar centro\n/ajuda - comandos\n\nMais sistemas serao adicionados por modulos.","FECHAR",""); return 1;
    }
    return 0;
}
