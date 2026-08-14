#if defined _NYX_CITIES_INCLUDED
    #endinput
#endif
#define _NYX_CITIES_INCLUDED

#define NYX_CITY_COUNT 5

enum E_NYX_CITY
{
    CityName[32],
    CityState[8],
    Float:CityX,
    Float:CityY,
    Float:CityZ
};

new const NYX_Cities[NYX_CITY_COUNT][E_NYX_CITY] =
{
    {"Sao Paulo", "SP", 1481.0, -1771.0, 18.8},
    {"Goiania", "GO", 2025.0, -1400.0, 17.0},
    {"Curitiba", "PR", 2200.0, -1800.0, 20.0},
    {"Blumenau", "SC", 1650.0, -1200.0, 15.0},
    {"Florianopolis", "SC", 1100.0, -950.0, 18.0}
};

stock NYX_GetCityName(cityid, output[], size)
{
    if (cityid < 0 || cityid >= NYX_CITY_COUNT)
    {
        format(output, size, "Desconhecida");
        return 0;
    }
    format(output, size, "%s", NYX_Cities[cityid][CityName]);
    return 1;
}

stock NYX_ShowCities(playerid)
{
    new list[512];
    list[0] = EOS;
    for (new i = 0; i < NYX_CITY_COUNT; i++)
    {
        new line[64];
        format(line, sizeof line, "%s - %s\n", NYX_Cities[i][CityName], NYX_Cities[i][CityState]);
        strcat(list, line, sizeof list);
    }
    ShowPlayerDialog(playerid, 1100, DIALOG_STYLE_LIST, "NYX | CIDADES DO BRASIL", list, "GPS", "FECHAR");
    return 1;
}
