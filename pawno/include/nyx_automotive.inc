#if defined _NYX_AUTOMOTIVE_INCLUDED
    #endinput
#endif
#define _NYX_AUTOMOTIVE_INCLUDED

#define NYX_DEALER_COUNT 6
#define NYX_TUNING_COUNT 5
#define NYX_VEHICLE_FEATURE_COUNT 8

enum E_NYX_DEALER { DealerName[40], DealerCategory[24] };
new const NYX_Dealers[NYX_DEALER_COUNT][E_NYX_DEALER] = {
    {"NYX Motors", "Populares"},
    {"NYX Premium", "Luxo"},
    {"NYX Performance", "Esportivos"},
    {"NYX Trucks", "Caminhoes"},
    {"NYX Motos", "Motocicletas"},
    {"NYX Nautica", "Barcos"}
};

new const NYX_TuningShop[NYX_TUNING_COUNT][] = {
    "NYX Customs", "Street Garage", "Performance Lab", "Off-Road Garage", "Elite Tuning"
};

new const NYX_VehicleFeature[NYX_VEHICLE_FEATURE_COUNT][] = {
    "Blindagem", "Motor", "Freios", "Suspensao", "Pneus", "Turbo", "Som", "Visual"
};

stock NYX_ShowDealers(playerid)
{
    new list[768], line[100]; list[0] = EOS;
    for(new i=0;i<NYX_DEALER_COUNT;i++)
    {
        format(line,sizeof line,"%s | %s\n",NYX_Dealers[i][DealerName],NYX_Dealers[i][DealerCategory]);
        strcat(list,line,sizeof list);
    }
    return ShowPlayerDialog(playerid,1200,DIALOG_STYLE_LIST,"NYX | CONCESSIONARIAS",list,"ENTRAR","FECHAR");
}

stock NYX_ShowTuning(playerid)
{
    new list[768], line[96]; list[0] = EOS;
    for(new i=0;i<NYX_TUNING_COUNT;i++)
    {
        format(line,sizeof line,"%s\n",NYX_TuningShop[i]);
        strcat(list,line,sizeof list);
    }
    return ShowPlayerDialog(playerid,1201,DIALOG_STYLE_LIST,"NYX | TUNING",list,"ABRIR","FECHAR");
}

stock NYX_ShowVehicleFeatures(playerid)
{
    new list[768], line[96]; list[0] = EOS;
    for(new i=0;i<NYX_VEHICLE_FEATURE_COUNT;i++)
    {
        format(line,sizeof line,"%s\n",NYX_VehicleFeature[i]);
        strcat(list,line,sizeof list);
    }
    return ShowPlayerDialog(playerid,1202,DIALOG_STYLE_LIST,"NYX | OFICINA AVANCADA",list,"CONFIGURAR","FECHAR");
}

// Conceito NYX: carro possui identidade própria, manutenção, desgaste e histórico.
// O banco persistente será conectado a estes campos na próxima camada.
