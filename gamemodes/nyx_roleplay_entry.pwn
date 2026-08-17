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
#define OnPlayerCommandText NYX_Legacy_OnPlayerCommandText

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
#undef OnPlayerCommandText

#include <nyx_identity_v3>
#include <nyx_ipban>
#include <nyx_admin>
#include <nyx_admin_hierarchy>
#include <nyx_admin_hierarchy_cmds>

forward NYX_AutoSave();
forward OnPlayerConnect(playerid);
forward OnPlayerDisconnect(playerid, reason);
forward OnDialogResponse(playerid, dialogid, response, listitem, inputtext[]);
forward OnPlayerCommandText(playerid, cmdtext[]);

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
    if(NYX_IPIsBanned(playerid))
    {
        SendClientMessage(playerid,COLOR_ERROR,"Seu IP esta banido da NYX ROLEPLAY.");
        SetTimerEx("NYX_KickBanned",250,false,"i",playerid);
        return 1;
    }
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

forward NYX_KickBanned(playerid);
public NYX_KickBanned(playerid)
{
    if(IsPlayerConnected(playerid)) Kick(playerid);
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
            if(!NYX_IdentityValidateName(inputtext))
            {
                SendClientMessage(playerid,COLOR_ERROR,"Nome invalido. Use de 3 a 24 caracteres: letras, numeros e _.");
                return NYX_IdentityShowNameDialog(playerid);
            }
            if(response)
            {
                if(NYX_IdentityFindByName(inputtext)<=0)
                {
                    SendClientMessage(playerid,COLOR_ERROR,"Conta nao encontrada. Clique em REGISTRAR para criar uma conta.");
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
            if(!response) return NYX_IdentityShowNameDialog(playerid);
            if(strlen(inputtext)<4 || strlen(inputtext)>NYX_AUTH_MAX_PASSWORD)
            {
                SendClientMessage(playerid,COLOR_ERROR,"A senha precisa ter de 4 a 63 caracteres.");
                if(NYX_AuthStage[playerid]==4) return NYX_IdentityShowRegisterPassword(playerid);
                return NYX_IdentityShowLoginPassword(playerid);
            }
            if(NYX_AuthStage[playerid]==4)
            {
                if(!NYX_IdentityCreate(playerid,NYX_IdentityName[playerid],inputtext))
                {
                    SendClientMessage(playerid,COLOR_ERROR,"Nao foi possivel criar a conta. O nome pode ja estar em uso.");
                    return NYX_IdentityShowNameDialog(playerid);
                }
                NYX_Player[playerid][NYX_Logged]=1;
                ResetPlayerMoney(playerid);
                GivePlayerMoney(playerid,NYX_Player[playerid][NYX_Money]);
                NYX_SaveAccount(playerid);
                SendClientMessage(playerid,COLOR_SUCCESS,"Conta criada e salva com sucesso!");
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
            return NYX_IdentityShowNameDialog(playerid);
        }
    }
    return NYX_Legacy_OnDialogResponse(playerid,dialogid,response,listitem,inputtext);
}

public OnPlayerCommandText(playerid,cmdtext[])
{
    if(!NYX_Player[playerid][NYX_Logged]) return 1;
    if(!strcmp(cmdtext,"/meusdados",true))
    {
        new ip[46],msg[256];
        GetPlayerIp(playerid,ip,sizeof ip);
        format(msg,sizeof msg,"NYX | ID: %d | Nome: %s | IP: %s | Dinheiro: $%d | Banco: $%d",NYX_IdentityId[playerid],NYX_IdentityName[playerid],ip,GetPlayerMoney(playerid),NYX_Bank[playerid]);
        return SendClientMessage(playerid,COLOR_WHITE,msg);
    }
    if(!strcmp(cmdtext,"/meuid",true))
    {
        new msg[64];
        format(msg,sizeof msg,"Seu ID NYX: %d",NYX_IdentityId[playerid]);
        return SendClientMessage(playerid,COLOR_NYX,msg);
    }
    return NYX_Legacy_OnPlayerCommandText(playerid,cmdtext);
}

main() {}
