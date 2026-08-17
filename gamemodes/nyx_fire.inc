#if defined _NYX_FIRE_INC
    #endinput
#endif
#define _NYX_FIRE_INC

#define VEHICLE_FIRETRUCK 407
#define REWARD_PER_FIRE 1500
#define NYX_FIRE_OBJECT 18691
#define NYX_FIRE_INTERVAL 90000
#define NYX_FIRE_RADIUS 8.0

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

stock NYX_FireInit()
{
    FireData[fObject] = INVALID_OBJECT_ID;
    FireData[fX] = 0.0;
    FireData[fY] = 0.0;
    FireData[fZ] = 0.0;
    FireData[fActive] = false;
    FireTimer = SetTimer("NYX_FireTimer", NYX_FIRE_INTERVAL, true);
    return 1;
}

stock NYX_FireResetPlayer(playerid)
{
    IsOnFireDuty[playerid] = false;
    return 1;
}

stock NYX_FireCleanup()
{
    if (FireTimer) KillTimer(FireTimer);
    FireTimer = 0;
    if (FireData[fActive] && FireData[fObject] != INVALID_OBJECT_ID)
        DestroyObject(FireData[fObject]);
    FireData[fObject] = INVALID_OBJECT_ID;
    FireData[fActive] = false;
    return 1;
}

stock NYX_FireSpawn()
{
    if (FireData[fActive]) return 0;

    new Float:spots[6][3] =
    {
        {1481.0, -1771.0, 18.8},
        {1172.0, -1323.0, 15.4},
        {1040.0, -1020.0, 32.0},
        {1465.0, -1010.0, 24.0},
        {1520.0, -1675.0, 15.0},
        {1550.0, -1600.0, 15.0}
    };
    new index = random(sizeof(spots));
    FireData[fX] = spots[index][0];
    FireData[fY] = spots[index][1];
    FireData[fZ] = spots[index][2];
    FireData[fObject] = CreateObject(NYX_FIRE_OBJECT, FireData[fX], FireData[fY], FireData[fZ], 0.0, 0.0, 0.0);
    if (FireData[fObject] == INVALID_OBJECT_ID) return 0;
    FireData[fActive] = true;

    for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
    {
        if (IsPlayerConnected(playerid) && IsOnFireDuty[playerid])
            SendClientMessage(playerid, 0xFF6B35FF, "[BOMBEIROS] Incendio detectado! Use o GPS e /apagar.");
    }
    return 1;
}

stock NYX_FireExtinguish(playerid)
{
    if (!FireData[fActive])
        return SendClientMessage(playerid, 0xFFCC00FF, "[BOMBEIROS] Nao existe incendio ativo.");

    if (!IsOnFireDuty[playerid])
        return SendClientMessage(playerid, 0xFF4444FF, "[BOMBEIROS] Voce nao esta em servico.");

    if (GetVehicleModel(GetPlayerVehicleID(playerid)) != VEHICLE_FIRETRUCK)
        return SendClientMessage(playerid, 0xFF4444FF, "[BOMBEIROS] Entre em um Firetruck para apagar o incendio.");

    if (!IsPlayerInRangeOfPoint(playerid, NYX_FIRE_RADIUS, FireData[fX], FireData[fY], FireData[fZ]))
        return SendClientMessage(playerid, 0xFFCC00FF, "[BOMBEIROS] Aproxime-se do incendio.");

    if (FireData[fObject] != INVALID_OBJECT_ID)
        DestroyObject(FireData[fObject]);
    FireData[fObject] = INVALID_OBJECT_ID;
    FireData[fActive] = false;
    GivePlayerMoney(playerid, REWARD_PER_FIRE);
    SendClientMessage(playerid, 0x66FF99FF, "[BOMBEIROS] Incendio apagado! Recompensa: $1500.");
    return 1;
}

stock NYX_FireCommand(playerid)
{
    if (GetPlayerState(playerid) != PLAYER_STATE_DRIVER || GetVehicleModel(GetPlayerVehicleID(playerid)) != VEHICLE_FIRETRUCK)
        return SendClientMessage(playerid, 0xFF4444FF, "[BOMBEIROS] Voce precisa estar dirigindo um Firetruck.");

    IsOnFireDuty[playerid] = !IsOnFireDuty[playerid];
    if (IsOnFireDuty[playerid])
    {
        SendClientMessage(playerid, 0x66FF99FF, "[BOMBEIROS] Servico iniciado. Voce recebera chamados de incendio.");
        if (!FireData[fActive]) NYX_FireSpawn();
    }
    else
    {
        SendClientMessage(playerid, 0xFFCC00FF, "[BOMBEIROS] Servico encerrado.");
    }
    return 1;
}

forward NYX_FireTimer();
public NYX_FireTimer()
{
    new activeDuty = 0;
    for (new playerid = 0; playerid < MAX_PLAYERS; playerid++)
        if (IsPlayerConnected(playerid) && IsOnFireDuty[playerid]) activeDuty++;

    if (activeDuty > 0 && !FireData[fActive]) NYX_FireSpawn();
    return 1;
}

stock NYX_FireCommandText(playerid, const cmdtext[])
{
    if (!strcmp(cmdtext, "/bombeiros", true)) return NYX_FireCommand(playerid);
    if (!strcmp(cmdtext, "/apagar", true)) return NYX_FireExtinguish(playerid);
    return 0;
}
