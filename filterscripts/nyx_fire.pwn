#include <a_samp>

#define VEHICLE_FIRETRUCK 407
#define REWARD_PER_FIRE 1500
#define NYX_FIRE_OBJECT 18691
#define NYX_FIRE_INTERVAL 90000
#define NYX_FIRE_RADIUS 8.0
#define NYX_FIRE_WEAPON 42

enum E_FIRE_DATA
{
    fObject,
    Float:fX,
    Float:fY,
    Float:fZ,
    bool:fActive
}
new FireData[E_FIRE_DATA];
new bool:IsOnFireDuty[MAX_PLAYERS];
new FireTimer;

forward NYX_FireTimer();

public OnFilterScriptInit()
{
    FireData[fObject] = INVALID_OBJECT_ID;
    FireData[fActive] = false;
    FireTimer = SetTimer("NYX_FireTimer", NYX_FIRE_INTERVAL, true);
    print("[NYX FIRE] Sistema de bombeiros carregado.");
    return 1;
}

public OnFilterScriptExit()
{
    if (FireTimer) KillTimer(FireTimer);
    if (FireData[fActive] && FireData[fObject] != INVALID_OBJECT_ID)
        DestroyObject(FireData[fObject]);
    return 1;
}

public OnPlayerConnect(playerid)
{
    IsOnFireDuty[playerid] = false;
    return 1;
}

public OnPlayerDisconnect(playerid, reason)
{
    IsOnFireDuty[playerid] = false;
    return 1;
}

stock NYX_FireSpawn()
{
    if (FireData[fActive]) return 0;

    new Float:spots[8][3] =
    {
        {1813.432, -1882.112, 13.414},
        {2104.123, -1805.321, 13.554},
        {1420.551, -1700.100, 13.546},
        {1481.0, -1771.0, 18.8},
        {1172.0, -1323.0, 15.4},
        {1040.0, -1020.0, 32.0},
        {1465.0, -1010.0, 24.0},
        {1520.0, -1675.0, 15.0}
    };

    new index = random(sizeof(spots));
    FireData[fX] = spots[index][0];
    FireData[fY] = spots[index][1];
    FireData[fZ] = spots[index][2];
    FireData[fObject] = CreateObject(NYX_FIRE_OBJECT, FireData[fX], FireData[fY], FireData[fZ] - 1.5, 0.0, 0.0, 0.0);
    if (FireData[fObject] == INVALID_OBJECT_ID) return 0;

    FireData[fActive] = true;
    for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if (IsPlayerConnected(playerid) && IsOnFireDuty[playerid])
        {
            SendClientMessage(playerid, 0xFF4444FF, "[EMERGENCIA] Novo incendio reportado! Use /chamadosbombeiro.");
            PlayerPlaySound(playerid, 1058, 0.0, 0.0, 0.0);
        }
    }
    return 1;
}

stock NYX_FireDuty(playerid)
{
    if (!IsPlayerConnected(playerid)) return 0;

    IsOnFireDuty[playerid] = !IsOnFireDuty[playerid];
    if (IsOnFireDuty[playerid])
    {
        GivePlayerWeapon(playerid, NYX_FIRE_WEAPON, 1000);
        SendClientMessage(playerid, 0x00FF00FF, "[BOMBEIRO] Voce entrou em servico! Fique atento as chamadas de incendio.");
        if (!FireData[fActive]) NYX_FireSpawn();
    }
    else
    {
        ResetPlayerWeapons(playerid);
        DisablePlayerCheckpoint(playerid);
        SendClientMessage(playerid, 0xFFFF00FF, "[BOMBEIRO] Voce saiu de servico.");
    }
    return 1;
}

stock NYX_FireCall(playerid)
{
    if (!IsOnFireDuty[playerid])
        return SendClientMessage(playerid, 0xFF0000FF, "[ERRO] Voce precisa estar em servico (/trabalharbombeiro).");

    if (!FireData[fActive])
        return SendClientMessage(playerid, 0xFF0000FF, "[ERRO] Nao ha nenhum incendio ativo no momento.");

    SetPlayerCheckpoint(playerid, FireData[fX], FireData[fY], FireData[fZ], 4.0);
    SendClientMessage(playerid, 0xFFFF00FF, "[BOMBEIRO] Ponto do incendio marcado no seu mapa!");
    return 1;
}

stock NYX_FireExtinguish(playerid)
{
    if (!FireData[fActive]) return SendClientMessage(playerid, 0xFFCC00FF, "[BOMBEIROS] Nao existe incendio ativo.");
    if (!IsOnFireDuty[playerid]) return SendClientMessage(playerid, 0xFF4444FF, "[BOMBEIROS] Voce nao esta em servico.");
    if (!IsPlayerInRangeOfPoint(playerid, NYX_FIRE_RADIUS, FireData[fX], FireData[fY], FireData[fZ]))
        return SendClientMessage(playerid, 0xFFCC00FF, "[BOMBEIROS] Aproxime-se do incendio.");

    new weapon = GetPlayerWeapon(playerid);
    new usingTruck = (IsPlayerInAnyVehicle(playerid) && GetVehicleModel(GetPlayerVehicleID(playerid)) == VEHICLE_FIRETRUCK);
    if (weapon != NYX_FIRE_WEAPON && !usingTruck)
        return SendClientMessage(playerid, 0xFF4444FF, "[BOMBEIROS] Use o extintor ou a mangueira de um Firetruck.");

    if (FireData[fObject] != INVALID_OBJECT_ID)
        DestroyObject(FireData[fObject]);
    FireData[fObject] = INVALID_OBJECT_ID;
    FireData[fActive] = false;
    DisablePlayerCheckpoint(playerid);
    GivePlayerMoney(playerid, REWARD_PER_FIRE);

    new msg[144];
    format(msg, sizeof(msg), "[BOMBEIRO] Incendio apagado com sucesso! Voce recebeu R$ %d de recompensa.", REWARD_PER_FIRE);
    SendClientMessage(playerid, 0x00FF00FF, msg);
    return 1;
}

public NYX_FireTimer()
{
    new activeDuty;
    for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
        if (IsPlayerConnected(playerid) && IsOnFireDuty[playerid]) activeDuty++;

    if (activeDuty > 0 && !FireData[fActive]) NYX_FireSpawn();
    return 1;
}

public OnPlayerUpdate(playerid)
{
    if (!IsOnFireDuty[playerid] || !FireData[fActive]) return 1;
    if (!IsPlayerInRangeOfPoint(playerid, 4.0, FireData[fX], FireData[fY], FireData[fZ])) return 1;

    new weapon = GetPlayerWeapon(playerid);
    new keys, updown, leftright;
    GetPlayerKeys(playerid, keys, updown, leftright);
    new usingTruck = (IsPlayerInAnyVehicle(playerid) && GetVehicleModel(GetPlayerVehicleID(playerid)) == VEHICLE_FIRETRUCK);

    if ((weapon == NYX_FIRE_WEAPON && (keys & KEY_FIRE)) || (usingTruck && (keys & KEY_FIRE)))
        NYX_FireExtinguish(playerid);
    return 1;
}

public OnPlayerCommandText(playerid, cmdtext[])
{
    if (!strcmp(cmdtext, "/trabalharbombeiro", true)) return NYX_FireDuty(playerid);
    if (!strcmp(cmdtext, "/chamadosbombeiro", true)) return NYX_FireCall(playerid);

    // Aliases mantidos para compatibilidade com a primeira versao do sistema.
    if (!strcmp(cmdtext, "/bombeiros", true)) return NYX_FireDuty(playerid);
    if (!strcmp(cmdtext, "/apagar", true)) return NYX_FireExtinguish(playerid);
    return 0;
}
