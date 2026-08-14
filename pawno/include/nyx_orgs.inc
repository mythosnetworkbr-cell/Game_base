#if defined _NYX_ORGS_INCLUDED
    #endinput
#endif
#define _NYX_ORGS_INCLUDED

// Organizacoes unicas da NYX. A mesma organizacao atua em todo o mapa.
#define NYX_ORG_COUNT 24

new const NYX_OrgName[NYX_ORG_COUNT][] = {
    "Policia Militar",
    "Policia Civil",
    "Policia Federal",
    "BOPE",
    "Forcas Armadas",
    "Exercito Brasileiro",
    "Marinha do Brasil",
    "Forca Aerea Brasileira",
    "Guarda Municipal",
    "SAMU",
    "Corpo de Bombeiros",
    "Governo",
    "Prefeitura NYX",
    "Jornal NYX",
    "Mecanicos",
    "Transportes NYX",
    "Ballas",
    "Families",
    "Comando Vermelho",
    "Primeiro Comando",
    "Los Aztecas",
    "Cartel NYX",
    "Motoclube",
    "Sindicato do Crime"
};

new bool:NYX_OrgActive[NYX_ORG_COUNT] = {
    true,true,true,true,true,true,true,true,
    true,true,true,true,true,true,true,true,
    true,true,true,true,true,true,true,true
};

stock NYX_IsValidOrg(orgid)
{
    return orgid >= 0 && orgid < NYX_ORG_COUNT;
}

stock NYX_GetOrgName(orgid, output[], size)
{
    if (!NYX_IsValidOrg(orgid)) return 0;
    format(output, size, "%s", NYX_OrgName[orgid]);
    return 1;
}

stock NYX_GetOrgStatus(orgid)
{
    if (!NYX_IsValidOrg(orgid)) return 0;
    return NYX_OrgActive[orgid];
}

stock NYX_SetOrgStatus(orgid, bool:active)
{
    if (!NYX_IsValidOrg(orgid)) return 0;
    NYX_OrgActive[orgid] = active;
    return 1;
}
