#if defined _NYX_ADMIN_TEXT_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_TEXT_INCLUDED
#include <sscanf2>
stock NYX_AdminDeny(playerid) { return SendClientMessage(playerid, COLOR_ERROR, "NYX: voce nao possui permissao para este comando."); }
stock strncmp(const a[], const b[], length, bool:ignorecase=false)
{
    new i, ca, cb;
    for(i=0; i<length; i++)
    {
        ca=a[i]; cb=b[i];
        if(ignorecase){if(ca>='A'&&ca<='Z') ca+=32;if(cb>='A'&&cb<='Z') cb+=32;}
        if(ca!=cb) return ca-cb;
        if(ca==EOS) return 0;
    }
    return 0;
}
stock bool:NYX_AdminCommandText(playerid, cmdtext[])
{
    if (!NYX_Player[playerid][NYX_Logged]) return false;
    new cmd[32], params[128];
    cmd[0]=EOS; params[0]=EOS;
    sscanf(cmdtext, "p< >s[32]S()[128]", cmd, params);
    if(cmd[0]=='/') strdel(cmd,0,1);
    if(!strcmp(cmd,"tbadm",true)) { if(NYX_Player[playerid][NYX_Admin]<1) return NYX_AdminDeny(playerid), true; SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF nivel 1: painel administrativo."); return true; }
    if(!strcmp(cmd,"voar",true)) { if(NYX_Player[playerid][NYX_Admin]<1) return NYX_AdminDeny(playerid), true; if(GetPVarType(playerid,"NYX_FLY")){DeletePVar(playerid,"NYX_FLY");SetPlayerPos(playerid,1481.0,-1771.0,18.8);SendClientMessage(playerid,COLOR_SUCCESS,"Modo voo desativado.");}else{SetPVarInt(playerid,"NYX_FLY",1);SendClientMessage(playerid,COLOR_SUCCESS,"Modo voo ativado.");} return true; }
    if(!strcmp(cmd,"setskin",true)) { if(NYX_Player[playerid][NYX_Admin]<3) return NYX_AdminDeny(playerid), true; new id,skin; if(sscanf(params,"ii",id,skin)||!IsPlayerConnected(id)||!NYX_IsValidSkin(skin)) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /setskin [playerid] [skin]"), true; NYX_Player[id][NYX_Skin]=skin; SetPlayerSkin(id,skin); return SendClientMessage(playerid,COLOR_SUCCESS,"Skin alterada."), true; }
    if(!strcmp(cmd,"dargrana",true)) { if(NYX_Player[playerid][NYX_Admin]<5) return NYX_AdminDeny(playerid), true; new id,amount; if(sscanf(params,"ii",id,amount)||!IsPlayerConnected(id)||amount<0) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /dargrana [playerid] [valor]"), true; GivePlayerMoney(id,amount); NYX_Player[id][NYX_Money]=GetPlayerMoney(id); return SendClientMessage(playerid,COLOR_SUCCESS,"Dinheiro entregue."), true; }
    if(!strcmp(cmd,"darcoins",true)) { if(NYX_Player[playerid][NYX_Admin]<10) return NYX_AdminDeny(playerid), true; new id,amount; if(sscanf(params,"ii",id,amount)||!IsPlayerConnected(id)||amount<0) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /darcoins [playerid] [ncoins]"), true; NYX_Player[id][NYX_NCoins]+=amount; return SendClientMessage(playerid,COLOR_SUCCESS,"NCoins entregues."), true; }
    if(!strcmp(cmd,"setadm",true)) { if(NYX_Player[playerid][NYX_Admin]<10) return NYX_AdminDeny(playerid), true; new id,level; if(sscanf(params,"ii",id,level)||!IsPlayerConnected(id)||level<0||level>10) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /setadm [playerid] [0-10]"), true; NYX_Player[id][NYX_Admin]=level; return SendClientMessage(playerid,COLOR_SUCCESS,"Nivel administrativo alterado."), true; }
    if(!strcmp(cmd,"sethora",true)) { if(NYX_Player[playerid][NYX_Admin]<10) return NYX_AdminDeny(playerid), true; new h; if(sscanf(params,"i",h)||h<0||h>23) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /sethora [0-23]"), true; SetWorldTime(h); return SendClientMessage(playerid,COLOR_SUCCESS,"Hora alterada."), true; }
    if(!strcmp(cmd,"setclima",true)) { if(NYX_Player[playerid][NYX_Admin]<10) return NYX_AdminDeny(playerid), true; new w; if(sscanf(params,"i",w)||w<0||w>255) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /setclima [0-255]"), true; SetWeather(w); return SendClientMessage(playerid,COLOR_SUCCESS,"Clima alterado."), true; }
    if(!strcmp(cmd,"limparchat",true)) { if(NYX_Player[playerid][NYX_Admin]<10) return NYX_AdminDeny(playerid), true; for(new i=0;i<25;i++) SendClientMessageToAll(-1," "); return SendClientMessage(playerid,COLOR_SUCCESS,"Chat limpo."), true; }
    if(!strcmp(cmd,"dv",true)) { if(NYX_Player[playerid][NYX_Admin]<10) return NYX_AdminDeny(playerid), true; new v=GetPlayerVehicleID(playerid); if(!v) return SendClientMessage(playerid,COLOR_ERROR,"Voce nao esta em um veiculo."), true; DestroyVehicle(v); return SendClientMessage(playerid,COLOR_SUCCESS,"Veiculo removido."), true; }
    if(!strcmp(cmd,"respawncars",true)) { if(NYX_Player[playerid][NYX_Admin]<1) return NYX_AdminDeny(playerid), true; for(new v=1;v<MAX_VEHICLES;v++) SetVehicleToRespawn(v); return SendClientMessage(playerid,COLOR_SUCCESS,"Veiculos respawnados."), true; }
    if(!strcmp(cmd,"reviver",true)) { if(NYX_Player[playerid][NYX_Admin]<1) return NYX_AdminDeny(playerid), true; new id; if(sscanf(params,"i",id)||!IsPlayerConnected(id)) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /reviver [playerid]"), true; SetPlayerHealth(id,100.0); return SendClientMessage(playerid,COLOR_SUCCESS,"Jogador revivido."), true; }
    if(!strcmp(cmd,"ir",true)) { if(NYX_Player[playerid][NYX_Admin]<10) return NYX_AdminDeny(playerid), true; new id; if(sscanf(params,"i",id)||!IsPlayerConnected(id)) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /ir [playerid]"), true; new Float:x,Float:y,Float:z; GetPlayerPos(id,x,y,z); SetPlayerPos(playerid,x+1.0,y,z); return true; }
    if(!strcmp(cmd,"trazer",true)) { if(NYX_Player[playerid][NYX_Admin]<10) return NYX_AdminDeny(playerid), true; new id; if(sscanf(params,"i",id)||!IsPlayerConnected(id)) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /trazer [playerid]"), true; new Float:x,Float:y,Float:z; GetPlayerPos(id,x,y,z); SetPlayerPos(id,x+1.0,y,z); return true; }
    new level=0;
    if(!strcmp(cmd,"aviso",true)) level=2;
    else if(!strcmp(cmd,"criarnpc",true)||!strcmp(cmd,"apagarnpc",true)||!strcmp(cmd,"irnpc",true)) level=6;
    else if(!strcmp(cmd,"kitadm",true)) level=7;
    else if(!strcmp(cmd,"banir",true)||!strcmp(cmd,"desbanir",true)) level=8;
    else if(!strcmp(cmd,"setskin",true)) level=3;
    else if(!strcmp(cmd,"dargrana",true)) level=5;
    else if(!strcmp(cmd,"setadm",true)||!strcmp(cmd,"limparchat",true)||!strcmp(cmd,"sethora",true)||!strcmp(cmd,"setclima",true)||!strcmp(cmd,"dv",true)||!strcmp(cmd,"rcar",true)||!strcmp(cmd,"tv",true)||!strcmp(cmd,"tvoff",true)||!strcmp(cmd,"ir",true)||!strcmp(cmd,"trazer",true)||!strcmp(cmd,"setidf",true)||!strcmp(cmd,"sethoras",true)||!strcmp(cmd,"darporte",true)||!strcmp(cmd,"setnumero",true)||!strcmp(cmd,"criaraluguel",true)||!strcmp(cmd,"criarlojinha",true)||!strcmp(cmd,"agendarcoins",true)||!strcmp(cmd,"criarcupom",true)||!strcmp(cmd,"apagarcupom",true)||!strcmp(cmd,"cupons",true)||!strcmp(cmd,"apagaraluguel",true)||!strcmp(cmd,"criaracessorios",true)||!strcmp(cmd,"apagaracessorios",true)||!strcmp(cmd,"daritem",true)||!strcmp(cmd,"pitem",true)||!strcmp(cmd,"apagarlojinha",true)||!strcmp(cmd,"setnotas",true)||!strcmp(cmd,"vernotas",true)||!strcmp(cmd,"zerarnota",true)||!strcmp(cmd,"setjoa",true)||!strcmp(cmd,"setbalcao",true)||!strcmp(cmd,"darcoins",true)) level=10;
    else if(!strcmp(cmd,"tbadm",true)||!strcmp(cmd,"frpos",true)||!strcmp(cmd,"ls",true)||!strcmp(cmd,"sf",true)||!strcmp(cmd,"lv",true)||!strcmp(cmd,"voar",true)||!strcmp(cmd,"veridf",true)||!strcmp(cmd,"limparorg",true)||!strcmp(cmd,"frorg",true)||!strcmp(cmd,"limparlider",true)||!strcmp(cmd,"limparvaga",true)||!strcmp(cmd,"cvenda",true)||!strcmp(cmd,"darmembro",true)||!strcmp(cmd,"darlider",true)||!strcmp(cmd,"criarposto",true)||!strcmp(cmd,"apagarposto",true)||!strcmp(cmd,"apagarnegro",true)||!strcmp(cmd,"apagarcaixa",true)||!strcmp(cmd,"apagargaragem",true)||!strcmp(cmd,"apagardesmanche",true)||!strcmp(cmd,"criarconce",true)||!strcmp(cmd,"apagarconce",true)||!strcmp(cmd,"criaragencia",true)||!strcmp(cmd,"apagaragencia",true)||!strcmp(cmd,"atender",true)||!strcmp(cmd,"satender",true)||!strcmp(cmd,"fila",true)||!strcmp(cmd,"fimatt",true)||!strcmp(cmd,"cadeia",true)||!strcmp(cmd,"scadeia",true)||!strcmp(cmd,"reviver",true)||!strcmp(cmd,"permitirorg",true)||!strcmp(cmd,"proibirorg",true)||!strcmp(cmd,"respawncars",true)||!strcmp(cmd,"frnpc",true)) level=1;
    if(level>0) { if(NYX_Player[playerid][NYX_Admin]<level) NYX_AdminDeny(playerid); else SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: comando reconhecido e reservado para integracao do sistema Galaxy."); return true; }
    return false;
}
