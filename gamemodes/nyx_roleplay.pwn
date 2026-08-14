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

    AddPlayerClass(23, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 0.0, 0,0,0,0,0,0);
    Create3DTextLabel("{8B5CF6}NYX ROLEPLAY\n{FFFFFF}Centro da Cidade", COLOR_WHITE, NYX_SPAWN_X, NYX_SPAWN_Y, 21.0, 30.0, 0, 1);

    printf("[NYX] %s v%s inicializada.", NYX_SERVER_NAME, NYX_VERSION);
    return 1;
}

public OnPlayerConnect(playerid)
{
    NYX_ResetPlayer(playerid);
    ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD,
        "NYX ROLEPLAY | LOGIN",
        "Digite sua senha para entrar na conta NYX.",
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
                    "NYX ROLEPLAY | LOGIN",
                    "Senha muito curta. Digite novamente.",
                    "ENTRAR", "REGISTRAR");
                return 1;
            }

            NYX_Player[playerid][NYX_Logged] = 1;
            SendClientMessage(playerid, COLOR_SUCCESS, "Login realizado. Bem-vindo ao NYX ROLEPLAY!");
            return NYX_ShowSkinDialog(playerid);
        }

        case D_REGISTER:
        {
            if (!response)
            {
                return ShowPlayerDialog(playerid, D_LOGIN, DIALOG_STYLE_PASSWORD,
                    "NYX ROLEPLAY | LOGIN", "Digite sua senha para entrar.",
                    "ENTRAR", "REGISTRAR");
            }

            if (strlen(inputtext) < 4)
            {
                return ShowPlayerDialog(playerid, D_REGISTER, DIALOG_STYLE_PASSWORD,
                    "NYX ROLEPLAY | REGISTRO",
                    "A senha precisa ter pelo menos 4 caracteres.",
                    "CRIAR", "VOLTAR");
            }

            NYX_Player[playerid][NYX_Logged] = 1;
            NYX_Player[playerid][NYX_Money] = NYX_START_MONEY;
            SendClientMessage(playerid, COLOR_SUCCESS, "Conta criada com sucesso!");
            return NYX_ShowSkinDialog(playerid);
        }

        case D_SKINS:
        {
            if (!response) return 1;

            new skins[8] = {23, 93, 280, 274, 50, 147, 102, 104};
            if (listitem >= 0 && listitem < 8)
                NYX_Player[playerid][NYX_Skin] = skins[listitem];

            SendClientMessage(playerid, COLOR_NYX, "Personagem configurado. Bem-vindo à cidade.");
            SpawnPlayer(playerid);
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
    ShowPlayerDialog(playerid, D_SKINS, DIALOG_STYLE_LIST,
        "NYX | CRIACAO DO PERSONAGEM",
        "Civil Masculino\nCivil Feminino\nPolicial\nMedico\nMecanico\nGoverno\nGangster\nGangster 2",
        "ESCOLHER", "SAIR");
    return 1;
}

stock NYX_ShowOrgDialog(playerid)
{
    new list[1024], line[96];
    list[0] = EOS;

    for (new i = 0; i < NYX_ORG_COUNT; i++)
    {
        format(line, sizeof line, "[%s] %s\n",
            NYX_OrgActive[i] ? "ATIVA" : "OFFLINE",
            NYX_OrgName[i]);
        strcat(list, line, sizeof list);
    }

    ShowPlayerDialog(playerid, D_ORGS, DIALOG_STYLE_LIST,
        "NYX | ORGANIZACOES", list, "VER", "FECHAR");
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!NYX_Player[playerid][NYX_Logged]) return 1;

    if (!strcmp(cmdtext, "/orgs", true))
        return NYX_ShowOrgDialog(playerid);

    if (!strcmp(cmdtext, "/org", true))
    {
        new org = NYX_Player[playerid][NYX_Org];
        if (!NYX_IsValidOrg(org))
            return SendClientMessage(playerid, COLOR_WHITE, "Voce esta como Civil.");

        new msg[192];
        format(msg, sizeof msg, "NYX | Organizacao: %s | Rank: %d | %s",
            NYX_OrgName[org],
            NYX_Player[playerid][NYX_OrgRank],
            NYX_OrgActive[org] ? "ATIVA" : "OFFLINE");
        return SendClientMessage(playerid, COLOR_NYX, msg);
    }

    if (!strcmp(cmdtext, "/status", true))
    {
        new orgName[64] = "Civil";
        new org = NYX_Player[playerid][NYX_Org];
        if (NYX_IsValidOrg(org)) format(orgName, sizeof orgName, "%s", NYX_OrgName[org]);

        new msg[256];
        format(msg, sizeof msg,
            "NYX | Dinheiro: $%d | Skin: %d | Organizacao: %s | Rank: %d",
            GetPlayerMoney(playerid),
            NYX_Player[playerid][NYX_Skin],
            orgName,
            NYX_Player[playerid][NYX_OrgRank]);
        return SendClientMessage(playerid, COLOR_WHITE, msg);
    }

    if (!strcmp(cmdtext, "/gps", true))
    {
        SetPlayerCheckpoint(playerid, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 4.0);
        return SendClientMessage(playerid, COLOR_SUCCESS, "GPS marcado: Centro de NYX.");
    }

    if (!strcmp(cmdtext, "/ajuda", true))
    {
        ShowPlayerDialog(playerid, D_HELP, DIALOG_STYLE_MSGBOX,
            "NYX | CENTRAL DE AJUDA",
            "/orgs - listar organizacoes\n/org - sua organizacao\n/status - status do personagem\n/gps - marcar centro da cidade\n/ajuda - esta central",
            "FECHAR", "");
        return 1;
    }

    return 0;
}
