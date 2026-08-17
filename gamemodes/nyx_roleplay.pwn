#include <a_samp>
#include <nyx_config>
#include <nyx_player>
#include <nyx_accounts>
#include <nyx_orgs>
#include <nyx_jobs>
#include <nyx_marriage>
#include <nyx_ncoins>
#include <nyx_world>
#include <nyx_properties>
#include <nyx_graphics>
#include <nyx_mobile>

#define D_AUTH_PASSWORD 2000
#define D_REGISTER_CONFIG 2001
#define D_FIGHT_MODE 2002
#define D_AGE 2003
#define D_CITY 2004
#define D_DISCORD 2005
#define D_EMAIL 2006
#define D_SKINS 2007
#define D_ORGS 2008
#define D_HELP 2009
#define D_JOBS 2010
#define D_STORE 2011

#define AUTH_COLOR 0x8B5CF6FF
#define AUTH_WHITE 0xFFFFFFFF
#define AUTH_RED 0xFF4D6DFF
#define AUTH_GREEN 0x6EE7B7FF

new NYX_PendingMarriage[MAX_PLAYERS];
new bool:NYX_ProfileExists[MAX_PLAYERS];
new bool:NYX_AuthRegister[MAX_PLAYERS];
new bool:NYX_AuthDialogOpen[MAX_PLAYERS];
new bool:NYX_AuthPasswordOK[MAX_PLAYERS];
new NYX_AuthPassword[MAX_PLAYERS][65];
new NYX_ProfileAge[MAX_PLAYERS];
new NYX_ProfileCity[MAX_PLAYERS];
new NYX_ProfileFight[MAX_PLAYERS];
new NYX_ProfileDiscord[MAX_PLAYERS][96];
new NYX_ProfileEmail[MAX_PLAYERS][160];

new PlayerText:NYX_TD_Back[MAX_PLAYERS];
new PlayerText:NYX_TD_Title[MAX_PLAYERS];
new PlayerText:NYX_TD_Username[MAX_PLAYERS];
new PlayerText:NYX_TD_Password[MAX_PLAYERS];
new PlayerText:NYX_TD_Action[MAX_PLAYERS];
new PlayerText:NYX_TD_Exit[MAX_PLAYERS];
new bool:NYX_AuthDrawsCreated[MAX_PLAYERS];

forward NYX_AutoSave();

stock NYX_ProfileInit()
{
    if (!NYX_DBReady) return 0;
    db_query(NYX_DB, "CREATE TABLE IF NOT EXISTS profile (name TEXT PRIMARY KEY, age INTEGER DEFAULT 18, city INTEGER DEFAULT 0, fightmode INTEGER DEFAULT 0, discord TEXT DEFAULT '', email TEXT DEFAULT '')");
    return 1;
}

stock NYX_ProfileLoad(playerid)
{
    if (!NYX_DBReady) return 0;
    new name[MAX_PLAYER_NAME + 1], esc[64], query[256];
    GetPlayerName(playerid, name, sizeof name);
    NYX_SQLQuote(name, esc, sizeof esc);
    format(query, sizeof query, "SELECT age,city,fightmode,discord,email FROM profile WHERE name='%s' LIMIT 1", esc);
    new DBResult:r = db_query(NYX_DB, query);
    NYX_ProfileExists[playerid] = false;
    if (r == DBResult:0) return 0;
    if (db_num_rows(r) > 0)
    {
        NYX_ProfileAge[playerid] = db_get_field_assoc_int(r, "age");
        NYX_ProfileCity[playerid] = db_get_field_assoc_int(r, "city");
        NYX_ProfileFight[playerid] = db_get_field_assoc_int(r, "fightmode");
        db_get_field_assoc(r, "discord", NYX_ProfileDiscord[playerid], sizeof NYX_ProfileDiscord[]);
        db_get_field_assoc(r, "email", NYX_ProfileEmail[playerid], sizeof NYX_ProfileEmail[]);
        NYX_ProfileExists[playerid] = true;
    }
    db_free_result(r);
    return 1;
}

stock NYX_ProfileSave(playerid)
{
    if (!NYX_DBReady || !NYX_AccountExists[playerid]) return 0;
    new name[MAX_PLAYER_NAME + 1], escName[64], escDiscord[192], escEmail[320], query[768];
    GetPlayerName(playerid, name, sizeof name);
    NYX_SQLQuote(name, escName, sizeof escName);
    NYX_SQLQuote(NYX_ProfileDiscord[playerid], escDiscord, sizeof escDiscord);
    NYX_SQLQuote(NYX_ProfileEmail[playerid], escEmail, sizeof escEmail);
    format(query, sizeof query, "INSERT OR REPLACE INTO profile (name,age,city,fightmode,discord,email) VALUES ('%s',%d,%d,%d,'%s','%s')", escName, NYX_ProfileAge[playerid], NYX_ProfileCity[playerid], NYX_ProfileFight[playerid], escDiscord, escEmail);
    new DBResult:r = db_query(NYX_DB, query);
    if (r != DBResult:0) db_free_result(r);
    return r != DBResult:0;
}

stock NYX_ResetAuth(playerid)
{
    NYX_AuthRegister[playerid] = false;
    NYX_AuthDialogOpen[playerid] = false;
    NYX_AuthPasswordOK[playerid] = false;
    NYX_AuthPassword[playerid][0] = EOS;
    NYX_ProfileAge[playerid] = 18;
    NYX_ProfileCity[playerid] = 0;
    NYX_ProfileFight[playerid] = 0;
    NYX_ProfileDiscord[playerid][0] = EOS;
    NYX_ProfileEmail[playerid][0] = EOS;
    return 1;
}

stock NYX_CreateAuthDraws(playerid)
{
    if (NYX_AuthDrawsCreated[playerid]) return 1;
    NYX_TD_Back[playerid] = CreatePlayerTextDraw(playerid, 0.0, 0.0, "_");
    PlayerTextDrawLetterSize(playerid, NYX_TD_Back[playerid], 0.0, 45.0);
    PlayerTextDrawTextSize(playerid, NYX_TD_Back[playerid], 640.0, 0.0);
    PlayerTextDrawUseBox(playerid, NYX_TD_Back[playerid], 1);
    PlayerTextDrawBoxColor(playerid, NYX_TD_Back[playerid], 0x101018EE);

    NYX_TD_Title[playerid] = CreatePlayerTextDraw(playerid, 320.0, 85.0, "NYX ROLEPLAY");
    PlayerTextDrawAlignment(playerid, NYX_TD_Title[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NYX_TD_Title[playerid], 0.55, 2.0);
    PlayerTextDrawColor(playerid, NYX_TD_Title[playerid], AUTH_COLOR);

    NYX_TD_Username[playerid] = CreatePlayerTextDraw(playerid, 320.0, 165.0, "USERNAME: Player");
    PlayerTextDrawAlignment(playerid, NYX_TD_Username[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NYX_TD_Username[playerid], 0.30, 1.3);
    PlayerTextDrawColor(playerid, NYX_TD_Username[playerid], AUTH_WHITE);

    NYX_TD_Password[playerid] = CreatePlayerTextDraw(playerid, 320.0, 205.0, "SENHA: clique para definir");
    PlayerTextDrawAlignment(playerid, NYX_TD_Password[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NYX_TD_Password[playerid], 0.30, 1.3);
    PlayerTextDrawColor(playerid, NYX_TD_Password[playerid], AUTH_WHITE);
    PlayerTextDrawSetSelectable(playerid, NYX_TD_Password[playerid], 1);
    PlayerTextDrawTextSize(playerid, NYX_TD_Password[playerid], 430.0, 220.0);

    NYX_TD_Action[playerid] = CreatePlayerTextDraw(playerid, 270.0, 285.0, "REGISTRAR");
    PlayerTextDrawAlignment(playerid, NYX_TD_Action[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NYX_TD_Action[playerid], 0.32, 1.4);
    PlayerTextDrawColor(playerid, NYX_TD_Action[playerid], AUTH_GREEN);
    PlayerTextDrawSetSelectable(playerid, NYX_TD_Action[playerid], 1);
    PlayerTextDrawTextSize(playerid, NYX_TD_Action[playerid], 330.0, 300.0);

    NYX_TD_Exit[playerid] = CreatePlayerTextDraw(playerid, 370.0, 285.0, "SAIR");
    PlayerTextDrawAlignment(playerid, NYX_TD_Exit[playerid], 2);
    PlayerTextDrawLetterSize(playerid, NYX_TD_Exit[playerid], 0.32, 1.4);
    PlayerTextDrawColor(playerid, NYX_TD_Exit[playerid], AUTH_RED);
    PlayerTextDrawSetSelectable(playerid, NYX_TD_Exit[playerid], 1);
    PlayerTextDrawTextSize(playerid, NYX_TD_Exit[playerid], 400.0, 300.0);
    NYX_AuthDrawsCreated[playerid] = true;
    return 1;
}

stock NYX_UpdateAuthDraws(playerid)
{
    new name[MAX_PLAYER_NAME + 1], text[96], stars[65];
    GetPlayerName(playerid, name, sizeof name);
    format(text, sizeof text, "USERNAME: %s", name);
    PlayerTextDrawSetString(playerid, NYX_TD_Username[playerid], text);
    if (strlen(NYX_AuthPassword[playerid]) > 0)
    {
        for (new i = 0; i < strlen(NYX_AuthPassword[playerid]) && i < sizeof stars - 1; i++) stars[i] = '*';
        stars[strlen(NYX_AuthPassword[playerid])] = EOS;
        format(text, sizeof text, "SENHA: %s", stars);
    }
    else format(text, sizeof text, "SENHA: clique para definir");
    PlayerTextDrawSetString(playerid, NYX_TD_Password[playerid], text);
    PlayerTextDrawSetString(playerid, NYX_TD_Action[playerid], NYX_AuthRegister[playerid] ? "REGISTRAR" : "ENTRAR");
    return 1;
}

stock NYX_ShowAuthScreen(playerid)
{
    NYX_CreateAuthDraws(playerid);
    NYX_UpdateAuthDraws(playerid);
    PlayerTextDrawShow(playerid, NYX_TD_Back[playerid]);
    PlayerTextDrawShow(playerid, NYX_TD_Title[playerid]);
    PlayerTextDrawShow(playerid, NYX_TD_Username[playerid]);
    PlayerTextDrawShow(playerid, NYX_TD_Password[playerid]);
    PlayerTextDrawShow(playerid, NYX_TD_Action[playerid]);
    PlayerTextDrawShow(playerid, NYX_TD_Exit[playerid]);
    SelectTextDraw(playerid, AUTH_COLOR);
    return 1;
}

stock NYX_HideAuthScreen(playerid)
{
    if (!NYX_AuthDrawsCreated[playerid]) return 1;
    PlayerTextDrawHide(playerid, NYX_TD_Back[playerid]);
    PlayerTextDrawHide(playerid, NYX_TD_Title[playerid]);
    PlayerTextDrawHide(playerid, NYX_TD_Username[playerid]);
    PlayerTextDrawHide(playerid, NYX_TD_Password[playerid]);
    PlayerTextDrawHide(playerid, NYX_TD_Action[playerid]);
    PlayerTextDrawHide(playerid, NYX_TD_Exit[playerid]);
    return 1;
}

stock NYX_DestroyAuthDraws(playerid)
{
    if (!NYX_AuthDrawsCreated[playerid]) return 1;
    PlayerTextDrawDestroy(playerid, NYX_TD_Back[playerid]);
    PlayerTextDrawDestroy(playerid, NYX_TD_Title[playerid]);
    PlayerTextDrawDestroy(playerid, NYX_TD_Username[playerid]);
    PlayerTextDrawDestroy(playerid, NYX_TD_Password[playerid]);
    PlayerTextDrawDestroy(playerid, NYX_TD_Action[playerid]);
    PlayerTextDrawDestroy(playerid, NYX_TD_Exit[playerid]);
    NYX_AuthDrawsCreated[playerid] = false;
    return 1;
}

stock NYX_ShowRegisterConfig(playerid)
{
    new fight[32], age[32], city[32], discord[96], email[160], list[512];
    format(fight, sizeof fight, "%s", NYX_ProfileFight[playerid] == 1 ? "Boxing" : "Normal");
    if (NYX_ProfileAge[playerid] < 13 || NYX_ProfileAge[playerid] > 100) format(age, sizeof age, "Nao informado");
    else format(age, sizeof age, "%d anos", NYX_ProfileAge[playerid]);
    if (NYX_ProfileCity[playerid] == 0) format(city, sizeof city, "Los Santos");
    else if (NYX_ProfileCity[playerid] == 1) format(city, sizeof city, "Las Venturas");
    else if (NYX_ProfileCity[playerid] == 2) format(city, sizeof city, "San Fierro");
    else format(city, sizeof city, "Nao selecionada");
    if (strlen(NYX_ProfileDiscord[playerid])) format(discord, sizeof discord, "%s", NYX_ProfileDiscord[playerid]);
    else format(discord, sizeof discord, "Opcional - recomendado");
    if (strlen(NYX_ProfileEmail[playerid])) format(email, sizeof email, "%s", NYX_ProfileEmail[playerid]);
    else format(email, sizeof email, "Opcional - recomendado");
    format(list, sizeof list, "1: Modo de luta - %s\n2: Idade - %s\n3: Cidade onde quer nascer - %s\n4: Discord - %s\n5: E-mail - %s\n6: Terminar\n7: Sair", fight, age, city, discord, email);
    NYX_AuthDialogOpen[playerid] = true;
    return ShowPlayerDialog(playerid, D_REGISTER_CONFIG, DIALOG_STYLE_LIST, "NYX ROLEPLAY | REGISTRO", list, "SELECIONAR", "SAIR");
}

stock NYX_FinishRegistration(playerid)
{
    if (!NYX_AuthPasswordOK[playerid] || strlen(NYX_AuthPassword[playerid]) < 6)
        return SendClientMessage(playerid, AUTH_RED, "Defina uma senha valida com no minimo 6 caracteres.");
    if (NYX_ProfileAge[playerid] < 13 || NYX_ProfileAge[playerid] > 100)
        return SendClientMessage(playerid, AUTH_RED, "A idade e obrigatoria e deve estar entre 13 e 100.");
    if (NYX_ProfileCity[playerid] < 0 || NYX_ProfileCity[playerid] > 2)
        return SendClientMessage(playerid, AUTH_RED, "Escolha uma cidade de nascimento.");
    if (!NYX_CreateAccount(playerid, NYX_AuthPassword[playerid]))
        return SendClientMessage(playerid, AUTH_RED, "Nao foi possivel criar a conta.");
    NYX_ProfileSave(playerid);
    NYX_Player[playerid][NYX_Logged] = 1;
    NYX_AuthRegister[playerid] = false;
    NYX_HideAuthScreen(playerid);
    SendClientMessage(playerid, AUTH_GREEN, "CONTA CRIADA OFICIALMENTE. Bem-vindo ao NYX ROLEPLAY!");
    SpawnPlayer(playerid);
    return 1;
}

stock NYX_SpawnByCity(playerid)
{
    switch (NYX_ProfileCity[playerid])
    {
        case 0: SetPlayerPos(playerid, 1481.0, -1771.0, 18.8);
        case 1: SetPlayerPos(playerid, 1690.0, 1448.0, 10.8);
        case 2: SetPlayerPos(playerid, -1985.0, 138.0, 27.7);
        default: SetPlayerPos(playerid, 1481.0, -1771.0, 18.8);
    }
    SetPlayerFacingAngle(playerid, 0.0);
    SetCameraBehindPlayer(playerid);
    SetPlayerSkin(playerid, NYX_Player[playerid][NYX_Skin]);
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid, NYX_Player[playerid][NYX_Money]);
    return 1;
}

public OnGameModeInit()
{
    SetGameModeText(NYX_SERVER_NAME);
    ShowPlayerMarkers(1); ShowNameTags(1); UsePlayerPedAnims();
    SetWorldTime(12); SetWeather(10);
    AddPlayerClass(NYX_DEFAULT_SKIN_MALE, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 0.0, 0,0,0,0,0,0);
    AddPlayerClass(NYX_DEFAULT_SKIN_FEMALE, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 0.0, 0,0,0,0,0,0);
    NYX_AccountsInit();
    NYX_ProfileInit();
    SetTimer("NYX_AutoSave", 120000, true);
    Create3DTextLabel("{8B5CF6}NYX ROLEPLAY\n{FFFFFF}Prefeitura / Centro", COLOR_WHITE, NYX_SPAWN_X, NYX_SPAWN_Y, 21.0, 30.0, 0, 1);
    Create3DTextLabel("{8B5CF6}HOSPITAL CENTRAL NYX", COLOR_WHITE, 1520.0, -1675.0, 15.0, 30.0, 0, 1);
    Create3DTextLabel("{8B5CF6}DELEGACIA CENTRAL NYX", COLOR_WHITE, 1550.0, -1600.0, 15.0, 30.0, 0, 1);
    print("[NYX] GameMode completa consolidada: autenticacao + perfil + empregos + orgs + NCoins + casamento + mundo + propriedades + mobile.");
    return 1;
}

public OnGameModeExit()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i) && NYX_Player[i][NYX_Logged])
        {
            NYX_Player[i][NYX_Money] = GetPlayerMoney(i);
            NYX_SaveAccount(i);
            NYX_ProfileSave(i);
        }
        NYX_DestroyAuthDraws(i);
    }
    NYX_AccountsExit();
    return 1;
}

public OnPlayerConnect(playerid)
{
    NYX_ResetPlayer(playerid);
    NYX_ResetJob(playerid);
    NYX_ResetMarriage(playerid);
    NYX_ResetAuth(playerid);
    NYX_PendingMarriage[playerid] = INVALID_PLAYER_ID;
    NYX_ApplyGraphicsProfile(playerid);
    NYX_LoadAccount(playerid);
    NYX_ProfileLoad(playerid);
    NYX_AuthRegister[playerid] = !NYX_AccountExists[playerid];
    NYX_ShowAuthScreen(playerid);
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    if (NYX_AccountExists[playerid])
    {
        NYX_Player[playerid][NYX_Money] = GetPlayerMoney(playerid);
        NYX_SaveAccount(playerid);
        NYX_ProfileSave(playerid);
    }
    if (NYX_JobVehicleId[playerid] != INVALID_VEHICLE_ID) DestroyVehicle(NYX_JobVehicleId[playerid]);
    NYX_ResetJob(playerid);
    NYX_ResetMarriage(playerid);
    NYX_DestroyAuthDraws(playerid);
    return 1;
}

public OnPlayerRequestClass(playerid, classid)
{
    if (NYX_Player[playerid][NYX_Logged]) return 1;
    SetPlayerSkin(playerid, classid == 0 ? NYX_DEFAULT_SKIN_MALE : NYX_DEFAULT_SKIN_FEMALE);
    SetPlayerCameraPos(playerid, 1488.0, -1757.0, 24.0);
    SetPlayerCameraLookAt(playerid, NYX_SPAWN_X, NYX_SPAWN_Y, 19.0);
    return 1;
}

public OnPlayerSpawn(playerid)
{
    if (!NYX_Player[playerid][NYX_Logged]) return 1;
    NYX_ApplyGraphicsProfile(playerid);
    NYX_SpawnByCity(playerid);
    return 1;
}

public OnPlayerClickPlayerTextDraw(playerid, PlayerText:playertextid)
{
    if (NYX_AuthDialogOpen[playerid] || !NYX_AuthDrawsCreated[playerid]) return 1;
    if (playertextid == NYX_TD_Exit[playerid]) return Kick(playerid);
    if (playertextid == NYX_TD_Password[playerid])
    {
        NYX_AuthDialogOpen[playerid] = true;
        return ShowPlayerDialog(playerid, D_AUTH_PASSWORD, DIALOG_STYLE_PASSWORD,
            NYX_AuthRegister[playerid] ? "NYX ROLEPLAY | SENHA" : "NYX ROLEPLAY | LOGIN",
            NYX_AuthRegister[playerid] ? "Digite uma senha para sua conta.\nMinimo: 6 caracteres." : "Digite a senha da sua conta NYX.",
            NYX_AuthRegister[playerid] ? "CONTINUAR" : "ENTRAR", "FECHAR");
    }
    if (playertextid == NYX_TD_Action[playerid])
    {
        if (NYX_AuthRegister[playerid])
        {
            if (!NYX_AuthPasswordOK[playerid]) return SendClientMessage(playerid, AUTH_RED, "Defina a senha no campo SENHA antes de REGISTRAR.");
            NYX_HideAuthScreen(playerid);
            return NYX_ShowRegisterConfig(playerid);
        }
        NYX_AuthDialogOpen[playerid] = true;
        return ShowPlayerDialog(playerid, D_AUTH_PASSWORD, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | LOGIN", "Digite a senha da sua conta NYX.", "ENTRAR", "FECHAR");
    }
    return 1;
}

public OnDialogResponse(playerid, dialogid, response, listitem, inputtext[])
{
    NYX_AuthDialogOpen[playerid] = false;
    switch (dialogid)
    {
        case D_AUTH_PASSWORD:
        {
            if (!response) return NYX_ShowAuthScreen(playerid);
            if (NYX_AuthRegister[playerid])
            {
                if (strlen(inputtext) < 6)
                {
                    NYX_AuthDialogOpen[playerid] = true;
                    return ShowPlayerDialog(playerid, D_AUTH_PASSWORD, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | SENHA", "Senha invalida. Minimo de 6 caracteres.", "CONTINUAR", "FECHAR");
                }
                format(NYX_AuthPassword[playerid], sizeof NYX_AuthPassword[], "%s", inputtext);
                NYX_AuthPasswordOK[playerid] = true;
                NYX_ShowAuthScreen(playerid);
                return SendClientMessage(playerid, AUTH_GREEN, "Senha definida. Clique em REGISTRAR para continuar.");
            }
            if (!NYX_AccountExists[playerid])
            {
                NYX_AuthRegister[playerid] = true;
                NYX_ShowAuthScreen(playerid);
                return SendClientMessage(playerid, AUTH_GREEN, "Conta nova detectada. Defina a senha para registrar.");
            }
            if (!NYX_CheckPassword(playerid, inputtext))
            {
                NYX_AuthDialogOpen[playerid] = true;
                return ShowPlayerDialog(playerid, D_AUTH_PASSWORD, DIALOG_STYLE_PASSWORD, "NYX ROLEPLAY | LOGIN", "Senha incorreta.", "ENTRAR", "FECHAR");
            }
            NYX_Player[playerid][NYX_Logged] = 1;
            NYX_HideAuthScreen(playerid);
            SendClientMessage(playerid, AUTH_GREEN, "Login realizado. Bem-vindo ao NYX ROLEPLAY!");
            SpawnPlayer(playerid);
            return 1;
        }
        case D_REGISTER_CONFIG:
        {
            if (!response) return Kick(playerid);
            switch (listitem)
            {
                case 0:
                {
                    NYX_AuthDialogOpen[playerid] = true;
                    return ShowPlayerDialog(playerid, D_FIGHT_MODE, DIALOG_STYLE_LIST, "NYX | MODO DE LUTA", "Normal\nBoxing", "SELECIONAR", "VOLTAR");
                }
                case 1:
                {
                    NYX_AuthDialogOpen[playerid] = true;
                    return ShowPlayerDialog(playerid, D_AGE, DIALOG_STYLE_INPUT, "NYX | IDADE", "Digite a idade.\nPermitido: 13 a 100 anos.", "SALVAR", "VOLTAR");
                }
                case 2:
                {
                    NYX_AuthDialogOpen[playerid] = true;
                    return ShowPlayerDialog(playerid, D_CITY, DIALOG_STYLE_LIST, "NYX | CIDADE", "Los Santos\nLas Venturas\nSan Fierro", "SELECIONAR", "VOLTAR");
                }
                case 3:
                {
                    NYX_AuthDialogOpen[playerid] = true;
                    return ShowPlayerDialog(playerid, D_DISCORD, DIALOG_STYLE_INPUT, "NYX | DISCORD (OPCIONAL)", "Opcional. Recomendamos informar para proteger e recuperar a conta.", "SALVAR", "VOLTAR");
                }
                case 4:
                {
                    NYX_AuthDialogOpen[playerid] = true;
                    return ShowPlayerDialog(playerid, D_EMAIL, DIALOG_STYLE_INPUT, "NYX | E-MAIL (OPCIONAL)", "Opcional. Recomendamos informar para proteger e recuperar a conta.", "SALVAR", "VOLTAR");
                }
                case 5: return NYX_FinishRegistration(playerid);
                case 6: return Kick(playerid);
            }
            return 1;
        }
        case D_FIGHT_MODE:
        {
            if (!response) return NYX_ShowRegisterConfig(playerid);
            NYX_ProfileFight[playerid] = listitem == 1 ? 1 : 0;
            return NYX_ShowRegisterConfig(playerid);
        }
        case D_AGE:
        {
            if (!response) return NYX_ShowRegisterConfig(playerid);
            new age = strval(inputtext);
            if (age < 13 || age > 100)
            {
                NYX_AuthDialogOpen[playerid] = true;
                return ShowPlayerDialog(playerid, D_AGE, DIALOG_STYLE_INPUT, "NYX | IDADE", "Idade invalida. Digite entre 13 e 100.", "SALVAR", "VOLTAR");
            }
            NYX_ProfileAge[playerid] = age;
            return NYX_ShowRegisterConfig(playerid);
        }
        case D_CITY:
        {
            if (!response) return NYX_ShowRegisterConfig(playerid);
            NYX_ProfileCity[playerid] = listitem;
            return NYX_ShowRegisterConfig(playerid);
        }
        case D_DISCORD:
        {
            if (response) format(NYX_ProfileDiscord[playerid], sizeof NYX_ProfileDiscord[], "%s", inputtext);
            return NYX_ShowRegisterConfig(playerid);
        }
        case D_EMAIL:
        {
            if (response) format(NYX_ProfileEmail[playerid], sizeof NYX_ProfileEmail[], "%s", inputtext);
            return NYX_ShowRegisterConfig(playerid);
        }
        case D_SKINS:
        {
            if (!response) return 1;
            new skinid = strval(inputtext);
            if (!NYX_IsValidSkin(skinid)) return SendClientMessage(playerid, AUTH_RED, "Skin invalida. Use 0-311.");
            NYX_Player[playerid][NYX_Skin] = skinid;
            SetPlayerSkin(playerid, skinid);
            NYX_SaveAccount(playerid);
            return SendClientMessage(playerid, AUTH_GREEN, "Skin equipada.");
        }
        case D_JOBS:
        {
            if (!response || listitem < 1 || listitem >= NYX_JOB_COUNT) return 1;
            NYX_PlayerJob[playerid] = listitem;
            new msg[128];
            format(msg, sizeof msg, "Emprego: %s | pagamento: $%d. Use /trabalhar.", NYX_JobName[listitem], NYX_JobPay[listitem]);
            return SendClientMessage(playerid, AUTH_GREEN, msg);
        }
        case D_ORGS:
        {
            if (!response || !NYX_IsValidOrg(listitem)) return 1;
            new info[320];
            format(info, sizeof info, "Organizacao: %s\nStatus: %s\nCargos: 1-%d", NYX_OrgName[listitem], NYX_OrgActive[listitem] ? "ATIVA" : "OFFLINE", NYX_MAX_RANKS);
            return ShowPlayerDialog(playerid, D_HELP, DIALOG_STYLE_MSGBOX, "NYX | ORGANIZACAO", info, "OK", "");
        }
    }
    return 1;
}

stock NYX_ShowJobs(playerid)
{
    new list[2048], line[128]; list[0] = EOS;
    for (new i = 1; i < NYX_JOB_COUNT; i++)
    {
        format(line, sizeof line, "%d. %s | $%d\n", i, NYX_JobName[i], NYX_JobPay[i]);
        strcat(list, line, sizeof list);
    }
    return ShowPlayerDialog(playerid, D_JOBS, DIALOG_STYLE_LIST, "NYX | EMPREGOS", list, "ESCOLHER", "FECHAR");
}

stock NYX_StartJob(playerid)
{
    new job = NYX_PlayerJob[playerid];
    if (job < 1 || job >= NYX_JOB_COUNT) return SendClientMessage(playerid, AUTH_RED, "Escolha um emprego em /empregos.");
    if (NYX_JobRunning[playerid]) return SendClientMessage(playerid, AUTH_RED, "Voce ja esta trabalhando.");
    NYX_JobRunning[playerid] = true;
    if (NYX_JobVehicle[job] > 0)
    {
        NYX_JobVehicleId[playerid] = CreateVehicle(NYX_JobVehicle[job], NYX_JobPoint[job][0], NYX_JobPoint[job][1], NYX_JobPoint[job][2], 0.0, -1, -1, 300);
        PutPlayerInVehicle(playerid, NYX_JobVehicleId[playerid], 0);
    }
    SetPlayerCheckpoint(playerid, NYX_JobPoint[job][0] + 25.0, NYX_JobPoint[job][1] + 25.0, NYX_JobPoint[job][2], 5.0);
    return SendClientMessage(playerid, AUTH_GREEN, "Trabalho iniciado. Siga o GPS.");
}

stock NYX_ConcludeJob(playerid)
{
    if (!NYX_JobRunning[playerid]) return SendClientMessage(playerid, AUTH_RED, "Nenhum servico em andamento.");
    new job = NYX_PlayerJob[playerid], pay = NYX_JobPay[job];
    GivePlayerMoney(playerid, pay);
    NYX_Player[playerid][NYX_Money] = GetPlayerMoney(playerid);
    NYX_JobRunning[playerid] = false;
    DisablePlayerCheckpoint(playerid);
    if (NYX_JobVehicleId[playerid] != INVALID_VEHICLE_ID)
    {
        RemovePlayerFromVehicle(playerid);
        DestroyVehicle(NYX_JobVehicleId[playerid]);
        NYX_JobVehicleId[playerid] = INVALID_VEHICLE_ID;
    }
    new msg[96]; format(msg, sizeof msg, "Servico concluido! +$%d", pay);
    return SendClientMessage(playerid, AUTH_GREEN, msg);
}

public NYX_AutoSave()
{
    for (new i = 0; i < MAX_PLAYERS; i++)
    {
        if (IsPlayerConnected(i) && NYX_Player[i][NYX_Logged])
        {
            NYX_Player[i][NYX_Money] = GetPlayerMoney(i);
            NYX_SaveAccount(i);
            NYX_ProfileSave(i);
        }
    }
    return 1;
}

public OnPlayerEnterCheckpoint(playerid)
{
    if (NYX_JobRunning[playerid])
    {
        DisablePlayerCheckpoint(playerid);
        SendClientMessage(playerid, AUTH_GREEN, "Destino alcancado. Use /concluir.");
    }
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!NYX_Player[playerid][NYX_Logged]) return 1;
    if (!strcmp(cmdtext, "/orgs", true))
    {
        new list[2048], line[96]; list[0] = EOS;
        for (new i = 0; i < NYX_ORG_COUNT; i++)
        {
            format(line, sizeof line, "[%s] %s\n", NYX_OrgActive[i] ? "ATIVA" : "OFFLINE", NYX_OrgName[i]);
            strcat(list, line, sizeof list);
        }
        return ShowPlayerDialog(playerid, D_ORGS, DIALOG_STYLE_LIST, "NYX | ORGANIZACOES", list, "VER", "FECHAR");
    }
    if (!strcmp(cmdtext, "/empregos", true)) return NYX_ShowJobs(playerid);
    if (!strcmp(cmdtext, "/trabalhar", true)) return NYX_StartJob(playerid);
    if (!strcmp(cmdtext, "/concluir", true)) return NYX_ConcludeJob(playerid);
    if (!strcmp(cmdtext, "/lanchonetes", true)) return NYX_ShowFood(playerid);
    if (!strcmp(cmdtext, "/imoveis", true)) return NYX_ShowProperties(playerid);
    if (!strcmp(cmdtext, "/leilao", true)) return NYX_ShowAuction(playerid);
    if (!strcmp(cmdtext, "/familias", true)) return NYX_ShowFamilies(playerid);
    if (!strcmp(cmdtext, "/skin", true)) return ShowPlayerDialog(playerid, D_SKINS, DIALOG_STYLE_INPUT, "NYX | SKINS", "Digite o ID da skin (0-311).", "USAR", "FECHAR");
    if (!strcmp(cmdtext, "/status", true))
    {
        new job[48], org[64], msg[256];
        NYX_GetJobName(NYX_PlayerJob[playerid], job, sizeof job);
        format(org, sizeof org, "Civil");
        if (NYX_IsValidOrg(NYX_Player[playerid][NYX_Org])) format(org, sizeof org, "%s", NYX_OrgName[NYX_Player[playerid][NYX_Org]]);
        format(msg, sizeof msg, "NYX | Dinheiro: $%d | NCoins: %d | Skin: %d | Emprego: %s | Org: %s", GetPlayerMoney(playerid), NYX_Player[playerid][NYX_NCoins], NYX_Player[playerid][NYX_Skin], job, org);
        return SendClientMessage(playerid, COLOR_WHITE, msg);
    }
    if (!strcmp(cmdtext, "/ncoins", true))
    {
        new msg[96]; format(msg, sizeof msg, "Seu saldo: %d NCoins.", NYX_Player[playerid][NYX_NCoins]);
        return SendClientMessage(playerid, COLOR_NYX, msg);
    }
    if (!strcmp(cmdtext, "/mundo", true)) return NYX_ShowWorldInfo(playerid);
    if (!strcmp(cmdtext, "/mobileinfo", true))
    {
        new payload[512];
        NYX_MobileBuildState(playerid, payload, sizeof payload);
        SendClientMessage(playerid, COLOR_NYX, "NYX Mobile | estado enviado.");
        return SendClientMessage(playerid, COLOR_WHITE, payload);
    }
    if (!strcmp(cmdtext, "/casamento", true)) return NYX_MarriageExplain(playerid);
    if (!strcmp(cmdtext, "/loja", true)) return ShowPlayerDialog(playerid, D_STORE, DIALOG_STYLE_MSGBOX, "NYX | NCOINS", "Moeda premium NYX.\nSkins premium podem usar NCoins.", "OK", "");
    if (!strcmp(cmdtext, "/gps", true))
    {
        SetPlayerCheckpoint(playerid, NYX_SPAWN_X, NYX_SPAWN_Y, NYX_SPAWN_Z, 4.0);
        return SendClientMessage(playerid, AUTH_GREEN, "GPS marcado: Centro NYX.");
    }
    if (!strcmp(cmdtext, "/ajuda", true))
        return ShowPlayerDialog(playerid, D_HELP, DIALOG_STYLE_MSGBOX, "NYX | AJUDA", "/empregos /trabalhar /concluir\n/orgs /status /skin /mobileinfo\n/lanchonetes /imoveis /leilao /familias\n/ncoins /loja /mundo /gps\n/casamento /ajuda", "FECHAR", "");
    return 0;
}
