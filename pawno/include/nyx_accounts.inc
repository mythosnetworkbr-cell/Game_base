#if defined _NYX_ACCOUNTS_INCLUDED
    #endinput
#endif
#define _NYX_ACCOUNTS_INCLUDED

new DB:NYX_DB;
new bool:NYX_DBReady;
new bool:NYX_AccountLoaded[MAX_PLAYERS];
new bool:NYX_AccountExists[MAX_PLAYERS];
new NYX_AccountName[MAX_PLAYERS][MAX_PLAYER_NAME + 1];
new NYX_AccountPassword[MAX_PLAYERS][65];

stock NYX_SQLQuote(const input[], output[], size)
{
    new j = 0;
    for (new i = 0; input[i] != EOS && j < size - 1; i++)
    {
        if (input[i] == 39)
        {
            if (j + 2 >= size) break;
            output[j++] = 39;
            output[j++] = 39;
        }
        else output[j++] = input[i];
    }
    output[j] = EOS;
    return 1;
}

stock NYX_AccountsInit()
{
    NYX_DB = db_open("nyx_accounts.db");
    if (NYX_DB == DB:0)
    {
        NYX_DBReady = false;
        print("[NYX][DB] Falha ao abrir nyx_accounts.db");
        return 0;
    }
    db_query(NYX_DB, "CREATE TABLE IF NOT EXISTS accounts (name TEXT PRIMARY KEY, password TEXT NOT NULL, money INTEGER NOT NULL DEFAULT 5000, ncoins INTEGER NOT NULL DEFAULT 0, skin INTEGER NOT NULL DEFAULT 1, org INTEGER NOT NULL DEFAULT -1, org_rank INTEGER NOT NULL DEFAULT 0, admin INTEGER NOT NULL DEFAULT 0)");
    NYX_DBReady = true;
    print("[NYX][DB] SQLite inicializado.");
    return 1;
}

stock NYX_AccountsExit()
{
    if (NYX_DBReady)
    {
        db_close(NYX_DB);
        NYX_DBReady = false;
    }
    return 1;
}

stock NYX_GetPlayerNameSafe(playerid, output[], size)
{
    GetPlayerName(playerid, output, size);
    return 1;
}

stock NYX_LoadAccount(playerid)
{
    if (!NYX_DBReady) return 0;
    new name[MAX_PLAYER_NAME + 1], escaped[(MAX_PLAYER_NAME * 2) + 3], query[256];
    NYX_GetPlayerNameSafe(playerid, name, sizeof name);
    NYX_SQLQuote(name, escaped, sizeof escaped);
    format(query, sizeof query, "SELECT password,money,ncoins,skin,org,org_rank,admin FROM accounts WHERE name='%s' LIMIT 1", escaped);
    new DBResult:result = db_query(NYX_DB, query);
    if (result == DBResult:0) return 0;
    NYX_AccountLoaded[playerid] = false;
    NYX_AccountExists[playerid] = false;
    if (db_num_rows(result) > 0)
    {
        db_get_field_assoc(result, "password", NYX_AccountPassword[playerid], sizeof NYX_AccountPassword[]);
        NYX_Player[playerid][NYX_Money] = db_get_field_assoc_int(result, "money");
        NYX_Player[playerid][NYX_NCoins] = db_get_field_assoc_int(result, "ncoins");
        NYX_Player[playerid][NYX_Skin] = db_get_field_assoc_int(result, "skin");
        NYX_Player[playerid][NYX_Org] = db_get_field_assoc_int(result, "org");
        NYX_Player[playerid][NYX_OrgRank] = db_get_field_assoc_int(result, "org_rank");
        NYX_Player[playerid][NYX_Admin] = db_get_field_assoc_int(result, "admin");
        NYX_AccountExists[playerid] = true;
    }
    db_free_result(result);
    NYX_AccountLoaded[playerid] = true;
    return 1;
}

stock NYX_CheckPassword(playerid, const password[])
{
    if (!NYX_AccountExists[playerid]) return 0;
    new hash[65];
    SHA256_PassHash(password, NYX_AccountName[playerid], hash, sizeof hash);
    return !strcmp(hash, NYX_AccountPassword[playerid], true);
}

stock NYX_CreateAccount(playerid, const password[])
{
    if (!NYX_DBReady) return 0;
    new name[MAX_PLAYER_NAME + 1], escapedName[(MAX_PLAYER_NAME * 2) + 3], escapedHash[140], hash[65], query[512];
    GetPlayerName(playerid, name, sizeof name);
    format(NYX_AccountName[playerid], sizeof NYX_AccountName[], "%s", name);
    NYX_SQLQuote(name, escapedName, sizeof escapedName);
    SHA256_PassHash(password, name, hash, sizeof hash);
    NYX_SQLQuote(hash, escapedHash, sizeof escapedHash);
    format(query, sizeof query, "INSERT INTO accounts (name,password,money,ncoins,skin,org,org_rank,admin) VALUES ('%s','%s',%d,0,%d,-1,0,0)", escapedName, escapedHash, NYX_START_MONEY, NYX_Player[playerid][NYX_Skin]);
    new DBResult:result = db_query(NYX_DB, query);
    if (result == DBResult:0) return 0;
    db_free_result(result);
    NYX_AccountExists[playerid] = true;
    format(NYX_AccountPassword[playerid], sizeof NYX_AccountPassword[], "%s", hash);
    NYX_Player[playerid][NYX_Money] = NYX_START_MONEY;
    NYX_Player[playerid][NYX_NCoins] = 0;
    NYX_Player[playerid][NYX_Org] = -1;
    NYX_Player[playerid][NYX_OrgRank] = 0;
    NYX_Player[playerid][NYX_Admin] = 0;
    return 1;
}

stock NYX_SaveAccount(playerid)
{
    if (!NYX_DBReady || !NYX_AccountExists[playerid]) return 0;
    new name[MAX_PLAYER_NAME + 1], escaped[(MAX_PLAYER_NAME * 2) + 3], query[512];
    GetPlayerName(playerid, name, sizeof name);
    NYX_SQLQuote(name, escaped, sizeof escaped);
    format(query, sizeof query, "UPDATE accounts SET money=%d,ncoins=%d,skin=%d,org=%d,org_rank=%d,admin=%d WHERE name='%s'", GetPlayerMoney(playerid), NYX_Player[playerid][NYX_NCoins], NYX_Player[playerid][NYX_Skin], NYX_Player[playerid][NYX_Org], NYX_Player[playerid][NYX_OrgRank], NYX_Player[playerid][NYX_Admin], escaped);
    new DBResult:result = db_query(NYX_DB, query);
    if (result != DBResult:0) db_free_result(result);
    return result != DBResult:0;
}

stock NYX_ResetAccountState(playerid)
{
    NYX_AccountLoaded[playerid] = false;
    NYX_AccountExists[playerid] = false;
    NYX_AccountName[playerid][0] = EOS;
    NYX_AccountPassword[playerid][0] = EOS;
    return 1;
}
