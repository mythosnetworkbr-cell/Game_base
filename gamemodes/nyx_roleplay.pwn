#include <a_samp>
#include <nyx_config>
#include <nyx_player>
#include <nyx_orgs>

#define D_LOGIN 1000
#define D_REGISTER 1001
#define D_SKINS 1002
#define D_ORGS 1003
#define D_HELP 1004

public OnGameModeInit()
{
    SetGameModeText(NYX_SERVER_NAME);
    ShowPlayerMarkers(1);
    ShowNameTags(1);
    UsePlayerPedAnims();
    SetWorldTime(12);
    SetWeather(10);

    // Skin 1 = mendigo masculino | Skin 2 = mendigo feminino.
    AddPlayerClass(1, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 0.0, 0,0,0,0,0,0);
    AddPlayerClass(2, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 0.0, 0,0,0,0,0,0);

    Create3DTextLabel("{8B5CF6}NYX ROLEPLAY\n{FFFFFF}Centro da Cidade", COLOR_WHITE, NYX_SPAWN_X, NYX_SPAWN_Y, 21.0, 30.0, 0, 1);
    printf("[NYX] %s v%s inicializada. Skins nativas: 0-311.", NYX_SERVER_NAME, NYX_VERSION);
    return 1;
}

public OnPlayerConnect(playerid)
{
    NYX_ResetPlayer(playerid);
    ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD,
        "NYX ROLEPLAY | LOGIN",
        "Digite sua senha para entrar na conta NYX.\n\nSeu personagem inicia com a skin de mendigo correspondente.",
        "ENTRAR", "REGISTRAR");
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    NYX_ResetPlayer(playerid);
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    // Alternancia inicial: classe 0 = mendigo masculino (1), classe 1 = mendigo feminino (2).
    if (classid == 0) NYX_Player[playerid][NYX_Skin] = 1;
    else if (classid == 1) NYX_Player[playerid][NYX_Skin] = 2;

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

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    switch (dialogid)
    {
        case D_LOGIN:
        {
            if (!response)
            {
                ShowPlayerDialog(playerid, D_REGISTER, DIALOG_STYLE_PASSWORD,
                    "NYX ROLEPLAY | REGISTRO",
                    "Crie a senha da sua conta.\n\nMinimo: 4 caracteres.",
                    "CRIAR", "VOLTAR");
                return 1;
            }
            if (strlen(inputtext) < 4)
            {
                ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD,
                    "NYX ROLEPLAY | LOGIN", "Senha muito curta. Digite novamente.",
                    "ENTRAR", "REGISTRAR");
                return 1;
            }
            NYX_Player[playerid][NYX_Logged] = 1;
            SendClientMessage(playerid, COLOR_SUCCESS, "Login realizado. Bem-vindo ao NYX ROLEPLAY!");
            SendClientMessage(playerid, COLOR_NYX, "Seu personagem inicia como mendigo. Use /skin ID para escolher qualquer skin nativa 0-311.");
            return 1;
        }

        case D_REGISTER:
        {
            if (!response)
                return ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD,
                    "NYX ROLEPLAY | LOGIN", "Digite sua senha para entrar.", "ENTRAR", "REGISTRAR");

            if (strlen(inputtext) < 4)
                return ShowPlayerDialog(playerid, D_REGISTER, DIALOG_STYLE_PASSWORD,
                    "NYX ROLEPLAY | REGISTRO", "A senha precisa ter pelo menos 4 caracteres.",
                    "CRIAR", "VOLTAR");

            NYX_Player[playerid][NYX_Logged] = 1;
            NYX_Player[playerid][NYX_Money] = NYX_START_MONEY;
            SendClientMessage(playerid, COLOR_SUCCESS, "Conta criada com sucesso!");
            SendClientMessage(playerid, COLOR_NYX, "Personagem inicial: mendigo. Use /skin ID para escolher qualquer skin nativa 0-311.");
            return 1;
        }

        case D_SKINS:
        {
            if (!response) return 1;
            if (listitem >= 0 && listitem <= 311)
            {
                NYX_Player[playerid][NYX_Skin] = listitem;
                SetPlayerSkin(playerid, listitem);
            }
            return 1;
        }

        case D_ORGS:
        {
            if (!response || !NYX_IsValidOrg(listitem)) return 1;
            new orgName[64], info[256];
            NYX_GetOrgName(listitem, orgName, sizeof orgName);
            format(info, sizeof info,
                "Organizacao: %s\nStatus: %s\n\nSistema de cargos: 1 a %d\n\nA lideranca controla recrutamento, promocoes e atividades.",
                orgName,
                NYX_GetOrgStatus(listitem) ? "ATIVA" : "OFFLINE",
                NYX_MAX_RANKS);
            ShowPlayerDialog(playerid, D_HELP, DIALOG_STYLE_MSGBOX,
                "NYX | ORGANIZACAO", info, "OK", "");
            return 1;
        }
    }
    return 1;
}

stock NYX_ShowSkinDialog(playerid)
{
    ShowPlayerDialog(playerid, D_SKINS, DIALOG_STYLE_INPUT,
        "NYX | SELECIONAR SKIN",
        "Digite o ID da skin nativa do SA-MP.\n\nIDs disponiveis: 0 ate 311.\n\n1 = Mendigo masculino\n2 = Mendigo feminino",
        "USAR", "FECHAR");
    return 1;
}

stock NYX_ShowOrgDialog(playerid)
{
    new list[1024], line[96];
    list[0] = EOS;
    for (new i = 0; i < NYX_ORG_COUNT; i++)
    {
        format(line, sizeof line, "[%s] %s\n",
            NYX_OrgActive[i] ? "ATIVA" : "OFFLINE", NYX_OrgName[i]);
        strcat(list, line, sizeof list);
    }
    ShowPlayerDialog(playerid, D_ORGS, DIALOG_STYLE_LIST,
        "NYX | ORGANIZACOES", list, "VER", "FECHAR");
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!NYX_Player[playerid][NYX_Logged]) return 1;

    if (!strcmp(cmdtext, "/orgs", true)) return NYX_ShowOrgDialog(playerid);

    if (!strcmp(cmdtext, "/org", true))
    {
        new org = NYX_Player[playerid][NYX_Org];
        if (!NYX_IsValidOrg(org)) return SendClientMessage(playerid, COLOR_WHITE, "Voce esta como Civil.");
        new msg[192];
        format(msg, sizeof msg, "NYX | Organizacao: %s | Rank: %d | %s",
            NYX_OrgName[org], NYX_Player[playerid][NYX_OrgRank],
            NYX_OrgActive[org] ? "ATIVA" : "OFFLINE");
        return SendClientMessage(playerid, COLOR_NYX, msg);
    }

    if (!strcmp(cmdtext, "/skin", true)) return NYX_ShowSkinDialog(playerid);

    // Uso: /skin 0 ... /skin 311
    if (!strncmp(cmdtext, "/skin ", 6, true))
    {
        new skinid = strval(cmdtext[6]);
        if (!NYX_IsValidSkin(skinid))
            return SendClientMessage(playerid, COLOR_ERROR, "Skin invalida. Use um ID entre 0 e 311.");
        NYX_Player[playerid][NYX_Skin] = skinid;
        SetPlayerSkin(playerid, skinid);
        new msg[96]; format(msg, sizeof msg, "Skin %d equipada.", skinid);
        return SendClientMessage(playerid, COLOR_SUCCESS, msg);
    }

    if (!strcmp(cmdtext, "/status", true))
    {
        new orgName[64] = "Civil";
        new org = NYX_Player[playerid][NYX_Org];
        if (NYX_IsValidOrg(org)) format(orgName, sizeof orgName, "%s", NYX_OrgName[org]);
        new msg[256];
        format(msg, sizeof msg, "NYX | Dinheiro: $%d | Skin: %d | Organizacao: %s | Rank: %d",
            GetPlayerMoney(playerid), NYX_Player[playerid][NYX_Skin], orgName, NYX_Player[playerid][NYX_OrgRank]);
        return SendClientMessage(playerid, COLOR_WHITE, msg);
    }

    if (!strcmp(cmdtext, "/gps", true))
    {
        SetPlayerCheckpoint(playerid, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 4.0);
        return SendClientMessage(playerid, COLOR_SUCCESS, "GPS marcado: Centro de NYX.");
    }

    if (!strcmp(cmdtext, "/ajuda", true))
    {
        ShowPlayerDialog(playerid, D_HELP, DIALOG_STYLE_MSGBOX, "NYX | CENTRAL DE AJUDA",
            "/orgs - listar organizacoes\n/org - sua organizacao\n/status - status do personagem\n/skin - escolher skin\n/skin ID - equipar skin 0-311\n/gps - marcar centro da cidade\n/ajuda - esta central",
            "FECHAR", "");
        return 1;
    }
    return 0;
}
