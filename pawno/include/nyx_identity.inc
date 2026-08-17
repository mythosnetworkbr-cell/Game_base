#if defined _NYX_IDENTITY_INCLUDED
    #endinput
#endif
#define _NYX_IDENTITY_INCLUDED

#define NYX_AUTH_NAME 1200
#define NYX_AUTH_PASSWORD 1201
#define NYX_AUTH_MAX_NAME 24
#define NYX_AUTH_MAX_PASSWORD 63
#define NYX_AUTH_MAX_SCAN 10000

enum E_NYX_IDENTITY
{
    NYX_IdentityId,
    NYX_IdentityLoaded
};

new NYX_Identity[MAX_PLAYERS][E_NYX_IDENTITY];
new NYX_IdentityName[MAX_PLAYERS][NYX_AUTH_MAX_NAME + 1];
new NYX_IdentityIP[MAX_PLAYERS][46];
new NYX_IdentityPassword[MAX_PLAYERS][NYX_AUTH_MAX_PASSWORD + 1];
new NYX_AuthStage[MAX_PLAYERS];

stock NYX_IdentityReset(playerid)
{
    NYX_Identity[playerid][NYX_IdentityId]=0;
    NYX_Identity[playerid][NYX_IdentityLoaded]=0;
    NYX_IdentityName[playerid][0]=EOS;
    NYX_IdentityIP[playerid][0]=EOS;
    NYX_IdentityPassword[playerid][0]=EOS;
    NYX_AuthStage[playerid]=0;
    return 1;
}

stock NYX_IdentityFile(accountid,path[],size)
{
    format(path,size,"scriptfiles/nyx_accounts_%d.dat",accountid);
    return 1;
}

stock NYX_IdentityValidateName(const name[])
{
    new len=strlen(name);
    if(len<3 || len>NYX_AUTH_MAX_NAME) return 0;
    for(new i=0;i<len;i++)
    {
        new c=name[i];
        if(!((c>='A' && c<='Z') || (c>='a' && c<='z') || (c>='0' && c<='9') || c=='_')) return 0;
    }
    return 1;
}

stock NYX_IdentityFindByName(const name[])
{
    new path[64],line[128],file;
    for(new id=1;id<=NYX_AUTH_MAX_SCAN;id++)
    {
        NYX_IdentityFile(id,path,sizeof path);
        if(!fexist(path)) break;
        file=fopen(path,io_read);
        if(!file) continue;
        fread(file,line,sizeof line);
        fclose(file);
        if(!strcmp(line,name,true)) return id;
    }
    return 0;
}

stock NYX_IdentityNextId()
{
    new id=1,path[64];
    for(id=1;id<=NYX_AUTH_MAX_SCAN;id++)
    {
        NYX_IdentityFile(id,path,sizeof path);
        if(!fexist(path)) return id;
    }
    return 0;
}

stock NYX_IdentityWriteCore(playerid,file)
{
    new line[128];
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_Money]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Bank[playerid]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_Skin]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_NCoins]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_Org]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_OrgRank]); fwrite(file,line);
    format(line,sizeof line,"%d",NYX_Player[playerid][NYX_Admin]); fwrite(file,line);
    for(new i=0;i<NYX_INV_SLOTS;i++)
    {
        format(line,sizeof line,"%d",NYX_InvItem[playerid][i]); fwrite(file,line);
        format(line,sizeof line,"%d",NYX_InvAmount[playerid][i]); fwrite(file,line);
    }
    for(new slot=0;slot<NYX_VEHICLE_SLOTS;slot++)
    {
        format(line,sizeof line,"%d",NYX_VehicleOwnedModel[playerid][slot]); fwrite(file,line);
        format(line,sizeof line,"%d",NYX_VehicleColor1[playerid][slot]); fwrite(file,line);
        format(line,sizeof line,"%d",NYX_VehicleColor2[playerid][slot]); fwrite(file,line);
        format(line,sizeof line,"%.2f",NYX_VehicleFuel[playerid][slot]); fwrite(file,line);
        format(line,sizeof line,"%.2f",NYX_VehicleHealth[playerid][slot]); fwrite(file,line);
    }
    return 1;
}

stock NYX_IdentitySave(playerid)
{
    if(!NYX_Identity[playerid][NYX_IdentityLoaded]) return 0;
    new ip[46],path[64],line[128],file;
    GetPlayerIp(playerid,ip,sizeof ip);
    format(NYX_IdentityIP[playerid],sizeof NYX_IdentityIP[],"%s",ip);
    NYX_IdentityFile(NYX_Identity[playerid][NYX_IdentityId],path,sizeof path);
    file=fopen(path,io_write);
    if(!file) return 0;
    format(line,sizeof line,"%d",NYX_Identity[playerid][NYX_IdentityId]); fwrite(file,line);
    fwrite(file,NYX_IdentityName[playerid]);
    fwrite(file,NYX_IdentityPassword[playerid]);
    fwrite(file,NYX_IdentityIP[playerid]);
    NYX_IdentityWriteCore(playerid,file);
    fclose(file);
    return 1;
}

stock NYX_IdentityLoad(playerid,const name[],const password[])
{
    new id=NYX_IdentityFindByName(name);
    if(id<=0) return 0;
    new path[64],line[128],file;
    NYX_IdentityFile(id,path,sizeof path);
    file=fopen(path,io_read);
    if(!file) return 0;
    fread(file,line,sizeof line);
    fread(file,line,sizeof line);
    if(strcmp(line,name,true)!=0){fclose(file);return 0;}
    fread(file,line,sizeof line);
    if(strcmp(line,password,false)!=0){fclose(file);return -1;}
    format(NYX_IdentityPassword[playerid],sizeof NYX_IdentityPassword[],"%s",password);
    format(NYX_IdentityName[playerid],sizeof NYX_IdentityName[],"%s",name);
    fread(file,line,sizeof line); format(NYX_IdentityIP[playerid],sizeof NYX_IdentityIP[],"%s",line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_Money]=strval(line);
    fread(file,line,sizeof line); NYX_Bank[playerid]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_Skin]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_NCoins]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_Org]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_OrgRank]=strval(line);
    fread(file,line,sizeof line); NYX_Player[playerid][NYX_Admin]=strval(line);
    NYX_InventoryReset(playerid);
    for(new i=0;i<NYX_INV_SLOTS;i++)
    {
        fread(file,line,sizeof line); NYX_InvItem[playerid][i]=strval(line);
        fread(file,line,sizeof line); NYX_InvAmount[playerid][i]=strval(line);
    }
    NYX_VehicleReset(playerid);
    for(new slot=0;slot<NYX_VEHICLE_SLOTS;slot++)
    {
        fread(file,line,sizeof line); NYX_VehicleOwnedModel[playerid][slot]=strval(line);
        fread(file,line,sizeof line); NYX_VehicleColor1[playerid][slot]=strval(line);
        fread(file,line,sizeof line); NYX_VehicleColor2[playerid][slot]=strval(line);
        fread(file,line,sizeof line); NYX_VehicleFuel[playerid][slot]=floatstr(line);
        fread(file,line,sizeof line); NYX_VehicleHealth[playerid][slot]=floatstr(line);
    }
    fclose(file);
    NYX_Identity[playerid][NYX_IdentityId]=id;
    NYX_Identity[playerid][NYX_IdentityLoaded]=1;
    NYX_AccountLoaded[playerid]=true;
    format(NYX_AccountPassword[playerid],sizeof NYX_AccountPassword[],"%s",password);
    return 1;
}

stock NYX_IdentityCreate(playerid,const name[],const password[])
{
    if(!NYX_IdentityValidateName(name)) return 0;
    if(strlen(password)<4 || strlen(password)>NYX_AUTH_MAX_PASSWORD) return 0;
    if(NYX_IdentityFindByName(name)>0) return 0;
    new id=NYX_IdentityNextId();
    if(id<=0) return 0;
    format(NYX_IdentityName[playerid],sizeof NYX_IdentityName[],"%s",name);
    format(NYX_IdentityPassword[playerid],sizeof NYX_IdentityPassword[],"%s",password);
    GetPlayerIp(playerid,NYX_IdentityIP[playerid],sizeof NYX_IdentityIP[]);
    NYX_Identity[playerid][NYX_IdentityId]=id;
    NYX_Identity[playerid][NYX_IdentityLoaded]=1;
    NYX_AccountLoaded[playerid]=true;
    format(NYX_AccountPassword[playerid],sizeof NYX_AccountPassword[],"%s",password);
    NYX_Player[playerid][NYX_Money]=NYX_START_MONEY;
    NYX_Bank[playerid]=NYX_BANK_START;
    NYX_Player[playerid][NYX_NCoins]=0;
    NYX_Player[playerid][NYX_Org]=-1;
    NYX_Player[playerid][NYX_OrgRank]=0;
    NYX_Player[playerid][NYX_Admin]=0;
    NYX_InventoryReset(playerid);
    NYX_VehicleReset(playerid);
    NYX_InventoryGiveStarter(playerid);
    NYX_IssuePassport(playerid);
    return NYX_IdentitySave(playerid);
}

stock NYX_IdentityNameValidForPlayer(playerid)
{
    return NYX_Identity[playerid][NYX_IdentityLoaded];
}

stock NYX_IdentityShowNameDialog(playerid)
{
    NYX_AuthStage[playerid]=1;
    return ShowPlayerDialog(playerid,NYX_AUTH_NAME,DIALOG_STYLE_INPUT,"NYX ROLEPLAY | LOGIN","Digite seu nome de conta.\n\nUse de 3 a 24 caracteres: letras, numeros e _.","ENTRAR","REGISTRAR");
}

stock NYX_IdentityShowLoginPassword(playerid)
{
    NYX_AuthStage[playerid]=2;
    return ShowPlayerDialog(playerid,NYX_AUTH_PASSWORD,DIALOG_STYLE_PASSWORD,"NYX ROLEPLAY | LOGIN","Conta encontrada.\n\nAgora digite sua senha.","ENTRAR","VOLTAR");
}

stock NYX_IdentityShowRegisterPassword(playerid)
{
    NYX_AuthStage[playerid]=4;
    return ShowPlayerDialog(playerid,NYX_AUTH_PASSWORD,DIALOG_STYLE_PASSWORD,"NYX ROLEPLAY | REGISTRO","Conta nova para este nome.\n\nCrie uma senha com pelo menos 4 caracteres.","CRIAR","VOLTAR");
}

stock NYX_IdentityRegisterName(playerid,const name[])
{
    if(!NYX_IdentityValidateName(name))
        return SendClientMessage(playerid,COLOR_ERROR,"Nome invalido. Use 3-24 caracteres, apenas letras, numeros e _.");
    if(NYX_IdentityFindByName(name)>0)
        return NYX_IdentityShowLoginPassword(playerid);
    format(NYX_IdentityName[playerid],sizeof NYX_IdentityName[],"%s",name);
    NYX_AuthStage[playerid]=3;
    return NYX_IdentityShowRegisterPassword(playerid);
}
