#if defined _NYX_MEDICAL_INCLUDED
    #endinput
#endif
#define _NYX_MEDICAL_INCLUDED

#define NYX_MEDICAL_HOSPITAL_X 1520.0
#define NYX_MEDICAL_HOSPITAL_Y -1675.0
#define NYX_MEDICAL_HOSPITAL_Z 15.0
#define NYX_MEDICAL_MAX_HEALTH 100.0
#define NYX_MEDICAL_TREATMENT_COST 250
#define NYX_MEDICAL_MAX_INJURY 100

new bool:NYX_MedicalDead[MAX_PLAYERS];
new NYX_MedicalInjury[MAX_PLAYERS];
new NYX_MedicalCooldown[MAX_PLAYERS];

stock NYX_MedicalInitPlayer(playerid)
{
    NYX_MedicalDead[playerid]=false;
    NYX_MedicalInjury[playerid]=0;
    NYX_MedicalCooldown[playerid]=0;
    return 1;
}

stock NYX_MedicalTick(playerid)
{
    if(!IsPlayerConnected(playerid)) return 0;
    if(NYX_MedicalCooldown[playerid]>0) NYX_MedicalCooldown[playerid]--;
    return 1;
}

stock NYX_MedicalOpen(playerid)
{
    new msg[192];
    format(msg,sizeof msg,"NYX | HOSPITAL | Estado: %s | Ferimentos: %d/%d | Tratamento: $%d",NYX_MedicalDead[playerid]?"INCONSCIENTE":"ESTAVEL",NYX_MedicalInjury[playerid],NYX_MEDICAL_MAX_INJURY,NYX_MEDICAL_TREATMENT_COST);
    return SendClientMessage(playerid,COLOR_WHITE,msg);
}

stock NYX_MedicalHeal(playerid,targetid)
{
    if(!IsPlayerConnected(targetid)) return SendClientMessage(playerid,COLOR_ERROR,"Paciente invalido.");
    if(GetPlayerDistanceFromPoint(playerid,NYX_MEDICAL_HOSPITAL_X,NYX_MEDICAL_HOSPITAL_Y,NYX_MEDICAL_HOSPITAL_Z)>80.0)
        return SendClientMessage(playerid,COLOR_WARNING,"Voce precisa estar no Hospital NYX.");
    if(NYX_MedicalCooldown[targetid]>0) return SendClientMessage(playerid,COLOR_WARNING,"Este paciente precisa aguardar o proximo atendimento.");
    if(GetPlayerMoney(targetid)<NYX_MEDICAL_TREATMENT_COST)
        return SendClientMessage(targetid,COLOR_ERROR,"Voce precisa de $250 para o tratamento.");
    GivePlayerMoney(targetid,-NYX_MEDICAL_TREATMENT_COST);
    NYX_SyncMoney(targetid);
    SetPlayerHealth(targetid,NYX_MEDICAL_MAX_HEALTH);
    NYX_MedicalDead[targetid]=false;
    NYX_MedicalInjury[targetid]=0;
    NYX_MedicalCooldown[targetid]=10;
    NYX_SaveAccount(targetid);
    SendClientMessage(targetid,COLOR_SUCCESS,"Tratamento concluido no Hospital NYX. Custo: $250.");
    if(playerid!=targetid) SendClientMessage(playerid,COLOR_SUCCESS,"Atendimento medico concluido.");
    return 1;
}

stock NYX_MedicalSetInjury(playerid)
{
    new Float:health; GetPlayerHealth(playerid,health);
    NYX_MedicalInjury[playerid]=floatround(100.0-health,floatround_ceil);
    if(NYX_MedicalInjury[playerid]<0) NYX_MedicalInjury[playerid]=0;
    if(NYX_MedicalInjury[playerid]>NYX_MEDICAL_MAX_INJURY) NYX_MedicalInjury[playerid]=NYX_MEDICAL_MAX_INJURY;
    return 1;
}

stock NYX_MedicalRespawn(playerid)
{
    NYX_MedicalDead[playerid]=false;
    NYX_MedicalInjury[playerid]=0;
    SetPlayerPos(playerid,NYX_MEDICAL_HOSPITAL_X,NYX_MEDICAL_HOSPITAL_Y,NYX_MEDICAL_HOSPITAL_Z);
    SetPlayerHealth(playerid,NYX_MEDICAL_MAX_HEALTH);
    ResetPlayerMoney(playerid);
    GivePlayerMoney(playerid,NYX_Player[playerid][NYX_Money]);
    SendClientMessage(playerid,COLOR_SUCCESS,"Voce foi encaminhado ao Hospital NYX.");
    return 1;
}

stock NYX_MedicalCommands(playerid,cmdtext[])
{
    if(!strcmp(cmdtext,"/hospital",true))
    {
        SetPlayerCheckpoint(playerid,NYX_MEDICAL_HOSPITAL_X,NYX_MEDICAL_HOSPITAL_Y,NYX_MEDICAL_HOSPITAL_Z,4.0);
        return SendClientMessage(playerid,COLOR_SUCCESS,"GPS marcado para o Hospital NYX.");
    }
    if(!strcmp(cmdtext,"/estado",true)) return NYX_MedicalOpen(playerid);
    if(!strcmp(cmdtext,"/curar",true)) return NYX_MedicalHeal(playerid,playerid);
    return 0;
}
