#if defined _NYX_SERVICES_INCLUDED
    #endinput
#endif
#define _NYX_SERVICES_INCLUDED

// A NYX terá apenas UMA unidade principal de hospital e UMA delegacia.
// Outros bairros/cidades recebem postos de atendimento, farmácias e serviços,
// sem duplicar organizações ou sedes.

enum E_NYX_SERVICE_TYPE
{
    NYX_SERVICE_HOSPITAL,
    NYX_SERVICE_DELEGACIA,
    NYX_SERVICE_POSTO_SAUDE,
    NYX_SERVICE_FARMACIA,
    NYX_SERVICE_SEXSHOP,
    NYX_SERVICE_PREFEITURA,
    NYX_SERVICE_OFICINA,
    NYX_SERVICE_CONCESSIONARIA
};

new const NYX_ServiceName[][] =
{
    "Hospital Central NYX",
    "Delegacia Central NYX",
    "Posto de Saude",
    "Farmacia",
    "Sex Shop",
    "Prefeitura NYX",
    "Oficina",
    "Concessionaria"
};

// O Sex Shop é tratado como estabelecimento comercial adulto, sem conteúdo sexual explícito.
// Pode vender itens fictícios de RP e funcionar como empresa do sistema econômico.
