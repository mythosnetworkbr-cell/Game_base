#if defined _NYX_ORG_RANKS_INCLUDED
    #endinput
#endif
#define _NYX_ORG_RANKS_INCLUDED

// Hierarquia centralizada das organizações NYX.
// Cargo 1 = entrada; Cargo 10 = liderança máxima.
#define NYX_ORG_RANK_MIN 1
#define NYX_ORG_RANK_MAX 10
#define NYX_ORG_LEADER_RANK 10

stock NYX_OrgRankValid(rank)
{
    return rank >= NYX_ORG_RANK_MIN && rank <= NYX_ORG_RANK_MAX;
}

stock NYX_OrgRankHas(playerid, rank)
{
    return NYX_Player[playerid][NYX_Org] >= 0 && NYX_Player[playerid][NYX_OrgRank] >= rank;
}

stock NYX_OrgRankIsLeader(playerid)
{
    return NYX_Player[playerid][NYX_Org] >= 0 && NYX_Player[playerid][NYX_OrgRank] == NYX_ORG_LEADER_RANK;
}

stock NYX_OrgCanInvite(playerid) { return NYX_OrgRankHas(playerid, 3); }
stock NYX_OrgCanPromote(playerid) { return NYX_OrgRankHas(playerid, 4); }
stock NYX_OrgCanDemote(playerid) { return NYX_OrgRankHas(playerid, 4); }
stock NYX_OrgCanRemoveMember(playerid) { return NYX_OrgRankHas(playerid, 5); }
stock NYX_OrgCanManageVehicles(playerid) { return NYX_OrgRankHas(playerid, 6); }
stock NYX_OrgCanManageVault(playerid) { return NYX_OrgRankHas(playerid, 7); }
stock NYX_OrgCanManageLeadership(playerid) { return NYX_OrgRankIsLeader(playerid); }

// Nomes genéricos para a estrutura; cada organização poderá substituir
// estes nomes por sua própria nomenclatura sem alterar as permissões.
stock NYX_OrgRankName(rank, name[], size)
{
    switch(rank)
    {
        case 1: format(name,size,"Membro");
        case 2: format(name,size,"Recruta");
        case 3: format(name,size,"Soldado");
        case 4: format(name,size,"Supervisor");
        case 5: format(name,size,"Gerente");
        case 6: format(name,size,"Oficial");
        case 7: format(name,size,"Subchefe");
        case 8: format(name,size,"Subcomandante");
        case 9: format(name,size,"Vice-Lider");
        case 10: format(name,size,"Lider");
        default: format(name,size,"Sem cargo");
    }
    return 1;
}

#endif
