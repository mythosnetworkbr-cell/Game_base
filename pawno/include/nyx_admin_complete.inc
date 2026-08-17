#if defined _NYX_ADMIN_COMPLETE_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_COMPLETE_INCLUDED
#define NYX_ADMIN_SUPPORT 1
#define NYX_ADMIN_ASSIST 2
#define NYX_ADMIN_2 3
#define NYX_ADMIN_LEADER 4
#define NYX_ADMIN_MANAGER 5
#define NYX_ADMIN_ALL 6
#define NYX_ADMIN_STAFF 7
#define NYX_ADMIN_DEV 80
#define NYX_ADMIN_OWNER 100
#define NYX_AUTO_JAIL_SECONDS 1800
#define NYX_JAIL_X 264.6288
#define NYX_JAIL_Y 77.5742
#define NYX_JAIL_Z 1001.0391
#define NYX_JAIL_A 270.0
#define NYX_JAIL_INTERIOR 6
#define NYX_JAIL_WORLD 1
new bool:NYX_AdminGod[MAX_PLAYERS];
new bool:NYX_AdminInvisible[MAX_PLAYERS];
new bool:NYX_AdminFrozen[MAX_PLAYERS];
new NYX_AdminJail[MAX_PLAYERS];
new NYX_AdminWarnings[MAX_PLAYERS];
new NYX_AdminJailReason[MAX_PLAYERS][64];
new NYX_AdminTimer;
stock bool:NYX_AdminHas(playerid,level){return NYX_Player[playerid][NYX_Admin]>=level;}
stock NYX_AdminDeny(playerid){return SendClientMessage(playerid,COLOR_ERROR,"NYX: voce nao possui permissao para este comando.");}
stock NYX_AdminFirstId(const text[],&id){if(text[0]==EOS)return 0;id=strval(text);return id>=0&&id<MAX_PLAYERS;}
stock NYX_AdminTail(const text[],out[],out_size){new p=strfind(text," ",true);if(p<0){out[0]=EOS;return 0;}format(out,out_size,"%s",text[p+1]);return 1;}
stock NYX_AdminIdReason(const text[],&id,reason[],reason_size){new tail[128];if(!NYX_AdminFirstId(text,id))return 0;if(!NYX_AdminTail(text,tail,sizeof tail))return 0;format(reason,reason_size,"%s",tail);return 1;}
stock NYX_AdminIdMinutesReason(const text[],&id,&minutes,reason[],reason_size){new p1=strfind(text," ",true),rest[128],p2;if(p1<1)return 0;id=strval(text);if(id<0||id>=MAX_PLAYERS)return 0;format(rest,sizeof rest,"%s",text[p1+1]);p2=strfind(rest," ",true);if(p2<1)return 0;minutes=strval(rest);format(reason,reason_size,"%s",rest[p2+1]);return 1;}
stock NYX_AdminLog(playerid,targetid,const command[],const details[]){new path[96],line[256],adminName[MAX_PLAYER_NAME],targetName[MAX_PLAYER_NAME],File:file;GetPlayerName(playerid,adminName,sizeof adminName);if(targetid!=INVALID_PLAYER_ID&&IsPlayerConnected(targetid))GetPlayerName(targetid,targetName,sizeof targetName);else format(targetName,sizeof targetName,"SERVER");format(path,sizeof path,"scriptfiles/nyx_admin_logs.dat");file=fopen(path,io_append);if(!file)return 0;format(line,sizeof line,"%d | %s | %s | %s | %s",gettime(),adminName,targetName,command,details);fwrite(file,line);fclose(file);return 1;}
stock NYX_AdminSave(playerid){NYX_SaveAccount(playerid);return 1;}
stock NYX_AdminJailPlayer(targetid,seconds,const reason[]){NYX_AdminJail[targetid]=seconds;format(NYX_AdminJailReason[targetid],64,"%s",reason);SetPlayerPos(targetid,NYX_JAIL_X,NYX_JAIL_Y,NYX_JAIL_Z);SetPlayerFacingAngle(targetid,NYX_JAIL_A);SetPlayerInterior(targetid,NYX_JAIL_INTERIOR);SetPlayerVirtualWorld(targetid,NYX_JAIL_WORLD);ResetPlayerWeapons(targetid);TogglePlayerControllable(targetid,1);return 1;}
stock NYX_AdminReleaseJail(targetid){NYX_AdminJail[targetid]=0;NYX_AdminJailReason[targetid][0]=EOS;SetPlayerInterior(targetid,0);SetPlayerVirtualWorld(targetid,0);SetPlayerPos(targetid,NYX_SPAWN_X,NYX_SPAWN_Y,NYX_SPAWN_Z);TogglePlayerControllable(targetid,1);return 1;}
stock NYX_AdminInit(){NYX_AdminTimer=SetTimer("NYX_AdminSecondTick",1000,true);for(new i=0;i<MAX_PLAYERS;i++){NYX_AdminGod[i]=false;NYX_AdminInvisible[i]=false;NYX_AdminFrozen[i]=false;NYX_AdminJail[i]=0;NYX_AdminWarnings[i]=0;NYX_AdminJailReason[i][0]=EOS;}return 1;}
stock NYX_AdminShutdown(){if(NYX_AdminTimer)KillTimer(NYX_AdminTimer);NYX_AdminTimer=0;return 1;}
stock NYX_AdminConnect(playerid){NYX_AdminGod[playerid]=false;NYX_AdminInvisible[playerid]=false;NYX_AdminFrozen[playerid]=false;NYX_AdminJail[playerid]=0;NYX_AdminWarnings[playerid]=0;NYX_AdminJailReason[playerid][0]=EOS;return 1;}
stock NYX_AdminDisconnect(playerid){NYX_AdminGod[playerid]=false;NYX_AdminInvisible[playerid]=false;NYX_AdminFrozen[playerid]=false;NYX_AdminJail[playerid]=0;NYX_AdminWarnings[playerid]=0;NYX_AdminJailReason[playerid][0]=EOS;return 1;}

// Os comandos enviados no chat foram migrados para nyx_admin_chat_migrated.inc.
stock NYX_AdminCommand(playerid,cmdtext[]){return 0;}

forward NYX_AdminSecondTick();
public NYX_AdminSecondTick(){for(new i=0;i<MAX_PLAYERS;i++){if(!IsPlayerConnected(i)||!NYX_Player[i][NYX_Logged])continue;if(NYX_AdminJail[i]>0){NYX_AdminJail[i]--;if(GetPlayerWeapon(i)>0)ResetPlayerWeapons(i);if(NYX_AdminJail[i]<=0){NYX_AdminReleaseJail(i);SendClientMessage(i,COLOR_SUCCESS,"[PRISAO]: Sua pena terminou. Voce foi liberado.");NYX_AdminSave(i);}}}return 1;}
stock NYX_AdminTakeDamage(playerid){if(NYX_AdminGod[playerid]){SetPlayerHealth(playerid,100.0);SetPlayerArmour(playerid,100.0);return 1;}if(NYX_AdminJail[playerid]>0){ResetPlayerWeapons(playerid);return 1;}return 0;}
stock NYX_AdminGiveDamage(playerid,damagedid){if(NYX_AdminJail[playerid]>0||(damagedid!=INVALID_PLAYER_ID&&NYX_AdminJail[damagedid]>0))return 1;if(damagedid!=INVALID_PLAYER_ID&&NYX_AdminGod[damagedid])return 1;return 0;}
