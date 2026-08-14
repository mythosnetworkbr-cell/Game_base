#if defined _NYX_JOBS_INCLUDED
    #endinput
#endif
#define _NYX_JOBS_INCLUDED

#define NYX_JOB_COUNT 10

new const NYX_JobName[NYX_JOB_COUNT][] = {
    "Desempregado", "Taxista", "Entregador", "Caminhoneiro", "Lixeiro",
    "Motorista de Onibus", "Pescador", "Agricultor", "Mecanico Civil", "Reporter"
};
new const NYX_JobPay[NYX_JOB_COUNT] = {0,450,550,850,500,650,700,600,750,600};

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
