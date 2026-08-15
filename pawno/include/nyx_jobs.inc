#if defined _NYX_JOBS_INCLUDED
    #endinput
#endif
#define _NYX_JOBS_INCLUDED

#define NYX_JOB_COUNT 15
#define NYX_JOB_FINISH_RADIUS 8.0
#define NYX_JOB_COOLDOWN_SECONDS 15

new const NYX_JobName[NYX_JOB_COUNT][] = {
    "Desempregado", "Taxista", "Entregador", "Caminhoneiro", "Lixeiro",
    "Motorista de Onibus", "Pescador", "Agricultor", "Mecanico Civil", "Reporter",
    "Construcao / Obra", "Minerador", "Frentista / Posto", "Gasista", "Motoboy"
};

new const NYX_JobPay[NYX_JOB_COUNT] = {
    0, 900, 1050, 1400, 1000,
    1150, 1200, 1000, 1250, 1100,
    1350, 1550, 1050, 1300, 1250
};

new const NYX_JobVehicle[NYX_JOB_COUNT] = {
    0, 420, 482, 403, 408,
    431, 453, 478, 525, 582,
    482, 408, 414, 414, 462
};

new const Float:NYX_JobPoint[NYX_JOB_COUNT][3] = {
    {1481.0,-1771.0,18.8}, {1800.0,-1870.0,13.6}, {1710.0,-1730.0,13.5},
    {1020.0,-1320.0,13.5}, {1750.0,-1900.0,13.5}, {1100.0,-1350.0,13.5},
    {2300.0,-1500.0,23.0}, {-1050.0,-1200.0,129.0}, {1500.0,-1750.0,13.5},
    {1450.0,-1600.0,13.5}, {1550.0,-1680.0,13.5}, {-380.0,-1400.0,25.7},
    {1000.0,-940.0,42.0}, {1200.0,-1350.0,13.5}, {1650.0,-1720.0,13.5}
};

new NYX_PlayerJob[MAX_PLAYERS];
new bool:NYX_JobRunning[MAX_PLAYERS];
new NYX_JobVehicleId[MAX_PLAYERS];
new NYX_JobLastFinish[MAX_PLAYERS];

stock NYX_GetJobName(jobid, output[], size)
{
    if (jobid < 0 || jobid >= NYX_JOB_COUNT) jobid = 0;
    format(output,size,"%s",NYX_JobName[jobid]);
    return 1;
}

stock NYX_GetJobPay(jobid)
{
    if (jobid < 0 || jobid >= NYX_JOB_COUNT) return 0;
    return NYX_JobPay[jobid];
}

stock NYX_GetJobVehicle(jobid)
{
    if (jobid < 0 || jobid >= NYX_JOB_COUNT) return 0;
    return NYX_JobVehicle[jobid];
}

stock NYX_ResetJob(playerid)
{
    NYX_PlayerJob[playerid] = 0;
    NYX_JobRunning[playerid] = false;
    NYX_JobVehicleId[playerid] = INVALID_VEHICLE_ID;
    NYX_JobLastFinish[playerid] = 0;
    return 1;
}
