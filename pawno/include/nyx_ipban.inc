#if defined _NYX_IPBAN_INCLUDED
    #endinput
#endif
#define _NYX_IPBAN_INCLUDED

stock NYX_IPBanFile(ip[], path[], size) { format(path,size,"scriptfiles/nyx_ipban_%s.dat",ip); return 1; }
stock bool:NYX_IPIsBanned(playerid)
{
    new ip[46],path[96];
    GetPlayerIp(playerid,ip,sizeof ip);
    NYX_IPBanFile(ip,path,sizeof path);
    return fexist(path);
}
