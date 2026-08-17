#if defined _NYX_INTEGRATED_SYSTEMS_INCLUDED
    #endinput
#endif
#define _NYX_INTEGRATED_SYSTEMS_INCLUDED

/*
    NYX ROLEPLAY - Integrated systems
    Adapted to the existing NYX SQLite/account architecture.
    No second OnPlayerCommandText/OnPlayerDisconnect/OnPlayerEnterCheckpoint
    callbacks are defined here; the master GM calls the dispatcher/hooks.
*/

#define NYX_LEVEL_SUPORTE             1
#define NYX_LEVEL_MODERADOR           2
#define NYX_LEVEL_ADMIN               3
#define NYX_LEVEL_DIRETOR             4
#define NYX_LEVEL_OWNER               5
#define NYX_PAYDAY_INTERVAL           3600
#define NYX_BASE_SALARY               2500
#define NYX_FIRETRUCK_MODEL           407
#define NYX_FIRE_REWARD               1500
#define NYX_MAX_ADMIN_VEHICLES        100
#define NYX_MAX_FIRE_CALLS            50
#define NYX_MAX_IFOOD_ORDERS          50
#define NYX_MAX_UBER_CALLS            50
#define NYX_JAIL_X                    264.6288
#define NYX_JAIL_Y                    77.5742
#define NYX_JAIL_Z                    1001.0391
#define NYX_JAIL_INTERIOR             6
#define NYX_JAIL_WORLD                1

new bool:NYX_GodMode[MAX_PLAYERS];
new bool:NYX_Invisible[MAX_PLAYERS];
new bool:NYX_FireDuty[MAX_PLAYERS];
new bool:NYX_Frozen[MAX_PLAYERS];
new NYX_Bank[MAX_PLAYERS];
new NYX_PaydaySeconds[MAX_PLAYERS];
new NYX_JailSeconds[MAX_PLAYERS];
new NYX_JailReason[MAX_PLAYERS][64];
new bool:NYX_Spectating[MAX_PLAYERS];
new Float:NYX_SpecPos[MAX_PLAYERS][3];
new NYX_SpecInterior[MAX_PLAYERS];
new NYX_SpecVW[MAX_PLAYERS];
new NYX_AdminVehicle[MAX_PLAYERS][NYX_MAX_ADMIN_VEHICLES];
new NYX_AdminVehicleLocked[MAX_PLAYERS][NYX_MAX_ADMIN_VEHICLES];
new NYX_AdminVehicleCount[MAX_PLAYERS];

new NYX_FireObject;
new Float:NYX_FireX, Float:NYX_FireY, Float:NYX_FireZ;
new bool:NYX_FireActive;
new NYX_PaydayTimer;
new NYX_FireTimer;

// 193 emergency calls
enum E_NYX_FIRE_CALL
{
    NYX_FC_Caller,
    Float:NYX_FC_X,
    Float:NYX_FC_Y,
    Float:NYX_FC_Z,
    NYX_FC_Description[128],
    bool:NYX_FC_Active
};
new NYX_FireCalls[NYX_MAX_FIRE_CALLS][E_NYX_FIRE_CALL];

// iFood
enum E_NYX_IFOOD
{
    NYX_IF_Customer,
    NYX_IF_Item[32],
    NYX_IF_Price,
    Float:NYX_IF_X,
    Float:NYX_IF_Y,
    Float:NYX_IF_Z,
    bool:NYX_IF_Active
};
new NYX_IFood[NYX_MAX_IFOOD_ORDERS][E_NYX_IFOOD];
new bool:NYX_IFoodDuty[MAX_PLAYERS];
new NYX_ActiveIFood[MAX_PLAYERS];

// Bus
enum E_NYX_BUS_STOP
{
    Float:NYX_BUS_X,
    Float:NYX_BUS_Y,
    Float:NYX_BUS_Z
};
new Float:NYX_BusStops[][3] =
{
    {1162.123, -1770.432, 13.554},
    {1480.551, -1750.100, 15.421},
    {2104.123, -1805.321, 13.554},
    {2240.220, -1160.110, 25.800}
};
new bool:NYX_BusDriver[MAX_PLAYERS];
new NYX_BusStep[MAX_PLAYERS];

// Uber
enum E_NYX_UBER
{
    NYX_UB_Passenger,
    Float:NYX_UB_X,
    Float:NYX_UB_Y,
    Float:NYX_UB_Z,
    bool:NYX_UB_Active
};
new NYX_Uber[NYX_MAX_UBER_CALLS][E_NYX_UBER];
new bool:NYX_UberDriver[MAX_PLAYERS];
new NYX_ActiveUber[MAX_PLAYERS];

stock NYX_GetAdminRank(level, output[], size)
{
    switch(level)
    {
        case NYX_LEVEL_SUPORTE: format(output, size, "Suporte");
        case NYX_LEVEL_MODERADOR: format(output, size, "Moderador");
        case NYX_LEVEL_ADMIN: format(output, size, "Administrador");
        case NYX_LEVEL_DIRETOR: format(output, size, "Diretor");
        case NYX_LEVEL_OWNER: format(output, size, "Dono");
        default: format(output, size, "Jogador");
    }
    return 1;
}

stock NYX_IsAdmin(playerid, level)
{
    return NYX_Player[playerid][NYX_Admin] >= level;
}

stock NYX_IntegratedInit()
{
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        NYX_Bank[i] = 0;
        NYX_PaydaySeconds[i] = 0;
        NYX_JailSeconds[i] = 0;
        NYX_JailReason[i][0] = EOS;
        NYX_GodMode[i] = false;
        NYX_Invisible[i] = false;
        NYX_FireDuty[i] = false;
        NYX_Frozen[i] = false;
        NYX_Spectating[i] = false;
        NYX_IFoodDuty[i] = false;
        NYX_ActiveIFood[i] = -1;
        NYX_BusDriver[i] = false;
        NYX_BusStep[i] = 0;
        NYX_UberDriver[i] = false;
        NYX_ActiveUber[i] = -1;
        for(new j = 0; j < NYX_MAX_ADMIN_VEHICLES; j++)
        {
            NYX_AdminVehicle[i][j] = INVALID_VEHICLE_ID;
            NYX_AdminVehicleLocked[i][j] = 0;
        }
    }
    NYX_FireActive = false;
    NYX_FireObject = INVALID_OBJECT_ID;
    NYX_PaydayTimer = SetTimer("NYX_IntegratedSecond", 1000, true);
    NYX_FireTimer = SetTimer("NYX_IntegratedRandomFire", 300000, true);
    if(NYX_DBReady)
    {
        db_query(NYX_DB, "CREATE TABLE IF NOT EXISTS nyx_integrated (name TEXT PRIMARY KEY, bank INTEGER DEFAULT 0, payday INTEGER DEFAULT 0, jail INTEGER DEFAULT 0, jail_reason TEXT DEFAULT '')");
    }
    return 1;
}

stock NYX_IntegratedExit()
{
    if(NYX_PaydayTimer) KillTimer(NYX_PaydayTimer);
    if(NYX_FireTimer) KillTimer(NYX_FireTimer);
    NYX_PaydayTimer = 0;
    NYX_FireTimer = 0;
    if(NYX_FireActive && NYX_FireObject != INVALID_OBJECT_ID) DestroyObject(NYX_FireObject);
    NYX_FireActive = false;
    return 1;
}

stock NYX_IntegratedLoad(playerid)
{
    if(!NYX_DBReady) return 0;
    new name[MAX_PLAYER_NAME + 1], esc[64], q[256];
    GetPlayerName(playerid, name, sizeof name);
    NYX_SQLQuote(name, esc, sizeof esc);
    format(q, sizeof q, "SELECT bank,payday,jail,jail_reason FROM nyx_integrated WHERE name='%s' LIMIT 1", esc);
    new DBResult:r = db_query(NYX_DB, q);
    if(r == DBResult:0) return 0;
    if(db_num_rows(r) > 0)
    {
        NYX_Bank[playerid] = db_get_field_assoc_int(r, "bank");
        NYX_PaydaySeconds[playerid] = db_get_field_assoc_int(r, "payday");
        NYX_JailSeconds[playerid] = db_get_field_assoc_int(r, "jail");
        db_get_field_assoc(r, "jail_reason", NYX_JailReason[playerid], sizeof NYX_JailReason[]);
    }
    db_free_result(r);
    return 1;
}

stock NYX_IntegratedSave(playerid)
{
    if(!NYX_DBReady) return 0;
    new name[MAX_PLAYER_NAME + 1], esc[64], reason[160], q[512];
    GetPlayerName(playerid, name, sizeof name);
    NYX_SQLQuote(name, esc, sizeof esc);
    NYX_SQLQuote(NYX_JailReason[playerid], reason, sizeof reason);
    format(q, sizeof q, "INSERT OR REPLACE INTO nyx_integrated (name,bank,payday,jail,jail_reason) VALUES ('%s',%d,%d,%d,'%s')", esc, NYX_Bank[playerid], NYX_PaydaySeconds[playerid], NYX_JailSeconds[playerid], reason);
    new DBResult:r = db_query(NYX_DB, q);
    if(r != DBResult:0) db_free_result(r);
    return r != DBResult:0;
}

stock NYX_IntegratedReset(playerid)
{
    NYX_Bank[playerid] = 0;
    NYX_PaydaySeconds[playerid] = 0;
    NYX_JailSeconds[playerid] = 0;
    NYX_JailReason[playerid][0] = EOS;
    NYX_GodMode[playerid] = false;
    NYX_Invisible[playerid] = false;
    NYX_FireDuty[playerid] = false;
    NYX_Frozen[playerid] = false;
    NYX_Spectating[playerid] = false;
    NYX_IFoodDuty[playerid] = false;
    NYX_ActiveIFood[playerid] = -1;
    NYX_BusDriver[playerid] = false;
    NYX_BusStep[playerid] = 0;
    NYX_UberDriver[playerid] = false;
    NYX_ActiveUber[playerid] = -1;
    return 1;
}

stock NYX_AdminMessage(color, const message[])
{
    for(new i = 0; i < MAX_PLAYERS; i++)
        if(IsPlayerConnected(i) && NYX_Player[i][NYX_Admin] >= NYX_LEVEL_SUPORTE) SendClientMessage(i, color, message);
    return 1;
}

stock NYX_IntegratedKick(playerid, targetid, const reason[])
{
    new an[MAX_PLAYER_NAME], tn[MAX_PLAYER_NAME], msg[180];
    GetPlayerName(playerid, an, sizeof an);
    GetPlayerName(targetid, tn, sizeof tn);
    format(msg, sizeof msg, "[ADM] %s foi kickado por %s. Motivo: %s", tn, an, reason);
    SendClientMessageToAll(0xFF4444FF, msg);
    SetTimerEx("NYX_DelayedKick", 100, false, "i", targetid);
    return 1;
}

forward NYX_DelayedKick(playerid);
public NYX_DelayedKick(playerid)
{
    if(IsPlayerConnected(playerid)) Kick(playerid);
    return 1;
}

forward NYX_IntegratedSecond();
public NYX_IntegratedSecond()
{
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i) || IsPlayerNPC(i)) continue;
        if(NYX_JailSeconds[i] > 0)
        {
            NYX_JailSeconds[i]--;
            if(NYX_JailSeconds[i] == 0)
            {
                NYX_JailReason[i][0] = EOS;
                SpawnPlayer(i);
                SendClientMessage(i, 0x00FF00FF, "[PRISAO] Sua pena terminou.");
            }
        }
        if(NYX_Player[i][NYX_Logged])
        {
            NYX_PaydaySeconds[i]++;
            if(NYX_PaydaySeconds[i] >= NYX_PAYDAY_INTERVAL)
            {
                NYX_PaydaySeconds[i] = 0;
                new salary = NYX_BASE_SALARY;
                if(NYX_Player[i][NYX_Admin] > 0) salary += 500;
                NYX_Bank[i] += salary;
                PlayerPlaySound(i, 1058, 0.0, 0.0, 0.0);
                new msg[144];
                format(msg, sizeof msg, "[PAYDAY] Salario: R$ %d | Banco: R$ %d", salary, NYX_Bank[i]);
                SendClientMessage(i, 0x00FF00FF, msg);
                NYX_IntegratedSave(i);
            }
        }
    }
    return 1;
}

forward NYX_IntegratedRandomFire();
public NYX_IntegratedRandomFire()
{
    if(NYX_FireActive) return 1;
    new Float:locs[][3] = {
        {1813.432, -1882.112, 13.414},
        {2104.123, -1805.321, 13.554},
        {1420.551, -1700.100, 13.546}
    };
    new r = random(sizeof locs);
    NYX_FireX = locs[r][0]; NYX_FireY = locs[r][1]; NYX_FireZ = locs[r][2];
    NYX_FireObject = CreateObject(18688, NYX_FireX, NYX_FireY, NYX_FireZ - 2.0, 0.0, 0.0, 0.0);
    NYX_FireActive = true;
    for(new i = 0; i < MAX_PLAYERS; i++)
    {
        if(IsPlayerConnected(i) && NYX_FireDuty[i])
        {
            SendClientMessage(i, 0xFF3333FF, "[193] Novo incendio. Use /chamadosbombeiro para localizar.");
            PlayerPlaySound(i, 1058, 0.0, 0.0, 0.0);
        }
    }
    return 1;
}

stock NYX_CompleteFire(playerid)
{
    if(!NYX_FireActive) return 0;
    if(!NYX_FireDuty[playerid]) return 0;
    if(!IsPlayerInRangeOfPoint(playerid, 5.0, NYX_FireX, NYX_FireY, NYX_FireZ)) return 0;
    new veh = GetPlayerVehicleID(playerid);
    new weapon = GetPlayerWeapon(playerid);
    if(weapon != 42 && !(veh != 0 && GetVehicleModel(veh) == NYX_FIRETRUCK_MODEL)) return 0;
    if(NYX_FireObject != INVALID_OBJECT_ID) DestroyObject(NYX_FireObject);
    NYX_FireObject = INVALID_OBJECT_ID;
    NYX_FireActive = false;
    DisablePlayerCheckpoint(playerid);
    NYX_Bank[playerid] += NYX_FIRE_REWARD;
    NYX_IntegratedSave(playerid);
    new msg[128]; format(msg, sizeof msg, "[BOMBEIRO] Incendio apagado. R$ %d depositados no banco.", NYX_FIRE_REWARD);
    SendClientMessage(playerid, 0x00FF00FF, msg);
    return 1;
}

stock NYX_ShowIFoodMenu(playerid)
{
    new list[256];
    format(list, sizeof list, "Item\tPreco\nHamburguer X-Tudo\tR$ 25\nPizza Grande\tR$ 45\nMarmita Executiva\tR$ 20\nRefrigerante LATA\tR$ 8");
    return ShowPlayerDialog(playerid, 8950, DIALOG_STYLE_TABLIST_HEADERS, "iFood | Pedido", list, "Pedir", "Cancelar");
}

stock NYX_ProcessIFoodDialog(playerid, response, listitem)
{
    if(!response) return 1;
    new item[32], price;
    switch(listitem)
    {
        case 0: { format(item, sizeof item, "Hamburguer X-Tudo"); price = 25; }
        case 1: { format(item, sizeof item, "Pizza Grande"); price = 45; }
        case 2: { format(item, sizeof item, "Marmita Executiva"); price = 20; }
        case 3: { format(item, sizeof item, "Refrigerante LATA"); price = 8; }
        default: return 1;
    }
    if(GetPlayerMoney(playerid) < price) return SendClientMessage(playerid, 0xFF3333FF, "[iFood] Dinheiro em maos insuficiente.");
    new slot = -1;
    for(new i = 0; i < NYX_MAX_IFOOD_ORDERS; i++) if(!NYX_IFood[i][NYX_IF_Active]) { slot = i; break; }
    if(slot == -1) return SendClientMessage(playerid, 0xFF3333FF, "[iFood] Central lotada.");
    GivePlayerMoney(playerid, -price);
    GetPlayerPos(playerid, NYX_IFood[slot][NYX_IF_X], NYX_IFood[slot][NYX_IF_Y], NYX_IFood[slot][NYX_IF_Z]);
    NYX_IFood[slot][NYX_IF_Customer] = playerid;
    format(NYX_IFood[slot][NYX_IF_Item], 32, "%s", item);
    NYX_IFood[slot][NYX_IF_Price] = price;
    NYX_IFood[slot][NYX_IF_Active] = true;
    SendClientMessage(playerid, 0x00FF00FF, "[iFood] Pedido confirmado. Aguarde um entregador.");
    new name[MAX_PLAYER_NAME], msg[180]; GetPlayerName(playerid, name, sizeof name);
    format(msg, sizeof msg, "[iFood] Pedido #%d | Cliente: %s | Item: %s | R$ %d", slot, name, item, price);
    for(new i = 0; i < MAX_PLAYERS; i++) if(IsPlayerConnected(i) && NYX_IFoodDuty[i]) SendClientMessage(i, 0xFFFF00FF, msg);
    return 1;
}

stock NYX_ShowHelp(playerid)
{
    SendClientMessage(playerid, 0x8B5CF6FF, "NYX | Admin: /limparchat /invisivel /godmode /ir /traz /spec /specoff");
    SendClientMessage(playerid, 0xFFFFFFFF, "NYX | Punições: /kick /ban /jail /unjail /freeze /unfreeze /slap /setadmin");
    SendClientMessage(playerid, 0xFFFFFFFF, "NYX | Jobs: /trabalharbombeiro /193 /atender193 /pedirifood /trabalharifood /atenderifood /entregarcomida");
    SendClientMessage(playerid, 0xFFFFFFFF, "NYX | Transporte: /iniciarrota /pararrota /chamaruber /trabalharuber /atenderuber /finalizaruber");
    return 1;
}

stock NYX_ParseCommand(const cmdtext[], command[], size, params[], paramsSize)
{
    command[0] = EOS; params[0] = EOS;
    if(cmdtext[0] != '/') return 0;
    new i = 1, j = 0;
    while(cmdtext[i] != EOS && cmdtext[i] != ' ' && j < size - 1) command[j++] = cmdtext[i++];
    command[j] = EOS;
    while(cmdtext[i] == ' ') i++;
    j = 0;
    while(cmdtext[i] != EOS && j < paramsSize - 1) params[j++] = cmdtext[i++];
    params[j] = EOS;
    return 1;
}

stock NYX_GetTargetFromParams(const params[], &targetid)
{
    if(params[0] == EOS) return 0;
    targetid = strval(params);
    return IsPlayerConnected(targetid);
}

stock NYX_IntegratedCommand(playerid, cmdtext[])
{
    new cmd[32], params[160];
    if(!NYX_ParseCommand(cmdtext, cmd, sizeof cmd, params, sizeof params)) return 0;

    if(!strcmp(cmd, "ajuda", true) || !strcmp(cmd, "help", true)) return NYX_ShowHelp(playerid);
    if(!strcmp(cmd, "limparchat", true) || !strcmp(cmd, "cc", true))
    {
        if(!NYX_IsAdmin(playerid, NYX_LEVEL_SUPORTE)) return SendClientMessage(playerid, 0xFF3333FF, "[ERRO] Sem permissao.");
        for(new i = 0; i < 100; i++) SendClientMessageToAll(-1, " ");
        new n[MAX_PLAYER_NAME], rank[32], msg[144]; GetPlayerName(playerid,n,sizeof n); NYX_GetAdminRank(NYX_Player[playerid][NYX_Admin],rank,sizeof rank);
        format(msg,sizeof msg,"[ADMIN] %s %s limpou o chat.",rank,n); SendClientMessageToAll(0xFFFF00FF,msg); return 1;
    }
    if(!strcmp(cmd, "invisivel", true) || !strcmp(cmd, "inv", true))
    {
        if(!NYX_IsAdmin(playerid, NYX_LEVEL_SUPORTE)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Sem permissao.");
        NYX_Invisible[playerid] = !NYX_Invisible[playerid];
        SetPlayerColor(playerid, NYX_Invisible[playerid] ? 0xFFFFFF00 : 0xFFFFFFFF);
        SendClientMessage(playerid, NYX_Invisible[playerid] ? 0x00FF00FF : 0xFFFF00FF, NYX_Invisible[playerid] ? "[ADMIN] Invisibilidade ativada." : "[ADMIN] Invisibilidade desativada.");
        return 1;
    }
    if(!strcmp(cmd, "godmode", true) || !strcmp(cmd, "god", true))
    {
        if(!NYX_IsAdmin(playerid, NYX_LEVEL_SUPORTE)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Sem permissao.");
        NYX_GodMode[playerid] = !NYX_GodMode[playerid];
        if(NYX_GodMode[playerid]) { SetPlayerHealth(playerid,100.0); SetPlayerArmour(playerid,100.0); }
        SendClientMessage(playerid, NYX_GodMode[playerid] ? 0x00FF00FF : 0xFFFF00FF, NYX_GodMode[playerid] ? "[ADMIN] GodMode ativado." : "[ADMIN] GodMode desativado."); return 1;
    }
    if(!strcmp(cmd,"ir",true) || !strcmp(cmd,"goto",true) || !strcmp(cmd,"traz",true) || !strcmp(cmd,"gethere",true))
    {
        if(!NYX_IsAdmin(playerid,NYX_LEVEL_SUPORTE)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Sem permissao.");
        new t; if(!NYX_GetTargetFromParams(params,t)) return SendClientMessage(playerid,0xFFFF00FF,"[USO] /ir [id] ou /traz [id]");
        new Float:x,Float:y,Float:z; GetPlayerPos(!strcmp(cmd,"traz",true)||!strcmp(cmd,"gethere",true)?playerid:t,x,y,z);
        if(!strcmp(cmd,"traz",true)||!strcmp(cmd,"gethere",true)) { SetPlayerPos(t,x+1.0,y+1.0,z); SetPlayerInterior(t,GetPlayerInterior(playerid)); SetPlayerVirtualWorld(t,GetPlayerVirtualWorld(playerid)); }
        else { SetPlayerPos(playerid,x+1.5,y+1.5,z); SetPlayerInterior(playerid,GetPlayerInterior(t)); SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(t)); }
        return 1;
    }
    if(!strcmp(cmd,"spec",true) || !strcmp(cmd,"sp",true))
    {
        if(!NYX_IsAdmin(playerid,NYX_LEVEL_SUPORTE)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Sem permissao.");
        new t; if(!NYX_GetTargetFromParams(params,t)) return SendClientMessage(playerid,0xFFFF00FF,"[USO] /spec [id]");
        GetPlayerPos(playerid,NYX_SpecPos[playerid][0],NYX_SpecPos[playerid][1],NYX_SpecPos[playerid][2]);
        NYX_SpecInterior[playerid]=GetPlayerInterior(playerid); NYX_SpecVW[playerid]=GetPlayerVirtualWorld(playerid); NYX_Spectating[playerid]=true;
        TogglePlayerSpectating(playerid,1); SetPlayerInterior(playerid,GetPlayerInterior(t)); SetPlayerVirtualWorld(playerid,GetPlayerVirtualWorld(t));
        if(IsPlayerInAnyVehicle(t)) PlayerSpectateVehicle(playerid,GetPlayerVehicleID(t)); else PlayerSpectatePlayer(playerid,t); return 1;
    }
    if(!strcmp(cmd,"specoff",true) || !strcmp(cmd,"spoff",true))
    {
        if(!NYX_IsAdmin(playerid,NYX_LEVEL_SUPORTE)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Sem permissao.");
        TogglePlayerSpectating(playerid,0); NYX_Spectating[playerid]=false; SetPlayerPos(playerid,NYX_SpecPos[playerid][0],NYX_SpecPos[playerid][1],NYX_SpecPos[playerid][2]); SetPlayerInterior(playerid,NYX_SpecInterior[playerid]); SetPlayerVirtualWorld(playerid,NYX_SpecVW[playerid]); SpawnPlayer(playerid); return 1;
    }
    if(!strcmp(cmd,"kick",true) || !strcmp(cmd,"ban",true))
    {
        if(!NYX_IsAdmin(playerid,!strcmp(cmd,"ban",true)?NYX_LEVEL_DIRETOR:NYX_LEVEL_SUPORTE)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Sem permissao.");
        new t = strval(params), reason[96]; reason[0]=EOS; new p=0; while(params[p] && params[p]!=' ') p++; while(params[p]==' ') p++; format(reason,sizeof reason,"%s",params[p]?params[p]:"Sem motivo");
        if(!IsPlayerConnected(t)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Jogador offline.");
        if(!strcmp(cmd,"ban",true) && NYX_DBReady) { new tn[MAX_PLAYER_NAME],esc[64],q[256]; GetPlayerName(t,tn,sizeof tn); NYX_SQLQuote(tn,esc,sizeof esc); format(q,sizeof q,"CREATE TABLE IF NOT EXISTS nyx_bans (name TEXT PRIMARY KEY); INSERT OR REPLACE INTO nyx_bans(name) VALUES('%s')",esc); db_query(NYX_DB,q); }
        NYX_IntegratedKick(playerid,t,reason); return 1;
    }
    if(!strcmp(cmd,"jail",true))
    {
        if(!NYX_IsAdmin(playerid,NYX_LEVEL_SUPORTE)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Sem permissao.");
        new t=strval(params), pos=0; while(params[pos] && params[pos]!=' ') pos++; while(params[pos]==' ') pos++; new mins=strval(params[pos]); while(params[pos] && params[pos]!=' ') pos++; while(params[pos]==' ') pos++; new reason[64]; format(reason,sizeof reason,"%s",params[pos]?params[pos]:"Sem motivo");
        if(!IsPlayerConnected(t)||mins<1) return SendClientMessage(playerid,0xFFFF00FF,"[USO] /jail [id] [minutos] [motivo]");
        NYX_JailSeconds[t]=mins*60; format(NYX_JailReason[t],64,"%s",reason); SetPlayerPos(t,NYX_JAIL_X,NYX_JAIL_Y,NYX_JAIL_Z); SetPlayerInterior(t,NYX_JAIL_INTERIOR); SetPlayerVirtualWorld(t,NYX_JAIL_WORLD); ResetPlayerWeapons(t); NYX_IntegratedSave(t); SendClientMessage(t,0xFFAA00FF,"[PRISAO] Voce foi preso pela administracao."); return 1;
    }
    if(!strcmp(cmd,"unjail",true) || !strcmp(cmd,"freeze",true) || !strcmp(cmd,"unfreeze",true) || !strcmp(cmd,"slap",true))
    {
        if(!NYX_IsAdmin(playerid,NYX_LEVEL_SUPORTE)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Sem permissao.");
        new t; if(!NYX_GetTargetFromParams(params,t)) return SendClientMessage(playerid,0xFFFF00FF,"[USO] Informe o ID.");
        if(!strcmp(cmd,"unjail",true)) { NYX_JailSeconds[t]=0; NYX_JailReason[t][0]=EOS; SpawnPlayer(t); SendClientMessage(t,0x00FF00FF,"[PRISAO] Voce foi solto."); }
        else if(!strcmp(cmd,"freeze",true)) { NYX_Frozen[t]=true; TogglePlayerControllable(t,0); }
        else if(!strcmp(cmd,"unfreeze",true)) { NYX_Frozen[t]=false; TogglePlayerControllable(t,1); }
        else { new Float:x,Float:y,Float:z; GetPlayerPos(t,x,y,z); SetPlayerPos(t,x,y,z+5.0); }
        return 1;
    }
    if(!strcmp(cmd,"setadmin",true))
    {
        if(!NYX_IsAdmin(playerid,NYX_LEVEL_OWNER)) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Apenas Donos.");
        new t=strval(params), p=0; while(params[p] && params[p]!=' ') p++; while(params[p]==' ') p++; new level=strval(params[p]); if(!IsPlayerConnected(t)||level<0||level>NYX_LEVEL_OWNER) return SendClientMessage(playerid,0xFFFF00FF,"[USO] /setadmin [id] [0-5]");
        NYX_Player[t][NYX_Admin]=level; NYX_SaveAccount(t); new msg[128]; format(msg,sizeof msg,"[ADMIN] Nivel administrativo alterado para %d.",level); SendClientMessage(t,0x00FF00FF,msg); return 1;
    }
    if(!strcmp(cmd,"trabalharbombeiro",true))
    {
        NYX_FireDuty[playerid]=!NYX_FireDuty[playerid]; if(NYX_FireDuty[playerid]) SendClientMessage(playerid,0x00FF00FF,"[BOMBEIRO] Voce entrou em servico."); else { DisablePlayerCheckpoint(playerid); SendClientMessage(playerid,0xFFFF00FF,"[BOMBEIRO] Voce saiu de servico."); } return 1;
    }
    if(!strcmp(cmd,"chamadosbombeiro",true))
    {
        if(!NYX_FireDuty[playerid]) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Entre em servico primeiro.");
        if(!NYX_FireActive) return SendClientMessage(playerid,0xFFFF00FF,"[BOMBEIRO] Nenhum incendio ativo.");
        SetPlayerCheckpoint(playerid,NYX_FireX,NYX_FireY,NYX_FireZ,5.0); return SendClientMessage(playerid,0x00FF00FF,"[BOMBEIRO] Incendio marcado no GPS.");
    }
    if(!strcmp(cmd,"193",true) || !strcmp(cmd,"socorrobombeiro",true))
    {
        if(params[0]==EOS) return SendClientMessage(playerid,0xFFFF00FF,"[USO] /193 [descricao]");
        if(NYX_FireDuty[playerid]) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Bombeiros em servico nao podem acionar o 193.");
        new slot=-1; for(new i=0;i<NYX_MAX_FIRE_CALLS;i++) if(!NYX_FireCalls[i][NYX_FC_Active]) {slot=i;break;}
        if(slot==-1) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Central 193 lotada.");
        GetPlayerPos(playerid,NYX_FireCalls[slot][NYX_FC_X],NYX_FireCalls[slot][NYX_FC_Y],NYX_FireCalls[slot][NYX_FC_Z]); NYX_FireCalls[slot][NYX_FC_Caller]=playerid; format(NYX_FireCalls[slot][NYX_FC_Description],128,"%s",params); NYX_FireCalls[slot][NYX_FC_Active]=true;
        SendClientMessage(playerid,0x00FF00FF,"[193] Chamado registrado. Aguarde os bombeiros."); new name[MAX_PLAYER_NAME],msg[180]; GetPlayerName(playerid,name,sizeof name); format(msg,sizeof msg,"[CENTRAL 193] Chamado #%d | %s (ID %d) | %s",slot,name,playerid,params); for(new i=0;i<MAX_PLAYERS;i++) if(IsPlayerConnected(i)&&NYX_FireDuty[i]) SendClientMessage(i,0xFF3333FF,msg); return 1;
    }
    if(!strcmp(cmd,"atender193",true))
    {
        if(!NYX_FireDuty[playerid]) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Apenas bombeiros em servico."); new id=strval(params); if(id<0||id>=NYX_MAX_FIRE_CALLS||!NYX_FireCalls[id][NYX_FC_Active]) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Chamado invalido."); SetPlayerCheckpoint(playerid,NYX_FireCalls[id][NYX_FC_X],NYX_FireCalls[id][NYX_FC_Y],NYX_FireCalls[id][NYX_FC_Z],5.0); NYX_FireCalls[id][NYX_FC_Active]=false; return 1;
    }
    if(!strcmp(cmd,"pedirifood",true)) return NYX_ShowIFoodMenu(playerid);
    if(!strcmp(cmd,"trabalharifood",true)) { NYX_IFoodDuty[playerid]=!NYX_IFoodDuty[playerid]; if(!NYX_IFoodDuty[playerid]) { NYX_ActiveIFood[playerid]=-1; DisablePlayerCheckpoint(playerid); } SendClientMessage(playerid,NYX_IFoodDuty[playerid]?0x00FF00FF:0xFFFF00FF,NYX_IFoodDuty[playerid]?"[iFood] Voce entrou em servico.":"[iFood] Voce saiu de servico."); return 1; }
    if(!strcmp(cmd,"atenderifood",true))
    {
        if(!NYX_IFoodDuty[playerid]) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Nao esta em servico."); if(NYX_ActiveIFood[playerid]!=-1) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Ja possui entrega ativa."); new id=strval(params); if(id<0||id>=NYX_MAX_IFOOD_ORDERS||!NYX_IFood[id][NYX_IF_Active]) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Pedido invalido."); NYX_ActiveIFood[playerid]=id; SetPlayerCheckpoint(playerid,NYX_IFood[id][NYX_IF_X],NYX_IFood[id][NYX_IF_Y],NYX_IFood[id][NYX_IF_Z],3.0); return 1;
    }
    if(!strcmp(cmd,"entregarcomida",true))
    {
        new id=NYX_ActiveIFood[playerid]; if(!NYX_IFoodDuty[playerid]||id==-1) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Nenhuma entrega ativa."); if(!IsPlayerInRangeOfPoint(playerid,5.0,NYX_IFood[id][NYX_IF_X],NYX_IFood[id][NYX_IF_Y],NYX_IFood[id][NYX_IF_Z])) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Va ate o ponto marcado."); new reward=NYX_IFood[id][NYX_IF_Price]+15; GivePlayerMoney(playerid,reward); NYX_IFood[id][NYX_IF_Active]=false; NYX_ActiveIFood[playerid]=-1; DisablePlayerCheckpoint(playerid); new msg[128]; format(msg,sizeof msg,"[iFood] Entrega concluida. Voce recebeu R$ %d.",reward); SendClientMessage(playerid,0x00FF00FF,msg); return 1;
    }
    if(!strcmp(cmd,"iniciarrota",true) || !strcmp(cmd,"pararrota",true))
    {
        if(!strcmp(cmd,"pararrota",true)) { NYX_BusDriver[playerid]=false; DisablePlayerCheckpoint(playerid); return SendClientMessage(playerid,0xFFFF00FF,"[ROTA] Rota cancelada."); }
        new veh=GetPlayerVehicleID(playerid); if(veh==0||GetPlayerState(playerid)!=PLAYER_STATE_DRIVER) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Voce precisa dirigir um onibus."); new model=GetVehicleModel(veh); if(model!=431&&model!=437) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Veiculo nao e onibus."); NYX_BusDriver[playerid]=true; NYX_BusStep[playerid]=0; SetPlayerCheckpoint(playerid,NYX_BusStops[0][0],NYX_BusStops[0][1],NYX_BusStops[0][2],5.0); return SendClientMessage(playerid,0x00FF00FF,"[ROTA] Rota iniciada.");
    }
    if(!strcmp(cmd,"chamaruber",true))
    {
        if(NYX_UberDriver[playerid]) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Motoristas nao podem chamar Uber."); new slot=-1; for(new i=0;i<NYX_MAX_UBER_CALLS;i++) if(!NYX_Uber[i][NYX_UB_Active]) {slot=i;break;} if(slot==-1) return SendClientMessage(playerid,0xFF3333FF,"[UBER] Central ocupada."); GetPlayerPos(playerid,NYX_Uber[slot][NYX_UB_X],NYX_Uber[slot][NYX_UB_Y],NYX_Uber[slot][NYX_UB_Z]); NYX_Uber[slot][NYX_UB_Passenger]=playerid; NYX_Uber[slot][NYX_UB_Active]=true; SendClientMessage(playerid,0x00FF00FF,"[UBER] Solicitação enviada."); return 1;
    }
    if(!strcmp(cmd,"trabalharuber",true)) { NYX_UberDriver[playerid]=!NYX_UberDriver[playerid]; if(!NYX_UberDriver[playerid]) {NYX_ActiveUber[playerid]=-1;DisablePlayerCheckpoint(playerid);} return SendClientMessage(playerid,NYX_UberDriver[playerid]?0x00FF00FF:0xFFFF00FF,NYX_UberDriver[playerid]?"[UBER] Voce entrou em servico.":"[UBER] Voce saiu de servico."); }
    if(!strcmp(cmd,"atenderuber",true))
    {
        if(!NYX_UberDriver[playerid]) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Entre em servico."); if(NYX_ActiveUber[playerid]!=-1) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Ja possui corrida ativa."); new id=strval(params); if(id<0||id>=NYX_MAX_UBER_CALLS||!NYX_Uber[id][NYX_UB_Active]) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Corrida invalida."); NYX_ActiveUber[playerid]=id; SetPlayerCheckpoint(playerid,NYX_Uber[id][NYX_UB_X],NYX_Uber[id][NYX_UB_Y],NYX_Uber[id][NYX_UB_Z],4.0); return 1;
    }
    if(!strcmp(cmd,"finalizaruber",true))
    {
        new id=NYX_ActiveUber[playerid]; if(!NYX_UberDriver[playerid]||id==-1) return SendClientMessage(playerid,0xFF3333FF,"[ERRO] Nenhuma corrida ativa."); new passenger=NYX_Uber[id][NYX_UB_Passenger], fare=1200; if(IsPlayerConnected(passenger)&&GetPlayerMoney(passenger)>=fare){GivePlayerMoney(passenger,-fare);GivePlayerMoney(playerid,fare);} else GivePlayerMoney(playerid,500); NYX_Uber[id][NYX_UB_Active]=false; NYX_ActiveUber[playerid]=-1; DisablePlayerCheckpoint(playerid); return SendClientMessage(playerid,0x00FF00FF,"[UBER] Corrida finalizada.");
    }
    return 0;
}

stock NYX_IntegratedDialog(playerid, dialogid, response, listitem)
{
    if(dialogid == 8950) return NYX_ProcessIFoodDialog(playerid,response,listitem);
    return 0;
}

stock NYX_IntegratedCheckpoint(playerid)
{
    if(NYX_BusDriver[playerid])
    {
        GivePlayerMoney(playerid,800); GameTextForPlayer(playerid,"~g~+ R$ 800",2000,1); PlayerPlaySound(playerid,1058,0.0,0.0,0.0);
        NYX_BusStep[playerid]++; if(NYX_BusStep[playerid]>=sizeof NYX_BusStops) NYX_BusStep[playerid]=0;
        SetPlayerCheckpoint(playerid,NYX_BusStops[NYX_BusStep[playerid]][0],NYX_BusStops[NYX_BusStep[playerid]][1],NYX_BusStops[NYX_BusStep[playerid]][2],5.0); return 1;
    }
    return 0;
}

stock NYX_IntegratedDamage(playerid)
{
    if(NYX_GodMode[playerid]) {SetPlayerHealth(playerid,100.0);SetPlayerArmour(playerid,100.0);return 1;}
    return 0;
}

stock NYX_IntegratedUpdate(playerid)
{
    if(NYX_FireDuty[playerid] && NYX_FireActive) NYX_CompleteFire(playerid);
    return 1;
}

stock NYX_IntegratedDisconnect(playerid)
{
    NYX_IntegratedSave(playerid);
    for(new i=0;i<NYX_MAX_FIRE_CALLS;i++) if(NYX_FireCalls[i][NYX_FC_Active] && NYX_FireCalls[i][NYX_FC_Caller]==playerid) NYX_FireCalls[i][NYX_FC_Active]=false;
    for(new i=0;i<NYX_MAX_IFOOD_ORDERS;i++) if(NYX_IFood[i][NYX_IF_Active] && NYX_IFood[i][NYX_IF_Customer]==playerid) NYX_IFood[i][NYX_IF_Active]=false;
    for(new i=0;i<NYX_MAX_UBER_CALLS;i++) if(NYX_Uber[i][NYX_UB_Active] && NYX_Uber[i][NYX_UB_Passenger]==playerid) NYX_Uber[i][NYX_UB_Active]=false;
    NYX_IntegratedReset(playerid);
    return 1;
}
