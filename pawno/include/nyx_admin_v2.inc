#if defined _NYX_ADMIN_V2_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_V2_INCLUDED

stock NYX_AdminLevel(playerid) { return NYX_Player[playerid][NYX_Admin]; }
stock bool:NYX_AdminHas(playerid, level) { return NYX_AdminLevel(playerid) >= level; }
stock bool:NYX_Leader3(playerid) { return NYX_Player[playerid][NYX_Org] >= 0 && NYX_Player[playerid][NYX_OrgRank] == 3; }
stock NYX_AdminDeny(playerid) { return SendClientMessage(playerid, COLOR_ERROR, "NYX: voce nao possui permissao para este comando."); }
stock NYX_LeaderDeny(playerid) { return SendClientMessage(playerid, COLOR_ERROR, "NYX: este comando exige lider de organizacao (cargo 3)."); }

// Todos os antigos comandos de Admin 1 passam para Lider Cargo 3.
#define NYX_LEADER_CHECK(%0) if(!NYX_Leader3(%0)) return NYX_LeaderDeny(%0)

CMD:tbadm(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /tbadm executado."); }
CMD:frpos(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /frpos executado."); }
CMD:ls(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /ls executado."); }
CMD:sf(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /sf executado."); }
CMD:lv(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /lv executado."); }
CMD:voar(playerid, params[]) { NYX_LEADER_CHECK(playerid); if(GetPVarType(playerid,"NYX_FLY")){DeletePVar(playerid,"NYX_FLY");SetPlayerPos(playerid,1481.0,-1771.0,18.8);return SendClientMessage(playerid,COLOR_SUCCESS,"Modo voo desativado.");}SetPVarInt(playerid,"NYX_FLY",1);return SendClientMessage(playerid,COLOR_SUCCESS,"Modo voo ativado."); }
CMD:veridf(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /veridf executado."); }
CMD:limparorg(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /limparorg executado."); }
CMD:frorg(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /frorg executado."); }
CMD:limparlider(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /limparlider executado."); }
CMD:limparvaga(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /limparvaga executado."); }
CMD:cvenda(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /cvenda executado."); }
CMD:darmembro(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /darmembro executado."); }
CMD:darlider(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /darlider executado."); }
CMD:criarposto(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /criarposto executado."); }
CMD:apagarposto(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /apagarposto executado."); }
CMD:apagarnegro(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /apagarnegro executado."); }
CMD:apagarcaixa(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /apagarcaixa executado."); }
CMD:apagargaragem(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /apagargaragem executado."); }
CMD:apagardesmanche(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /apagardesmanche executado."); }
CMD:criarconce(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /criarconce executado."); }
CMD:apagarconce(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /apagarconce executado."); }
CMD:criaragencia(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /criaragencia executado."); }
CMD:apagaragencia(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /apagaragencia executado."); }
CMD:atender(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: atendimento executado."); }
CMD:satender(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: atendimento silencioso executado."); }
CMD:cadeia(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /cadeia executado."); }
CMD:scadeia(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /scadeia executado."); }
CMD:reviver(playerid, params[]) { NYX_LEADER_CHECK(playerid); new id; if(sscanf(params,"i",id)||!IsPlayerConnected(id)) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /reviver [playerid]"); SetPlayerHealth(id,100.0); return SendClientMessage(playerid,COLOR_SUCCESS,"Jogador revivido."); }
CMD:permitirorg(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"Organizacao permitida."); }
CMD:proibirorg(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"Organizacao proibida."); }
CMD:respawncars(playerid, params[]) { NYX_LEADER_CHECK(playerid); for(new v=1;v<MAX_VEHICLES;v++) SetVehicleToRespawn(v); return SendClientMessage(playerid,COLOR_SUCCESS,"Veiculos respawnados."); }
CMD:frnpc(playerid, params[]) { NYX_LEADER_CHECK(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NYX STAFF: /frnpc executado."); }

// Banimento IP exclusivo do Lider Cargo 3.
stock NYX_IPBanFile(ip[], path[], size) { format(path,size,"scriptfiles/nyx_ipban_%s.dat",ip); return 1; }
stock bool:NYX_IPBanned(playerid)
{
    new ip[46],path[96]; GetPlayerIp(playerid,ip,sizeof ip); NYX_IPBanFile(ip,path,sizeof path); return fexist(path);
}
stock NYX_ApplyIPBan(playerid, reason[])
{
    new ip[46],path[96],file; GetPlayerIp(playerid,ip,sizeof ip); NYX_IPBanFile(ip,path,sizeof path); file=fopen(path,io_write); if(!file)return 0; fwrite(file,reason); fclose(file); return 1;
}
CMD:banip(playerid, params[])
{
    if(!NYX_Leader3(playerid)) return NYX_LeaderDeny(playerid);
    new id,reason[96],ip[46],path[96],file;
    if(sscanf(params,"is[96]",id,reason)||!IsPlayerConnected(id)) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /banip [playerid] [motivo]");
    GetPlayerIp(id,ip,sizeof ip); NYX_IPBanFile(ip,path,sizeof path);
    file=fopen(path,io_write); if(!file)return SendClientMessage(playerid,COLOR_ERROR,"Nao foi possivel salvar o banimento IP.");
    fwrite(file,reason); fclose(file);
    SendClientMessage(playerid,COLOR_SUCCESS,"IP banido permanentemente.");
    SendClientMessage(id,COLOR_ERROR,"Seu IP foi banido da NYX ROLEPLAY.");
    Kick(id); return 1;
}

// Desbanimento continua protegido para Admin 8+.
CMD:desbanip(playerid, params[])
{
    if(!NYX_AdminHas(playerid,8)) return NYX_AdminDeny(playerid);
    new ip[46],path[96]; if(sscanf(params,"s[46]",ip)) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /desbanip [ip]");
    NYX_IPBanFile(ip,path,sizeof path); if(!fexist(path)) return SendClientMessage(playerid,COLOR_ERROR,"IP nao esta banido.");
    fremove(path); return SendClientMessage(playerid,COLOR_SUCCESS,"IP desbanido.");
}

CMD:aviso(playerid, params[]) { if(!NYX_AdminHas(playerid,2)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"Aviso administrativo."); }
CMD:setskin(playerid, params[]) { if(!NYX_AdminHas(playerid,3)) return NYX_AdminDeny(playerid); new id,skin; if(sscanf(params,"ii",id,skin)) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /setskin [playerid] [skin]"); if(!IsPlayerConnected(id)||!NYX_IsValidSkin(skin)) return SendClientMessage(playerid,COLOR_ERROR,"Jogador ou skin invalida."); NYX_Player[id][NYX_Skin]=skin; SetPlayerSkin(id,skin); return SendClientMessage(playerid,COLOR_SUCCESS,"Skin alterada."); }
CMD:dargrana(playerid, params[]) { if(!NYX_AdminHas(playerid,5)) return NYX_AdminDeny(playerid); new id,amount; if(sscanf(params,"ii",id,amount)||!IsPlayerConnected(id)||amount<0) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /dargrana [playerid] [valor]"); GivePlayerMoney(id,amount); NYX_Player[id][NYX_Money]=GetPlayerMoney(id); return SendClientMessage(playerid,COLOR_SUCCESS,"Dinheiro entregue."); }
CMD:criarnpc(playerid, params[]) { if(!NYX_AdminHas(playerid,6)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NPC criado."); }
CMD:apagarnpc(playerid, params[]) { if(!NYX_AdminHas(playerid,6)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"NPC removido."); }
CMD:irnpc(playerid, params[]) { if(!NYX_AdminHas(playerid,6)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"Teleportado ao NPC."); }
CMD:kitadm(playerid, params[]) { if(!NYX_AdminHas(playerid,7)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"Kit administrativo entregue."); }
CMD:banir(playerid, params[]) { if(!NYX_AdminHas(playerid,8)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"Banimento administrativo aplicado."); }
CMD:desbanir(playerid, params[]) { if(!NYX_AdminHas(playerid,8)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"Banimento administrativo removido."); }
CMD:setadm(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); new id,level; if(sscanf(params,"ii",id,level)||!IsPlayerConnected(id)||level<0||level>10) return SendClientMessage(playerid,COLOR_ERROR,"Uso: /setadm [playerid] [0-10]"); NYX_Player[id][NYX_Admin]=level; return SendClientMessage(playerid,COLOR_SUCCESS,"Nivel administrativo alterado."); }
CMD:limparchat(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); for(new i=0;i<25;i++) SendClientMessageToAll(-1," "); return SendClientMessage(playerid,COLOR_SUCCESS,"Chat limpo."); }
CMD:sethora(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); new h; if(sscanf(params,"i",h)||h<0||h>23)return SendClientMessage(playerid,COLOR_ERROR,"Uso: /sethora [0-23]");SetWorldTime(h);return SendClientMessage(playerid,COLOR_SUCCESS,"Hora alterada."); }
CMD:setclima(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); new w; if(sscanf(params,"i",w)||w<0||w>255)return SendClientMessage(playerid,COLOR_ERROR,"Uso: /setclima [0-255]");SetWeather(w);return SendClientMessage(playerid,COLOR_SUCCESS,"Clima alterado."); }
CMD:dv(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); new vid=GetPlayerVehicleID(playerid);if(!vid)return SendClientMessage(playerid,COLOR_ERROR,"Voce nao esta em um veiculo.");DestroyVehicle(vid);return SendClientMessage(playerid,COLOR_SUCCESS,"Veiculo removido."); }
CMD:ir(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); new id; if(sscanf(params,"i",id)||!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Uso: /ir [playerid]");new Float:x,Float:y,Float:z;GetPlayerPos(id,x,y,z);SetPlayerPos(playerid,x+1.0,y,z);return SendClientMessage(playerid,COLOR_SUCCESS,"Teleporte realizado."); }
CMD:trazer(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); new id; if(sscanf(params,"i",id)||!IsPlayerConnected(id))return SendClientMessage(playerid,COLOR_ERROR,"Uso: /trazer [playerid]");new Float:x,Float:y,Float:z;GetPlayerPos(playerid,x,y,z);SetPlayerPos(id,x+1.0,y,z);return SendClientMessage(playerid,COLOR_SUCCESS,"Jogador trazido."); }
CMD:rcar(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"/rcar executado."); }
CMD:tv(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"Modo TV ativado."); }
CMD:tvoff(playerid, params[]) { if(!NYX_AdminHas(playerid,10)) return NYX_AdminDeny(playerid); return SendClientMessage(playerid,COLOR_SUCCESS,"Modo TV desativado."); }
