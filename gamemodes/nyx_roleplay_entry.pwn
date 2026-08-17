// NYX account entrypoint: custom account name/password/IP/ID persistence.
// Legacy routines remain available so existing RP modules keep their behavior.
#define NYX_AccountFile NYX_Legacy_AccountFile
#define NYX_AccountExists NYX_Legacy_AccountExists
#define NYX_SaveAccount NYX_Legacy_SaveAccount
#define NYX_LoadAccount NYX_Legacy_LoadAccount
#define NYX_CreateAccount NYX_Legacy_CreateAccount
#define NYX_AutoSave NYX_Legacy_AutoSave
#define OnPlayerConnect NYX_Legacy_OnPlayerConnect
#define OnPlayerDisconnect NYX_Legacy_OnPlayerDisconnect
#define OnDialogResponse NYX_Legacy_OnDialogResponse

#include "nyx_roleplay.pwn"

#undef NYX_AccountFile
#undef NYX_AccountExists
#undef NYX_SaveAccount
#undef NYX_LoadAccount
#undef NYX_CreateAccount
#undef NYX_AutoSave
#undef OnPlayerConnect
#undef OnPlayerDisconnect
#undef OnDialogResponse

#include <nyx_identity_v3>

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
    NYX_ResetPlayer(playerid);
    NYX_ResetJob(playerid);
    NYX_ResetMarriage(playerid);
    NYX2_InitPlayer(playerid);
    NYX_PendingMarriage[playerid]=INVALID_PLAYER_ID;
    NYX_IdentityReset(playerid);
    NYX_ApplyGraphicsProfile(playerid);
    NYX_IdentityShowNameDialog(playerid);
    return 1;
}

public OnPlayerDisconnect(playerid,reason)
{
    if(NYX_Player[playerid][NYX_Logged]) NYX_SaveAccount(playerid);
    NYX_IdentityReset(playerid);
    return NYX_Legacy_OnPlayerDisconnect(playerid,reason);
}

public OnDialogResponse(playerid,dialogid,response,listitem,inputtext[])
{
    switch(dialogid)
    {
        case NYX_AUTH_NAME:
        {
            if(strlen(inputtext)<3 || strlen(inputtext)>NYX_AUTH_MAX_NAME)
            {
                SendClientMessage(playerid,COLOR_ERROR,"Nome invalido. Use de 3 a 24 caracteres.");
                return NYX_IdentityShowNameDialog(playerid);
            }
            if(response)
            {
                if(NYX_IdentityFindByName(inputtext)<=0)
                {
                    SendClientMessage(playerid,COLOR_ERROR,"Conta nao encontrada. Use REGISTRAR para criar uma nova conta.");
                    return NYX_IdentityShowNameDialog(playerid);
                }
                format(NYX_IdentityName[playerid],sizeof NYX_IdentityName[],"%s",inputtext);
                return NYX_IdentityShowLoginPassword(playerid);
            }
            if(NYX_IdentityFindByName(inputtext)>0)
            {
                SendClientMessage(playerid,COLOR_WARNING,"Esse nome ja possui uma conta. Entre com a senha.");
                format(NYX_IdentityName[playerid],sizeof NYX_IdentityName[],"%s",inputtext);
                return NYX_IdentityShowLoginPassword(playerid);
            }
            format(NYX_IdentityName[playerid],sizeof NYX_IdentityName[],"%s",inputtext);
            return NYX_IdentityShowRegisterPassword(playerid);
        }
        case NYX_AUTH_PASSWORD:
        {
            if(!response)
                return NYX_IdentityShowNameDialog(playerid);
            if(strlen(inputtext)<4)
            {
                SendClientMessage(playerid,COLOR_ERROR,"A senha precisa ter pelo menos 4 caracteres.");
                if(NYX_AuthStage[playerid]==4) return NYX_IdentityShowRegisterPassword(playerid);
                return NYX_IdentityShowLoginPassword(playerid);
            }
            if(NYX_AuthStage[playerid]==4)
            {
                if(!NYX_IdentityCreate(playerid,NYX_IdentityName[playerid],inputtext))
                {
                    SendClientMessage(playerid,COLOR_ERROR,"Nao foi possivel criar a conta. Verifique o nome e tente novamente.");
                    return NYX_IdentityShowNameDialog(playerid);
                }
                NYX_Player[playerid][NYX_Logged]=1;
                ResetPlayerMoney(playerid);
                GivePlayerMoney(playerid,NYX_Player[playerid][NYX_Money]);
                NYX_SaveAccount(playerid);
                SendClientMessage(playerid,COLOR_SUCCESS,"Conta criada e salva com sucesso!");
                SendClientMessage(playerid,COLOR_WHITE,"Seu ID NYX comeca em 1 e fica vinculado ao seu nome, senha e IP.");
                SpawnPlayer(playerid);
                return 1;
            }
            new auth=NYX_IdentityLoad(playerid,NYX_IdentityName[playerid],inputtext);
            if(auth==1)
            {
                NYX_Player[playerid][NYX_Logged]=1;
                ResetPlayerMoney(playerid);
                GivePlayerMoney(playerid,NYX_Player[playerid][NYX_Money]);
                NYX_SaveAccount(playerid);
                SendClientMessage(playerid,COLOR_SUCCESS,"Login realizado. Seus dados foram carregados.");
                SpawnPlayer(playerid);
                return 1;
            }
            if(auth==-1)
            {
                SendClientMessage(playerid,COLOR_ERROR,"Senha incorreta.");
                return NYX_IdentityShowLoginPassword(playerid);
            }
            SendClientMessage(playerid,COLOR_ERROR,"Conta nao encontrada.");
            return NYX_IdentityShowNameDialog(playerid);
        }
    }
    return NYX_Legacy_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
}

// SA-MP/Pawn requires a valid AMX entry point.
main() {}
