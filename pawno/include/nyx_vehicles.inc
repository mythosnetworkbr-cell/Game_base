#if defined _NYX_VEHICLES_INCLUDED
    #endinput
#endif
#define _NYX_VEHICLES_INCLUDED

#define NYX_VEHICLE_COUNT 16

new const NYX_VehicleModel[NYX_VEHICLE_COUNT] = {
    400,401,404,410,411,415,418,420,421,426,429,445,458,462,496,516
};
new const NYX_VehicleName[NYX_VEHICLE_COUNT][] = {
    "Landstalker","Bravura","Perennial","Manana","Infernus","Cheetah","Moonbeam","Taxi",
    "Washington","Premier","Banshee","Admiral","Solair","Faggio","Blista Compact","Nebula"
};

stock NYX_GetVehicleModel(index)
{
    if (index < 0 || index >= NYX_VEHICLE_COUNT) return 400;
    return NYX_VehicleModel[index];
}
