#if defined _NYX_CITYHALL_INCLUDED
    #endinput
#endif
#define _NYX_CITYHALL_INCLUDED

stock NYX_CreateFutureCityHall()
{
    CreateObject(18762, 1481.0, -1771.0, 18.0, 0.0, 0.0, 0.0);
    CreateObject(18762, 1481.0, -1761.0, 18.0, 0.0, 0.0, 0.0);
    CreateObject(18762, 1491.0, -1771.0, 18.0, 0.0, 0.0, 90.0);
    CreateObject(18762, 1471.0, -1771.0, 18.0, 0.0, 0.0, 90.0);

    CreateObject(19379, 1468.0, -1771.0, 24.0, 0.0, 0.0, 90.0);
    CreateObject(19379, 1494.0, -1771.0, 24.0, 0.0, 0.0, 90.0);
    CreateObject(19379, 1481.0, -1784.0, 24.0, 0.0, 0.0, 0.0);
    CreateObject(19379, 1481.0, -1771.0, 22.0, 0.0, 0.0, 0.0);
    CreateObject(19379, 1481.0, -1771.0, 27.0, 0.0, 0.0, 0.0);

    CreateObject(19448, 1473.0, -1770.0, 25.0, 0.0, 90.0, 0.0);
    CreateObject(19448, 1489.0, -1770.0, 25.0, 0.0, 90.0, 0.0);
    CreateObject(19448, 1481.0, -1778.0, 25.0, 90.0, 0.0, 0.0);

    // Colunas frontais feitas com objetos nativos
    CreateObject(19379, 1474.0, -1763.0, 22.0, 0.0, 90.0, 0.0);
    CreateObject(19379, 1481.0, -1763.0, 22.0, 0.0, 90.0, 0.0);
    CreateObject(19379, 1488.0, -1763.0, 22.0, 0.0, 90.0, 0.0);

    CreateObject(18762, 1481.0, -1771.0, 31.0, 0.0, 0.0, 0.0);
    CreateObject(19353, 1481.0, -1762.0, 28.0, 0.0, 0.0, 0.0);

    Create3DTextLabel("{8B5CF6}NYX PREFEITURA\n{FFFFFF}CENTRO ADMINISTRATIVO", 0xFFFFFFFF, 1481.0, -1760.0, 30.0, 45.0, 0, 1);
    Create3DTextLabel("{6EE7B7}ENTRADA PRINCIPAL\n{FFFFFF}Atendimento ao cidadao", 0xFFFFFFFF, 1481.0, -1758.0, 19.5, 20.0, 0, 1);
    CreatePickup(1274, 1, 1481.0, -1758.0, 19.0, 0);
    return 1;
}

stock NYX_CityHallEntrance(playerid)
{
    SetPlayerPos(playerid, 1481.0, -1757.5, 19.0);
    SetPlayerFacingAngle(playerid, 180.0);
    SetCameraBehindPlayer(playerid);
    return 1;
}
