#if defined _NYX_INVENTORY_INCLUDED
    #endinput
#endif
#define _NYX_INVENTORY_INCLUDED

#define NYX_INV_SLOTS 24
#define NYX_INV_NAME_LEN 24

new NYX_InvItem[MAX_PLAYERS][NYX_INV_SLOTS];
new NYX_InvAmount[MAX_PLAYERS][NYX_INV_SLOTS];
new const NYX_InvNames[][] = {
    "Vazio", "Agua", "Comida", "Kit medico", "Celular", "Documento",
    "Pescado", "Combustivel", "Mochila", "Ferramenta", "Colete", "Chave"
};
new const NYX_InvPrice[] = {0,25,40,120,300,50,60,150,500,180,800,75};

stock NYX_InventoryReset(playerid)
{
    for(new i=0;i<NYX_INV_SLOTS;i++){NYX_InvItem[playerid][i]=0;NYX_InvAmount[playerid][i]=0;}
    return 1;
}

stock NYX_InventoryFind(playerid,itemid)
{
    for(new i=0;i<NYX_INV_SLOTS;i++) if(NYX_InvItem[playerid][i]==itemid && NYX_InvAmount[playerid][i]>0) return i;
    return -1;
}

stock NYX_InventoryAdd(playerid,itemid,amount)
{
    if(itemid<=0 || itemid>=sizeof(NYX_InvNames) || amount<=0) return 0;
    new slot=NYX_InventoryFind(playerid,itemid);
    if(slot==-1)
    {
        for(new i=0;i<NYX_INV_SLOTS;i++) if(NYX_InvItem[playerid][i]==0){slot=i;break;}
    }
    if(slot==-1) return 0;
    NYX_InvItem[playerid][slot]=itemid;
    NYX_InvAmount[playerid][slot]+=amount;
    return 1;
}

stock NYX_InventoryRemove(playerid,itemid,amount)
{
    new slot=NYX_InventoryFind(playerid,itemid);
    if(slot==-1 || amount<=0 || NYX_InvAmount[playerid][slot]<amount) return 0;
    NYX_InvAmount[playerid][slot]-=amount;
    if(NYX_InvAmount[playerid][slot]==0) NYX_InvItem[playerid][slot]=0;
    return 1;
}

stock NYX_InventoryBuy(playerid,itemid,amount)
{
    if(itemid<=0 || itemid>=sizeof(NYX_InvNames) || amount<=0 || amount>20) return 0;
    new total=NYX_InvPrice[itemid]*amount;
    if(total<=0 || GetPlayerMoney(playerid)<total) return 0;
    if(!NYX_InventoryAdd(playerid,itemid,amount)) return 0;
    GivePlayerMoney(playerid,-total);
    NYX_SyncMoney(playerid);
    NYX_SaveAccount(playerid);
    new msg[128];
    format(msg,sizeof msg,"Compra concluida: %s x%d por $%d.",NYX_InvNames[itemid],amount,total);
    SendClientMessage(playerid,COLOR_SUCCESS,msg);
    return 1;
}

stock NYX_InventoryShow(playerid)
{
    new list[2048],line[96];
    list[0]=EOS;
    for(new i=0;i<NYX_INV_SLOTS;i++)
    {
        if(NYX_InvItem[playerid][i]>0)
        {
            format(line,sizeof line,"Slot %d: %s x%d\n",i+1,NYX_InvNames[NYX_InvItem[playerid][i]],NYX_InvAmount[playerid][i]);
            strcat(list,line,sizeof list);
        }
    }
    if(!strlen(list)) format(list,sizeof list,"Inventario vazio.\n\nUse /compraragua, /comprarcomida ou /comprarkit.");
    return ShowPlayerDialog(playerid,1010,DIALOG_STYLE_MSGBOX,"NYX | INVENTARIO",list,"FECHAR","");
}

stock NYX_InventoryUse(playerid,itemid)
{
    if(!NYX_InventoryRemove(playerid,itemid,1)) return 0;
    switch(itemid)
    {
        case 1: SetPlayerHealth(playerid,100.0);
        case 2: SetPlayerHealth(playerid,100.0);
        case 3: SetPlayerHealth(playerid,100.0);
        case 4: SendClientMessage(playerid,COLOR_NYX,"Celular aberto: use /ajuda para os servicos NYX.");
        case 5: SendClientMessage(playerid,COLOR_NYX,"Documento conferido.");
        case 6: SendClientMessage(playerid,COLOR_SUCCESS,"Pescado consumido.");
        case 7: SendClientMessage(playerid,COLOR_SUCCESS,"Combustivel preparado para uso.");
        case 8: SendClientMessage(playerid,COLOR_SUCCESS,"Mochila equipada.");
        case 9: SendClientMessage(playerid,COLOR_SUCCESS,"Ferramenta equipada.");
        case 10: SetPlayerArmour(playerid,50.0);
        case 11: SendClientMessage(playerid,COLOR_SUCCESS,"Chave utilizada.");
    }
    return 1;
}

stock NYX_InventoryGiveStarter(playerid)
{
    NYX_InventoryReset(playerid);
    NYX_InventoryAdd(playerid,1,2);
    NYX_InventoryAdd(playerid,2,2);
    NYX_InventoryAdd(playerid,4,1);
    NYX_InventoryAdd(playerid,5,1);
    return 1;
}

stock NYX_InventoryCommands(playerid,cmdtext[])
{
    if(!strcmp(cmdtext,"/inventario",true)) return NYX_InventoryShow(playerid);
    if(!strcmp(cmdtext,"/compraragua",true)) return NYX_InventoryBuy(playerid,1,1);
    if(!strcmp(cmdtext,"/comprarcomida",true)) return NYX_InventoryBuy(playerid,2,1);
    if(!strcmp(cmdtext,"/comprarkit",true)) return NYX_InventoryBuy(playerid,3,1);
    if(!strcmp(cmdtext,"/comprarcelular",true)) return NYX_InventoryBuy(playerid,4,1);
    if(!strcmp(cmdtext,"/comprarmochila",true)) return NYX_InventoryBuy(playerid,8,1);
    if(!strcmp(cmdtext,"/comprarcolete",true)) return NYX_InventoryBuy(playerid,10,1);
    if(!strcmp(cmdtext,"/usaritem 1",true)) return NYX_InventoryUse(playerid,1);
    if(!strcmp(cmdtext,"/usaritem 2",true)) return NYX_InventoryUse(playerid,2);
    if(!strcmp(cmdtext,"/usaritem 3",true)) return NYX_InventoryUse(playerid,3);
    if(!strcmp(cmdtext,"/usaritem 4",true)) return NYX_InventoryUse(playerid,4);
    if(!strcmp(cmdtext,"/usaritem 5",true)) return NYX_InventoryUse(playerid,5);
    if(!strcmp(cmdtext,"/usaritem 6",true)) return NYX_InventoryUse(playerid,6);
    if(!strcmp(cmdtext,"/usaritem 7",true)) return NYX_InventoryUse(playerid,7);
    if(!strcmp(cmdtext,"/usaritem 8",true)) return NYX_InventoryUse(playerid,8);
    if(!strcmp(cmdtext,"/usaritem 9",true)) return NYX_InventoryUse(playerid,9);
    if(!strcmp(cmdtext,"/usaritem 10",true)) return NYX_InventoryUse(playerid,10);
    if(!strcmp(cmdtext,"/usaritem 11",true)) return NYX_InventoryUse(playerid,11);
    return 0;
}
