#if defined _NYX_WORLD_INCLUDED
    #endinput
#endif
#define _NYX_WORLD_INCLUDED

#define NYX_CITY_COUNT 5
new const NYX_CityName[NYX_CITY_COUNT][] = {"Sao Paulo","Goiania","Curitiba","Blumenau","Florianopolis"};
new const NYX_ServiceName[5][] = {"Hospital Central NYX","Delegacia Central NYX","Prefeitura NYX","Cassino NYX","Sex Shop NYX"};
new const Float:NYX_ServicePoint[5][3] = {
    {1520.0,-1675.0,13.5}, {1550.0,-1600.0,13.5}, {1481.0,-1771.0,18.8},
    {2200.0,-1670.0,14.0}, {1350.0,-1740.0,13.5}
};

stock NYX_ShowWorldInfo(playerid)
{
    SendClientMessage(playerid, COLOR_NYX, "NYX | Cidades: Sao Paulo, Goiania, Curitiba, Blumenau e Florianopolis.");
    SendClientMessage(playerid, COLOR_WHITE, "Servicos principais: 1 Hospital, 1 Delegacia, 1 Prefeitura, 1 Cassino e estabelecimentos de bairro.");
    return 1;
}
