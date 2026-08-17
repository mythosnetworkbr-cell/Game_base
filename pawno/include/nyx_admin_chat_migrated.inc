#if defined _NYX_ADMIN_CHAT_MIGRATED_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_CHAT_MIGRATED_INCLUDED

// Comandos enviados pelo projeto/Chat anterior, centralizados na GameMode NYX.
// Os comandos que ja existem em nyx_admin_complete sao delegados primeiro para
// manter a implementacao centralizada e evitar duas regras diferentes.

#define NYX_MIGRATED_LOG_DIALOG 8520
#define NYX_MIGRATED_MAX_ADMIN_VEHICLES 100

new NYX_MigratedAdminVehicle[NYX_MIGRATED_MAX_ADMIN_VEHICLES];
new NYX_MigratedAdminVehicleOwner[NYX_MIGRATED_MAX_ADMIN_VEHICLES];
new bool:NYX_MigratedAdminVehicleLocked[NYX_MIGRATED_MAX_ADMIN_VEHICLES];
new bool:NYX_MigratedSpec[MAX_PLAYERS];
new Float:NYX_MigratedSpecPos[MAX_PLAYERS][3];
new NYX_MigratedSpecInterior[MAX_PLAYERS];
new NYX_MigratedSpecVW[MAX_PLAYERS];

stock NYX_MigratedAdminInit()
{
    for(new i=0;i<NYX_MIGRATED_MAX_ADMIN_VEHICLES;i++)
    {
        NYX_MigratedAdminVehicle[i]=INVALID_VEHICLE_ID;
        NYX_MigratedAdminVehicleOwner[i]=INVALID_PLAYER_ID;
        NYX_MigratedAdminVehicleLocked[i]=false;
    }
    for(new p=0;p<MAX_PLAYERS;p++) NYX_MigratedSpec[p]=false;
    return 1;
}

stock NYX_MigratedAdminCan(playerid, level)
{
    return NYX_Player[playerid][NYX_Admin] >= level;
}

stock NYX_MigratedAdminReason(const text[], out[], size)
{
    new p=strfind(text," ",true);
    if(p<0){out[0]=EOS;return 0;}
    format(out,size,"%s",text[p+1]);
    return 1;
}

stock NYX_MigratedAdminFindVehicleSlot(vehicleid)
{
    for(new i=0;i<NYX_MIGRATED_MAX_ADMIN_VEHICLES;i++)
        if(NYX_MigratedAdminVehicle[i]==vehicleid) return i;
    return -1;
}

stock NYX_MigratedAdminFindFreeVehicleSlot()
{
    for(new i=0;i<NYX_MIGRATED_MAX_ADMIN_VEHICLES;i++)
        if(NYX_MigratedAdminVehicle[i]==INVALID_VEHICLE_ID) return i;
    return -1;
}

stock NYX_MigratedAdminCommand(playerid, cmdtext[])
{
    // Primeiro usa a implementacao central ja existente no modulo administrativo.
    if(NYX_AdminCommand(playerid,cmdtext)) return 1;

    new id, value, model, slot, Float:x, Float:y, Float:z, Float:a, msg[192], reason[96];

    // /verlogs
    if(!strcmp(cmdtext,"/verlogs",true) || !strncmp(cmdtext,"/verlogs ",9,true))
    {
        if(!NYX_MigratedAdminCan(playerid,5)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Voce nao possui permissao para visualizar os logs.");
        new filter[32], line[256], content[2048], File:file;
        filter[0]=EOS;
        if(strlen(cmdtext)>9) format(filter,sizeof filter,"%s",cmdtext[9]);
        content[0]=EOS;
        file=fopen("scriptfiles/nyx_admin_logs.dat",io_read);
        if(!file) return SendClientMessage(playerid,COLOR_ERROR,"[LOGS]: Nenhum log foi encontrado.");
        new count=0;
        while(fread(file,line,sizeof line) && count<15)
        {
            if(filter[0] && strfind(line,filter,true)==-1) continue;
            strcat(content,line,sizeof content); strcat(content,"\n",sizeof content); count++;
        }
        fclose(file);
        if(!count) return SendClientMessage(playerid,COLOR_WARNING,"[LOGS]: Nenhum log foi encontrado para essa busca.");
        return ShowPlayerDialog(playerid,NYX_MIGRATED_LOG_DIALOG,DIALOG_STYLE_MSGBOX,"NYX | LOGS ADMINISTRATIVOS",content,"Fechar","");
    }

    // /unban [nome]
    if(!strncmp(cmdtext,"/unban ",true,7))
    {
        if(!NYX_MigratedAdminCan(playerid,4)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        new targetName[32]; format(targetName,sizeof targetName,"%s",cmdtext[7]);
        new path[96], marker[96], File:file;
        format(path,sizeof path,"scriptfiles/nyx_unban_%s.dat",targetName);
        file=fopen(path,io_write);
        if(file){fwrite(file,"UNBANNED");fclose(file);}
        format(msg,sizeof msg,"[ADM-UNBAN]: Conta %s marcada como desbanida.",targetName);
        SendAdminMessage(COLOR_SUCCESS,msg);
        NYX_AdminLog(playerid,INVALID_PLAYER_ID,"/unban",targetName);
        return 1;
    }

    // /criarcarro e /v
    if(!strncmp(cmdtext,"/criarcarro ",true,12) || !strncmp(cmdtext,"/v ",true,3))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        new start = !strncmp(cmdtext,"/v ",true,3) ? 3 : 12;
        model=strval(cmdtext[start]);
        if(model<400 || model>611) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Modelo invalido. Use 400-611.");
        slot=NYX_MigratedAdminFindFreeVehicleSlot();
        if(slot<0) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Limite de veiculos administrativos atingido.");
        GetPlayerPos(playerid,x,y,z); GetPlayerFacingAngle(playerid,a);
        NYX_MigratedAdminVehicle[slot]=CreateVehicle(model,x+2.0,y,z,a,random(126),random(126),-1);
        NYX_MigratedAdminVehicleOwner[slot]=playerid;
        NYX_MigratedAdminVehicleLocked[slot]=false;
        PutPlayerInVehicle(playerid,NYX_MigratedAdminVehicle[slot],0);
        format(msg,sizeof msg,"[ADM]: Veiculo modelo %d criado. ID: %d | Slot: %d",model,NYX_MigratedAdminVehicle[slot],slot);
        SendClientMessage(playerid,COLOR_SUCCESS,msg);
        NYX_AdminLog(playerid,INVALID_PLAYER_ID,"/criarcarro",msg);
        return 1;
    }

    // /puxarcarro [ID]
    if(!strncmp(cmdtext,"/puxarcarro ",true,12))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        id=strval(cmdtext[12]);
        if(id<1 || id>=MAX_VEHICLES) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: ID de veiculo invalido.");
        GetPlayerPos(playerid,x,y,z);
        SetVehiclePos(id,x+2.0,y+2.0,z);
        LinkVehicleToInterior(id,GetPlayerInterior(playerid));
        SetVehicleVirtualWorld(id,GetPlayerVirtualWorld(playerid));
        return SendClientMessage(playerid,COLOR_SUCCESS,"[ADM]: Veiculo puxado ate voce.");
    }

    // /spec [ID] e /specoff
    if(!strncmp(cmdtext,"/spec ",true,6) || !strncmp(cmdtext,"/sp ",true,4))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        new start=!strncmp(cmdtext,"/sp ",true,4)?4:6;
        id=strval(cmdtext[start]);
        if(id==playerid || !IsPlayerConnected(id)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Jogador invalido.");
        GetPlayerPos(playerid,NYX_MigratedSpecPos[playerid][0],NYX_MigratedSpecPos[playerid][1],NYX_MigratedSpecPos[playerid][2]);
        NYX_MigratedSpecInterior[playerid]=GetPlayerInterior(playerid);
        NYX_MigratedSpecVW[playerid]=GetPlayerVirtualWorld(playerid);
        NYX_MigratedSpec[playerid]=true;
        TogglePlayerSpectating(playerid,1);
        if(IsPlayerInAnyVehicle(id)) PlayerSpectateVehicle(playerid,GetPlayerVehicleID(id));
        else PlayerSpectatePlayer(playerid,id);
        return 1;
    }
    if(!strcmp(cmdtext,"/specoff",true) || !strcmp(cmdtext,"/spoff",true))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        if(!NYX_MigratedSpec[playerid]) return SendClientMessage(playerid,COLOR_WARNING,"Voce nao esta espectando ninguem.");
        TogglePlayerSpectating(playerid,0);
        SetPlayerPos(playerid,NYX_MigratedSpecPos[playerid][0],NYX_MigratedSpecPos[playerid][1],NYX_MigratedSpecPos[playerid][2]);
        SetPlayerInterior(playerid,NYX_MigratedSpecInterior[playerid]);
        SetPlayerVirtualWorld(playerid,NYX_MigratedSpecVW[playerid]);
        NYX_MigratedSpec[playerid]=false;
        return 1;
    }

    // /sethp [ID] [vida]
    if(!strncmp(cmdtext,"/sethp ",true,7))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        id=strval(cmdtext[7]); new p=strfind(cmdtext," ",true,7); if(p<0) return SendClientMessage(playerid,COLOR_WARNING,"Uso: /sethp [id] [vida]"); value=strval(cmdtext[p+1]);
        if(!IsPlayerConnected(id)||value<1||value>100) return SendClientMessage(playerid,COLOR_ERROR,"Jogador ou vida invalida.");
        SetPlayerHealth(id,float(value)); return 1;
    }

    // /setarmour [ID] [colete]
    if(!strncmp(cmdtext,"/setarmour ",true,11))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        id=strval(cmdtext[11]); new p=strfind(cmdtext," ",true,11); if(p<0) return SendClientMessage(playerid,COLOR_WARNING,"Uso: /setarmour [id] [colete]"); value=strval(cmdtext[p+1]);
        if(!IsPlayerConnected(id)||value<0||value>100) return SendClientMessage(playerid,COLOR_ERROR,"Jogador ou colete invalido.");
        SetPlayerArmour(id,float(value)); return 1;
    }

    // /dararma [ID] [arma] [municao]
    if(!strncmp(cmdtext,"/dararma ",true,9))
    {
        if(!NYX_MigratedAdminCan(playerid,2)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        new rest[64], p1, p2, weapon, ammo;
        format(rest,sizeof rest,"%s",cmdtext[9]); id=strval(rest); p1=strfind(rest," ",true); if(p1<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /dararma [id] [arma] [municao]"); weapon=strval(rest[p1+1]); p2=strfind(rest," ",true,p1+1); if(p2<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /dararma [id] [arma] [municao]"); ammo=strval(rest[p2+1]);
        if(!IsPlayerConnected(id)||weapon<1||weapon>46||ammo<1) return SendClientMessage(playerid,COLOR_ERROR,"Parametros invalidos.");
        GivePlayerWeapon(id,weapon,ammo); return 1;
    }

    // /jetpack
    if(!strcmp(cmdtext,"/jetpack",true))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        SetPlayerSpecialAction(playerid,SPECIAL_ACTION_USEJETPACK); return SendClientMessage(playerid,COLOR_SUCCESS,"Jetpack ativado.");
    }

    // /ann [mensagem]
    if(!strncmp(cmdtext,"/ann ",true,5))
    {
        if(!NYX_MigratedAdminCan(playerid,2)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        GameTextForAll(cmdtext[5],5000,3); NYX_AdminLog(playerid,INVALID_PLAYER_ID,"/ann",cmdtext[5]); return 1;
    }

    return 0;
}
