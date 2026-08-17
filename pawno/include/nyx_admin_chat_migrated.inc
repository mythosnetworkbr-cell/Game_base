#if defined _NYX_ADMIN_CHAT_MIGRATED_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_CHAT_MIGRATED_INCLUDED

// Comandos administrativos enviados anteriormente no chat, agora integrados
// diretamente ao GameMode Game_base/NYX. Este modulo usa somente o estado central
// NYX_Player e natives SA-MP, evitando dependencias de outro projeto.

#define NYX_MIGRATED_LOG_DIALOG 8520
#define NYX_MIGRATED_MAX_ADMIN_VEHICLES 100
#define NYX_MIGRATED_AUTO_JAIL_SECONDS 1800
#define NYX_MIGRATED_PAYDAY_SECONDS 3600
#define NYX_MIGRATED_PAYDAY_VALUE 2500

new NYX_MigratedAdminVehicle[NYX_MIGRATED_MAX_ADMIN_VEHICLES];
new NYX_MigratedAdminVehicleOwner[NYX_MIGRATED_MAX_ADMIN_VEHICLES];
new bool:NYX_MigratedAdminVehicleLocked[NYX_MIGRATED_MAX_ADMIN_VEHICLES];
new bool:NYX_MigratedSpec[MAX_PLAYERS];
new Float:NYX_MigratedSpecPos[MAX_PLAYERS][3];
new NYX_MigratedSpecInterior[MAX_PLAYERS];
new NYX_MigratedSpecVW[MAX_PLAYERS];
new bool:NYX_MigratedGod[MAX_PLAYERS];
new bool:NYX_MigratedInvisible[MAX_PLAYERS];
new NYX_MigratedJailTime[MAX_PLAYERS];
new NYX_MigratedWarnings[MAX_PLAYERS];
new NYX_MigratedPaydayTime[MAX_PLAYERS];

stock NYX_MigratedAdminInit()
{
    for(new i=0;i<NYX_MIGRATED_MAX_ADMIN_VEHICLES;i++)
    {
        NYX_MigratedAdminVehicle[i]=INVALID_VEHICLE_ID;
        NYX_MigratedAdminVehicleOwner[i]=INVALID_PLAYER_ID;
        NYX_MigratedAdminVehicleLocked[i]=false;
    }
    for(new p=0;p<MAX_PLAYERS;p++)
    {
        NYX_MigratedSpec[p]=false;
        NYX_MigratedGod[p]=false;
        NYX_MigratedInvisible[p]=false;
        NYX_MigratedJailTime[p]=0;
        NYX_MigratedWarnings[p]=0;
        NYX_MigratedPaydayTime[p]=0;
    }
    return 1;
}

stock NYX_MigratedAdminCan(playerid, level)
{
    return NYX_Player[playerid][NYX_Admin] >= level;
}

stock NYX_MigratedAdminLog(playerid,targetid,const command[],const details[])
{
    new adminName[MAX_PLAYER_NAME],targetName[MAX_PLAYER_NAME],line[256],File:file;
    GetPlayerName(playerid,adminName,sizeof adminName);
    if(targetid!=INVALID_PLAYER_ID && IsPlayerConnected(targetid)) GetPlayerName(targetid,targetName,sizeof targetName);
    else format(targetName,sizeof targetName,"SERVER");
    file=fopen("scriptfiles/nyx_admin_logs.dat",io_append);
    if(!file) return 0;
    format(line,sizeof line,"%d | %s | %s | %s | %s",gettime(),adminName,targetName,command,details);
    fwrite(file,line); fclose(file); return 1;
}

stock NYX_MigratedAdminReason(const text[],out[],size)
{
    new p=strfind(text," ",true);
    if(p<0){out[0]=EOS;return 0;}
    format(out,size,"%s",text[p+1]); return 1;
}

stock NYX_MigratedAdminFindFreeVehicleSlot()
{
    for(new i=0;i<NYX_MIGRATED_MAX_ADMIN_VEHICLES;i++)
        if(NYX_MigratedAdminVehicle[i]==INVALID_VEHICLE_ID || NYX_MigratedAdminVehicle[i]==0) return i;
    return -1;
}

stock NYX_MigratedAdminCommand(playerid,cmdtext[])
{
    new id,value,model,slot,Float:x,Float:y,Float:z,Float:a,msg[192],reason[96];

    // CHAT STAFF
    if(!strncmp(cmdtext,"/a ",true,3))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        format(msg,sizeof msg,"[CHAT STAFF] Admin %d: %s",playerid,cmdtext[3]);
        for(new i=0;i<MAX_PLAYERS;i++) if(IsPlayerConnected(i)&&NYX_Player[i][NYX_Admin]>0) SendClientMessage(i,COLOR_WARNING,msg);
        NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/a",cmdtext[3]); return 1;
    }

    // PUNICOES
    if(!strncmp(cmdtext,"/kick ",true,6))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        id=strval(cmdtext[6]); if(!IsPlayerConnected(id)) return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado.");
        if(NYX_Player[id][NYX_Admin]>=NYX_Player[playerid][NYX_Admin] && NYX_Player[playerid][NYX_Admin]<100) return SendClientMessage(playerid,COLOR_ERROR,"Voce nao pode punir administrador igual ou superior.");
        NYX_MigratedAdminReason(cmdtext[6],reason,sizeof reason); NYX_MigratedAdminLog(playerid,id,"/kick",reason); SetTimerEx("NYX_MigratedDelayedKick",100,false,"i",id); return 1;
    }
    if(!strncmp(cmdtext,"/ban ",true,5))
    {
        if(!NYX_MigratedAdminCan(playerid,1)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        id=strval(cmdtext[5]); if(!IsPlayerConnected(id)) return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado.");
        if(NYX_Player[id][NYX_Admin]>=NYX_Player[playerid][NYX_Admin] && NYX_Player[playerid][NYX_Admin]<100) return SendClientMessage(playerid,COLOR_ERROR,"Voce nao pode banir administrador igual ou superior.");
        new name[MAX_PLAYER_NAME],File:file; GetPlayerName(id,name,sizeof name); NYX_MigratedAdminReason(cmdtext[5],reason,sizeof reason);
        file=fopen("scriptfiles/nyx_bans.dat",io_append); if(file){format(msg,sizeof msg,"%s|%d|%s",name,gettime(),reason);fwrite(file,msg);fclose(file);} NYX_MigratedAdminLog(playerid,id,"/ban",reason); SetTimerEx("NYX_MigratedDelayedKick",100,false,"i",id); return 1;
    }
    if(!strncmp(cmdtext,"/banip ",true,7))
    {
        if(!NYX_MigratedAdminCan(playerid,3)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Este comando exige Admin 2 (nivel 3).");
        id=strval(cmdtext[7]); if(!IsPlayerConnected(id)) return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado.");
        new ip[46],path[96],File:file; GetPlayerIp(id,ip,sizeof ip); format(path,sizeof path,"scriptfiles/nyx_ipban_%s.dat",ip); file=fopen(path,io_write); if(file){fwrite(file,"BANNED");fclose(file);} NYX_MigratedAdminLog(playerid,id,"/banip",ip); SetTimerEx("NYX_MigratedDelayedKick",100,false,"i",id); return 1;
    }
    if(!strncmp(cmdtext,"/unban ",true,7))
    {
        if(!NYX_MigratedAdminCan(playerid,4)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        new targetName[32],path[96],File:file; format(targetName,sizeof targetName,"%s",cmdtext[7]); format(path,sizeof path,"scriptfiles/nyx_unban_%s.dat",targetName); file=fopen(path,io_write); if(file){fwrite(file,"UNBANNED");fclose(file);} NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/unban",targetName); return 1;
    }

    // JAIL / ADVERTENCIAS
    if(!strncmp(cmdtext,"/jail ",true,6))
    {
        if(!NYX_MigratedAdminCan(playerid,2)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");
        new rest[128],p1,p2,minutes; format(rest,sizeof rest,"%s",cmdtext[6]); id=strval(rest); p1=strfind(rest," ",true); if(p1<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /jail [id] [minutos] [motivo]"); minutes=strval(rest[p1+1]); p2=strfind(rest," ",true,p1+1); if(p2<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /jail [id] [minutos] [motivo]"); format(reason,sizeof reason,"%s",rest[p2+1]);
        if(!IsPlayerConnected(id)||minutes<1||minutes>300)return SendClientMessage(playerid,COLOR_ERROR,"Parametros invalidos."); NYX_MigratedJailTime[id]=minutes*60; SetPlayerPos(id,264.6288,77.5742,1001.0391);SetPlayerFacingAngle(id,270.0);SetPlayerInterior(id,6);SetPlayerVirtualWorld(id,1);ResetPlayerWeapons(id);NYX_MigratedAdminLog(playerid,id,"/jail",reason);return 1;
    }
    if(!strncmp(cmdtext,"/unjail ",true,8))
    {
        if(!NYX_MigratedAdminCan(playerid,2)) return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao."); id=strval(cmdtext[8]); if(!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado."); NYX_MigratedJailTime[id]=0;SetPlayerInterior(id,0);SetPlayerVirtualWorld(id,0);SetPlayerPos(id,1481.0,-1771.0,18.8);NYX_MigratedAdminLog(playerid,id,"/unjail","Liberou da prisao");return 1;
    }
    if(!strncmp(cmdtext,"/adv ",true,5))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao."); id=strval(cmdtext[5]); if(!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado."); NYX_MigratedWarnings[id]++; NYX_MigratedAdminReason(cmdtext[5],reason,sizeof reason); format(msg,sizeof msg,"[ADVERTENCIA] [%d/3] Motivo: %s",NYX_MigratedWarnings[id],reason);SendClientMessage(id,COLOR_WARNING,msg);NYX_MigratedAdminLog(playerid,id,"/adv",reason);if(NYX_MigratedWarnings[id]>=3){NYX_MigratedWarnings[id]=0;NYX_MigratedJailTime[id]=NYX_MIGRATED_AUTO_JAIL_SECONDS;SetPlayerPos(id,264.6288,77.5742,1001.0391);SetPlayerInterior(id,6);SetPlayerVirtualWorld(id,1);ResetPlayerWeapons(id);}return 1;
    }
    if(!strncmp(cmdtext,"/unadv ",true,7))
    {
        if(!NYX_MigratedAdminCan(playerid,2))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");id=strval(cmdtext[7]);if(!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado.");if(NYX_MigratedWarnings[id]>0)NYX_MigratedWarnings[id]--;NYX_MigratedAdminLog(playerid,id,"/unadv","Removeu advertencia");return 1;
    }
    if(!strncmp(cmdtext,"/desarmar ",true,10))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");id=strval(cmdtext[10]);if(!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado.");ResetPlayerWeapons(id);NYX_MigratedAdminLog(playerid,id,"/desarmar","Todas as armas removidas");return 1;
    }

    // VIDA / COLETE / ARMAS
    if(!strncmp(cmdtext,"/curar ",true,7))
    {
        if(!NYX_MigratedAdminCan(playerid,2))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");id=strval(cmdtext[7]);if(!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado.");SetPlayerHealth(id,100.0);SetPlayerArmour(id,100.0);NYX_MigratedAdminLog(playerid,id,"/curar","Vida e colete restaurados");return 1;
    }
    if(!strncmp(cmdtext,"/sethp ",true,7))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");id=strval(cmdtext[7]);new p=strfind(cmdtext," ",true,7);if(p<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /sethp [id] [vida]");value=strval(cmdtext[p+1]);if(!IsPlayerConnected(id)||value<0||value>100)return SendClientMessage(playerid,COLOR_ERROR,"Parametros invalidos.");SetPlayerHealth(id,float(value));return 1;
    }
    if(!strncmp(cmdtext,"/setarmour ",true,11))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");id=strval(cmdtext[11]);new p=strfind(cmdtext," ",true,11);if(p<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /setarmour [id] [colete]");value=strval(cmdtext[p+1]);if(!IsPlayerConnected(id)||value<0||value>100)return SendClientMessage(playerid,COLOR_ERROR,"Parametros invalidos.");SetPlayerArmour(id,float(value));return 1;
    }
    if(!strncmp(cmdtext,"/dararma ",true,9))
    {
        if(!NYX_MigratedAdminCan(playerid,2))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");new rest[64],p1,p2,weapon,ammo;format(rest,sizeof rest,"%s",cmdtext[9]);id=strval(rest);p1=strfind(rest," ",true);if(p1<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /dararma [id] [arma] [municao]");weapon=strval(rest[p1+1]);p2=strfind(rest," ",true,p1+1);if(p2<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /dararma [id] [arma] [municao]");ammo=strval(rest[p2+1]);if(!IsPlayerConnected(id)||weapon<1||weapon>46||ammo<1)return SendClientMessage(playerid,COLOR_ERROR,"Parametros invalidos.");GivePlayerWeapon(id,weapon,ammo);return 1;
    }

    // TELEPORTE
    if(!strncmp(cmdtext,"/ir ",true,4) || !strncmp(cmdtext,"/goto ",true,6))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");new start=!strncmp(cmdtext,"/goto ",true,6)?6:4;id=strval(cmdtext[start]);if(!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado.");GetPlayerPos(id,x,y,z);SetPlayerPos(playerid,x+1.5,y+1.5,z);SetPlayerInterior(playerid,GetPlayerInterior(id));SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(id));NYX_MigratedAdminLog(playerid,id,"/ir","Teleportou-se ate o jogador");return 1;
    }
    if(!strncmp(cmdtext,"/traz ",true,6) || !strncmp(cmdtext,"/gethere ",true,9))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");new start=!strncmp(cmdtext,"/gethere ",true,9)?9:6;id=strval(cmdtext[start]);if(!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado.");GetPlayerPos(playerid,x,y,z);SetPlayerPos(id,x+1.0,y+1.0,z);SetPlayerInterior(id,GetPlayerInterior(playerid));SetPlayerVirtualWorld(id,GetPlayerVirtualWorld(playerid));NYX_MigratedAdminLog(playerid,id,"/traz","Trouxe o jogador");return 1;
    }

    // ESPECTADOR
    if(!strncmp(cmdtext,"/spec ",true,6)||!strncmp(cmdtext,"/sp ",true,4))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");new start=!strncmp(cmdtext,"/sp ",true,4)?4:6;id=strval(cmdtext[start]);if(id==playerid||!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Jogador invalido.");GetPlayerPos(playerid,NYX_MigratedSpecPos[playerid][0],NYX_MigratedSpecPos[playerid][1],NYX_MigratedSpecPos[playerid][2]);NYX_MigratedSpecInterior[playerid]=GetPlayerInterior(playerid);NYX_MigratedSpecVW[playerid]=GetPlayerVirtualWorld(playerid);NYX_MigratedSpec[playerid]=true;TogglePlayerSpectating(playerid,1);if(IsPlayerInAnyVehicle(id))PlayerSpectateVehicle(playerid,GetPlayerVehicleID(id));else PlayerSpectatePlayer(playerid,id);return 1;
    }
    if(!strcmp(cmdtext,"/specoff",true)||!strcmp(cmdtext,"/spoff",true))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");TogglePlayerSpectating(playerid,0);SetPlayerPos(playerid,NYX_MigratedSpecPos[playerid][0],NYX_MigratedSpecPos[playerid][1],NYX_MigratedSpecPos[playerid][2]);SetPlayerInterior(playerid,NYX_MigratedSpecInterior[playerid]);SetPlayerVirtualWorld(playerid,NYX_MigratedSpecVW[playerid]);NYX_MigratedSpec[playerid]=false;return 1;
    }

    // INVISIVEL / GODMODE
    if(!strcmp(cmdtext,"/invisivel",true)||!strcmp(cmdtext,"/inv",true))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");NYX_MigratedInvisible[playerid]=!NYX_MigratedInvisible[playerid];SetPlayerAlpha(playerid,NYX_MigratedInvisible[playerid]?0:255);SetPlayerColor(playerid,NYX_MigratedInvisible[playerid]?0xFFFFFF00:0xFFFFFFFF);NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/invisivel",NYX_MigratedInvisible[playerid]?"Ativou":"Desativou");return 1;
    }
    if(!strcmp(cmdtext,"/visivel",true))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");NYX_MigratedInvisible[playerid]=false;SetPlayerAlpha(playerid,255);SetPlayerColor(playerid,0xFFFFFFFF);return 1;
    }
    if(!strcmp(cmdtext,"/godmode",true)||!strcmp(cmdtext,"/god",true))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");NYX_MigratedGod[playerid]=!NYX_MigratedGod[playerid];SetPlayerHealth(playerid,100.0);SetPlayerArmour(playerid,100.0);NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/godmode",NYX_MigratedGod[playerid]?"Ativou":"Desativou");return SendClientMessage(playerid,NYX_MigratedGod[playerid]?COLOR_SUCCESS:COLOR_WARNING,NYX_MigratedGod[playerid]?"Modo Deus ativado.":"Modo Deus desativado.");
    }

    // CHAT / ANUNCIO
    if(!strcmp(cmdtext,"/limparchat",true)||!strcmp(cmdtext,"/cc",true))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");for(new i=0;i<60;i++)SendClientMessageToAll(-1," ");NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/limparchat","Limpou chat global");return 1;
    }
    if(!strncmp(cmdtext,"/ann ",true,5))
    {
        if(!NYX_MigratedAdminCan(playerid,2))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");GameTextForAll(cmdtext[5],5000,3);NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/ann",cmdtext[5]);return 1;
    }

    // JETPACK
    if(!strcmp(cmdtext,"/jetpack",true))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");SetPlayerSpecialAction(playerid,SPECIAL_ACTION_USEJETPACK);return 1;
    }

    // VEICULOS ADMIN
    if(!strncmp(cmdtext,"/criarcarro ",true,12)||!strncmp(cmdtext,"/v ",true,3))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");new start=!strncmp(cmdtext,"/v ",true,3)?3:12;model=strval(cmdtext[start]);if(model<400||model>611)return SendClientMessage(playerid,COLOR_ERROR,"Modelo invalido. Use 400-611.");slot=NYX_MigratedAdminFindFreeVehicleSlot();if(slot<0)return SendClientMessage(playerid,COLOR_ERROR,"Limite de veiculos administrativos atingido.");GetPlayerPos(playerid,x,y,z);GetPlayerFacingAngle(playerid,a);NYX_MigratedAdminVehicle[slot]=CreateVehicle(model,x+2.0,y,z,a,random(126),random(126),-1);NYX_MigratedAdminVehicleOwner[slot]=playerid;PutPlayerInVehicle(playerid,NYX_MigratedAdminVehicle[slot],0);format(msg,sizeof msg,"Veiculo modelo %d criado. ID %d | Slot %d",model,NYX_MigratedAdminVehicle[slot],slot);SendClientMessage(playerid,COLOR_SUCCESS,msg);NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/criarcarro",msg);return 1;
    }
    if(!strncmp(cmdtext,"/puxarcarro ",true,12))
    {
        if(!NYX_MigratedAdminCan(playerid,1))return SendClientMessage(playerid,COLOR_ERROR,"[ERRO]: Sem permissao.");id=strval(cmdtext[12]);if(id<1||id>=MAX_VEHICLES)return SendClientMessage(playerid,COLOR_ERROR,"ID de veiculo invalido.");GetPlayerPos(playerid,x,y,z);SetVehiclePos(id,x+2.0,y+2.0,z);LinkVehicleToInterior(id,GetPlayerInterior(playerid));SetVehicleVirtualWorld(id,GetPlayerVirtualWorld(playerid));return SendClientMessage(playerid,COLOR_SUCCESS,"Veiculo puxado ate voce.");
    }

    // LOGS
    if(!strcmp(cmdtext,"/verlogs",true)||!strncmp(cmdtext,"/verlogs ",true,9))
    {
        if(!NYX_MigratedAdminCan(playerid,5))return SendClientMessage(playerid,COLOR_ERROR,"Sem permissao para visualizar logs.");new filter[32],line[256],content[2048],File:file;filter[0]=EOS;if(strlen(cmdtext)>9)format(filter,sizeof filter,"%s",cmdtext[9]);content[0]=EOS;file=fopen("scriptfiles/nyx_admin_logs.dat",io_read);if(!file)return SendClientMessage(playerid,COLOR_WARNING,"Nenhum log encontrado.");new count=0;while(fread(file,line,sizeof line)&&count<15){if(filter[0]&&strfind(line,filter,true)==-1)continue;strcat(content,line,sizeof content);strcat(content,"\n",sizeof content);count++;}fclose(file);if(!count)return SendClientMessage(playerid,COLOR_WARNING,"Nenhum log encontrado para essa busca.");return ShowPlayerDialog(playerid,NYX_MIGRATED_LOG_DIALOG,DIALOG_STYLE_MSGBOX,"NYX | LOGS ADMINISTRATIVOS",content,"Fechar","");
    }

    // GMX / CLIMA / HORA
    if(!strcmp(cmdtext,"/gmx",true))
    {
        if(!NYX_MigratedAdminCan(playerid,100))return SendClientMessage(playerid,COLOR_ERROR,"Apenas Owners podem reiniciar a GM.");for(new i=0;i<MAX_PLAYERS;i++)if(IsPlayerConnected(i)&&NYX_Player[i][NYX_Logged])NYX_SaveAccount(i);SendClientMessageToAll(COLOR_ERROR,"[SERVIDOR] GameMode reiniciando...");NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/gmx","Reiniciou a GameMode");SetTimerEx("NYX_MigratedExecuteGMX",1000,false,"i",playerid);return 1;
    }
    if(!strncmp(cmdtext,"/clima ",true,7))
    {
        if(!NYX_MigratedAdminCan(playerid,2))return SendClientMessage(playerid,COLOR_ERROR,"Sem permissao.");value=strval(cmdtext[7]);if(value<0||value>45)return SendClientMessage(playerid,COLOR_ERROR,"Clima deve ser 0-45.");SetWeather(value);NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/clima",cmdtext[7]);return 1;
    }
    if(!strncmp(cmdtext,"/hora ",true,6))
    {
        if(!NYX_MigratedAdminCan(playerid,2))return SendClientMessage(playerid,COLOR_ERROR,"Sem permissao.");value=strval(cmdtext[6]);if(value<0||value>23)return SendClientMessage(playerid,COLOR_ERROR,"Hora deve ser 0-23.");SetWorldTime(value);NYX_MigratedAdminLog(playerid,INVALID_PLAYER_ID,"/hora",cmdtext[6]);return 1;
    }

    // SETADMIN / STATUS / PAYDAY
    if(!strncmp(cmdtext,"/setadmin ",true,10))
    {
        if(!NYX_MigratedAdminCan(playerid,100))return SendClientMessage(playerid,COLOR_ERROR,"Apenas Owner pode alterar cargos.");new rest[64],p1,level;format(rest,sizeof rest,"%s",cmdtext[10]);id=strval(rest);p1=strfind(rest," ",true);if(p1<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /setadmin [id] [nivel]");level=strval(rest[p1+1]);if(id==playerid||!IsPlayerConnected(id)||level<0||level>100)return SendClientMessage(playerid,COLOR_ERROR,"Parametros invalidos.");NYX_Player[id][NYX_Admin]=level;NYX_SaveAccount(id);NYX_MigratedAdminLog(playerid,id,"/setadmin","Nivel alterado");return 1;
    }
    if(!strcmp(cmdtext,"/status",true))
    {
        if(NYX_MigratedJailTime[playerid]>0){format(msg,sizeof msg,"[PRISAO] Tempo restante: %d minuto(s) e %d segundo(s).",NYX_MigratedJailTime[playerid]/60,NYX_MigratedJailTime[playerid]%60);return SendClientMessage(playerid,COLOR_WARNING,msg);}return 0;
    }
    if(!strcmp(cmdtext,"/payday",true))
    {
        new left=NYX_MIGRATED_PAYDAY_SECONDS-NYX_MigratedPaydayTime[playerid];if(left<0)left=0;format(msg,sizeof msg,"[PAYDAY] Faltam %d minuto(s) e %d segundo(s) para o proximo salario.",left/60,left%60);return SendClientMessage(playerid,COLOR_WARNING,msg);
    }

    return 0;
}

forward NYX_MigratedDelayedKick(playerid);
public NYX_MigratedDelayedKick(playerid){if(IsPlayerConnected(playerid))Kick(playerid);return 1;}
forward NYX_MigratedExecuteGMX(playerid);
public NYX_MigratedExecuteGMX(playerid){SendRconCommand("gmx");return 1;}
