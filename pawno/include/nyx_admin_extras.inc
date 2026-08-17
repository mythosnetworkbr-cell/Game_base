#if defined _NYX_ADMIN_EXTRAS_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_EXTRAS_INCLUDED

#define NYX_ADMIN_LOG_DIALOG 8520
#define NYX_PAYDAY_INTERVAL 3600
#define NYX_PAYDAY_BASE 2500

new NYX_AdminExtrasTimer;

stock SendAdminMessage(color,const message[])
{
    for(new i=0;i<MAX_PLAYERS;i++)
        if(IsPlayerConnected(i)&&NYX_Player[i][NYX_Admin]>0)
            SendClientMessage(i,color,message);
    return 1;
}

stock NYX_AdminExtrasInit()
{
    NYX_AdminExtrasTimer=SetTimer("NYX_AdminExtrasSecond",1000,true);
    return 1;
}
stock NYX_AdminExtrasShutdown()
{
    if(NYX_AdminExtrasTimer) KillTimer(NYX_AdminExtrasTimer);
    NYX_AdminExtrasTimer=0;
    return 1;
}

forward NYX_AdminExtrasSecond();
public NYX_AdminExtrasSecond()
{
    for(new i=0;i<MAX_PLAYERS;i++)
    {
        if(!IsPlayerConnected(i)||!NYX_Player[i][NYX_Logged]) continue;
        NYX_Player[i][NYX_PaydayTime]++;
        if(NYX_Player[i][NYX_PaydayTime]>=NYX_PAYDAY_INTERVAL)
        {
            NYX_Player[i][NYX_PaydayTime]=0;
            // Administradores NAO recebem bonus de Payday.
            NYX_Bank[i]+=NYX_PAYDAY_BASE;
            PlayerPlaySound(i,1058,0.0,0.0,0.0);
            new msg[160];
            format(msg,sizeof msg,"[PAYDAY] Salario recebido: $%d. Saldo bancario: $%d.",NYX_PAYDAY_BASE,NYX_Bank[i]);
            SendClientMessage(i,COLOR_SUCCESS,msg);
            NYX_SaveAccount(i);
        }
    }
    return 1;
}

stock NYX_AdminExtrasLogLine(playerid,targetid,const command[],const details[])
{
    new adminName[MAX_PLAYER_NAME],targetName[MAX_PLAYER_NAME],line[256],File:file;
    GetPlayerName(playerid,adminName,sizeof adminName);
    if(targetid!=INVALID_PLAYER_ID&&IsPlayerConnected(targetid))GetPlayerName(targetid,targetName,sizeof targetName);
    else format(targetName,sizeof targetName,"SERVER");
    file=fopen("scriptfiles/nyx_admin_logs.dat",io_append);
    if(!file)return 0;
    format(line,sizeof line,"%d | %s | %s | %s | %s",gettime(),adminName,targetName,command,details);
    fwrite(file,line);fclose(file);return 1;
}

stock NYX_AdminExtrasCommand(playerid,cmdtext[])
{
    new id,level,hour,weather,days,start,model,Float:x,Float:y,Float:z,Float:a,msg[256],rest[160],reason[96],name[MAX_PLAYER_NAME],File:file;

    if(!strcmp(cmdtext,"/payday",true))
    {
        new left=NYX_PAYDAY_INTERVAL-NYX_Player[playerid][NYX_PaydayTime];
        if(left<0)left=0;
        format(msg,sizeof msg,"[PAYDAY] Faltam %d minuto(s) e %d segundo(s) para o proximo salario de $%d.",left/60,left%60,NYX_PAYDAY_BASE);
        return SendClientMessage(playerid,COLOR_WARNING,msg);
    }

    if(strfind(cmdtext,"/setadmin ",true)==0 || strfind(cmdtext,"/daradmin ",true)==0)
    {
        if(NYX_Player[playerid][NYX_Admin]<100)return SendClientMessage(playerid,COLOR_ERROR,"[ERRO] Apenas Owner pode alterar a Staff.");
        start=(strfind(cmdtext,"/setadmin ",true)==0)?10:10;
        format(rest,sizeof rest,"%s",cmdtext[start]);
        id=strval(rest);new p=strfind(rest," ",true);if(p<1)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /setadmin [id] [nivel]");level=strval(rest[p+1]);
        if(!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Jogador nao encontrado.");
        if(id==playerid)return SendClientMessage(playerid,COLOR_ERROR,"Voce nao pode alterar o proprio cargo.");
        if(!(level==0||level==1||level==2||level==3||level==4||level==5||level==6||level==7||level==80||level==100))return SendClientMessage(playerid,COLOR_ERROR,"Nivel invalido.");
        NYX_Player[id][NYX_Admin]=level;NYX_SaveAccount(id);GetPlayerName(id,name,sizeof name);
        format(msg,sizeof msg,"[STAFF] %s recebeu cargo nivel %d.",name,level);SendAdminMessage(COLOR_WARNING,msg);
        NYX_AdminExtrasLogLine(playerid,id,"/setadmin",msg);return 1;
    }

    if(!strcmp(cmdtext,"/gmx",true))
    {
        if(NYX_Player[playerid][NYX_Admin]<100)return SendClientMessage(playerid,COLOR_ERROR,"[ERRO] Apenas Owner pode reiniciar a GM.");
        SendClientMessageToAll(COLOR_ERROR,"[SERVIDOR] GameMode sendo reiniciada. Dados salvos.");
        for(new i=0;i<MAX_PLAYERS;i++)if(IsPlayerConnected(i)&&NYX_Player[i][NYX_Logged])NYX_SaveAccount(i);
        NYX_AdminExtrasLogLine(playerid,INVALID_PLAYER_ID,"/gmx","Reiniciou a GameMode");
        SetTimer("NYX_ExecuteGMX",1000,false);return 1;
    }

    if(strfind(cmdtext,"/clima ",true)==0)
    {
        if(NYX_Player[playerid][NYX_Admin]<2)return SendClientMessage(playerid,COLOR_ERROR,"[ERRO] Sem permissao.");weather=strval(cmdtext[7]);if(weather<0||weather>45)return SendClientMessage(playerid,COLOR_ERROR,"Clima deve ser 0-45.");SetWeather(weather);format(msg,sizeof msg,"[ADM] Clima alterado para %d.",weather);SendAdminMessage(COLOR_WARNING,msg);NYX_AdminExtrasLogLine(playerid,INVALID_PLAYER_ID,"/clima",cmdtext[7]);return 1;
    }
    if(strfind(cmdtext,"/hora ",true)==0)
    {
        if(NYX_Player[playerid][NYX_Admin]<2)return SendClientMessage(playerid,COLOR_ERROR,"[ERRO] Sem permissao.");hour=strval(cmdtext[6]);if(hour<0||hour>23)return SendClientMessage(playerid,COLOR_ERROR,"Hora deve ser 0-23.");SetWorldTime(hour);format(msg,sizeof msg,"[ADM] Hora alterada para %d:00.",hour);SendAdminMessage(COLOR_WARNING,msg);NYX_AdminExtrasLogLine(playerid,INVALID_PLAYER_ID,"/hora",cmdtext[6]);return 1;
    }

    if(strfind(cmdtext,"/verlogs",true)==0)
    {
        if(NYX_Player[playerid][NYX_Admin]<5)return SendClientMessage(playerid,COLOR_ERROR,"[ERRO] Manager ou superior.");
        new content[4096],line[256],lines[15][256],count=0,File:lf;
        content[0]=EOS;lf=fopen("scriptfiles/nyx_admin_logs.dat",io_read);
        if(lf){while(fread(lf,line,sizeof line)){if(count<15){format(lines[count],256,"%s",line);count++;}else{for(new j=0;j<14;j++)format(lines[j],256,"%s",lines[j+1]);format(lines[14],256,"%s",line);}}fclose(lf);}
        if(count==0)return SendClientMessage(playerid,COLOR_WARNING,"[LOGS] Nenhum log encontrado.");
        for(new k=0;k<count;k++){strcat(content,lines[k],sizeof content);strcat(content,"\n",sizeof content);}
        ShowPlayerDialog(playerid,NYX_ADMIN_LOG_DIALOG,DIALOG_STYLE_LIST,"NYX | LOGS ADMINISTRATIVOS",content,"FECHAR","");return 1;
    }

    if(strfind(cmdtext,"/ban ",true)==0)
    {
        if(NYX_Player[playerid][NYX_Admin]<2)return SendClientMessage(playerid,COLOR_ERROR,"[ERRO] Admin Assistente ou superior.");
        format(rest,sizeof rest,"%s",cmdtext[5]);id=strval(rest);new p1=strfind(rest," ",true);if(p1<1)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /ban [id] [dias 0=permanente] [motivo]");days=strval(rest[p1+1]);new p2=strfind(rest," ",true,p1+1);if(p2<0)return SendClientMessage(playerid,COLOR_WARNING,"Uso: /ban [id] [dias] [motivo]");format(reason,sizeof reason,"%s",rest[p2+1]);
        if(!IsPlayerConnected(id)||days<0||days>3650)return SendClientMessage(playerid,COLOR_ERROR,"Parametros invalidos.");
        if(NYX_Player[id][NYX_Admin]>=NYX_Player[playerid][NYX_Admin]&&NYX_Player[playerid][NYX_Admin]<100)return SendClientMessage(playerid,COLOR_ERROR,"Voce nao pode banir cargo igual ou superior.");
        GetPlayerName(id,name,sizeof name);new ip[46];GetPlayerIp(id,ip,sizeof ip);new path[128];format(path,sizeof path,"scriptfiles/nyx_ban_%s.dat",name);file=fopen(path,io_write);if(file){new banline[220];format(banline,sizeof banline,"%d|%d|%s|%s|%s",gettime(),days,ip,name,reason);fwrite(file,banline);fclose(file);}format(msg,sizeof msg,"[BAN] %s foi banido. Duracao: %s. Motivo: %s",name,days>0?"temporario":"permanente",reason);SendAdminMessage(COLOR_ERROR,msg);NYX_AdminExtrasLogLine(playerid,id,"/ban",reason);SetTimerEx("NYX_MigratedDelayedKick",100,false,"i",id);return 1;
    }

    if(strfind(cmdtext,"/criarcarro ",true)==0||strfind(cmdtext,"/v ",true)==0)
    {
        if(NYX_Player[playerid][NYX_Admin]<3)return SendClientMessage(playerid,COLOR_ERROR,"[ERRO] Admin 2 ou superior.");start=(strfind(cmdtext,"/v ",true)==0)?3:12;model=strval(cmdtext[start]);if(model<400||model>611)return SendClientMessage(playerid,COLOR_ERROR,"Modelo deve ser 400-611.");GetPlayerPos(playerid,x,y,z);GetPlayerFacingAngle(playerid,a);new veh=CreateVehicle(model,x+2.0,y,z,a,-1,-1,-1);LinkVehicleToInterior(veh,GetPlayerInterior(playerid));SetVehicleVirtualWorld(veh,GetPlayerVirtualWorld(playerid));PutPlayerInVehicle(playerid,veh,0);format(msg,sizeof msg,"[ADM] Veiculo %d criado (ID %d).",model,veh);SendClientMessage(playerid,COLOR_SUCCESS,msg);NYX_AdminExtrasLogLine(playerid,INVALID_PLAYER_ID,"/v",msg);return 1;
    }

    return 0;
}

forward NYX_ExecuteGMX();
public NYX_ExecuteGMX(){SendRconCommand("gmx");return 1;}
