#if defined _NYX_PROPERTIES_INCLUDED
    #endinput
#endif
#define _NYX_PROPERTIES_INCLUDED

#define NYX_FOOD_COUNT 8
#define NYX_PROPERTY_COUNT 12
#define NYX_AUCTION_COUNT 6
#define NYX_FAMILY_COUNT 20

new const NYX_FoodName[NYX_FOOD_COUNT][] = {
    "Lanchonete NYX", "Burger House", "Pastelaria Central", "Pizzaria NYX",
    "Espetinho do Centro", "Cafeteria NYX", "Restaurante Brasil", "Drive Thru NYX"
};

new const NYX_PropertyName[NYX_PROPERTY_COUNT][] = {
    "Casa Popular", "Casa Familiar", "Casa Premium", "Apartamento Central",
    "Cobertura NYX", "Mansao Jardim", "Mansao Praia", "Mansao Florianopolis",
    "Mansao Blumenau", "Penthouse NYX", "Residencia Executiva", "Mansao Presidencial"
};
new const NYX_PropertyPrice[NYX_PROPERTY_COUNT] = {
    50000, 120000, 250000, 350000, 750000, 1200000,
    1800000, 2200000, 1600000, 3000000, 4500000, 8000000
};

new const NYX_AuctionName[NYX_AUCTION_COUNT][] = {
    "Mansao Sunset", "Mansao Lago", "Mansao Marina", "Cobertura Skyline",
    "Mansao Serra", "Residencia Imperial"
};
new const NYX_AuctionStart[NYX_AUCTION_COUNT] = {1500000,1800000,2200000,2500000,1400000,5000000};
new NYX_AuctionBid[NYX_AUCTION_COUNT];
new NYX_AuctionBidder[NYX_AUCTION_COUNT];

new NYX_FamilyId[MAX_PLAYERS];
new const NYX_FamilyName[NYX_FAMILY_COUNT][] = {
    "Familia Silva", "Familia Santos", "Familia Oliveira", "Familia Souza",
    "Familia Costa", "Familia Lima", "Familia Almeida", "Familia Rocha",
    "Familia Ferreira", "Familia Gomes", "Familia Ribeiro", "Familia Carvalho",
    "Familia Martins", "Familia Barbosa", "Familia Dias", "Familia Nascimento",
    "Familia Araujo", "Familia Teixeira", "Familia Correia", "Familia NYX"
};

stock NYX_ShowFood(playerid)
{
    new list[1024], line[100]; list[0]=EOS;
    for(new i=0;i<NYX_FOOD_COUNT;i++){format(line,sizeof line,"%s\n",NYX_FoodName[i]);strcat(list,line,sizeof list);}
    return ShowPlayerDialog(playerid,1100,DIALOG_STYLE_LIST,"NYX | LANCHONETES",list,"ENTRAR","FECHAR");
}

stock NYX_ShowProperties(playerid)
{
    new list[1536], line[128]; list[0]=EOS;
    for(new i=0;i<NYX_PROPERTY_COUNT;i++){format(line,sizeof line,"%s | $%d\n",NYX_PropertyName[i],NYX_PropertyPrice[i]);strcat(list,line,sizeof list);}
    return ShowPlayerDialog(playerid,1101,DIALOG_STYLE_LIST,"NYX | IMOVEIS",list,"VER","FECHAR");
}

stock NYX_ShowAuction(playerid)
{
    new list[1024], line[128]; list[0]=EOS;
    for(new i=0;i<NYX_AUCTION_COUNT;i++){format(line,sizeof line,"%s | Lance inicial $%d | Atual $%d\n",NYX_AuctionName[i],NYX_AuctionStart[i],NYX_AuctionBid[i]);strcat(list,line,sizeof list);}
    return ShowPlayerDialog(playerid,1102,DIALOG_STYLE_LIST,"NYX | LEILAO DE MANSOES",list,"DAR LANCE","FECHAR");
}

stock NYX_ShowFamilies(playerid)
{
    new list[1536], line[96]; list[0]=EOS;
    for(new i=0;i<NYX_FAMILY_COUNT;i++){format(line,sizeof line,"%d. %s\n",i+1,NYX_FamilyName[i]);strcat(list,line,sizeof list);}
    return ShowPlayerDialog(playerid,1103,DIALOG_STYLE_LIST,"NYX | FAMILIAS",list,"ESCOLHER","FECHAR");
}
