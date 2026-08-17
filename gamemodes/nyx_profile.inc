#if defined _NYX_PROFILE_INCLUDED
    #endinput
#endif
#define _NYX_PROFILE_INCLUDED

new bool:NYX_ProfileExists[MAX_PLAYERS];
new NYX_ProfileAge[MAX_PLAYERS];
new NYX_ProfileCity[MAX_PLAYERS];
new NYX_ProfileFight[MAX_PLAYERS];
new NYX_ProfileDiscord[MAX_PLAYERS][96];
new NYX_ProfileEmail[MAX_PLAYERS][160];

stock NYX_ProfileInit()
{
    if(!NYX_DBReady) return 0;
    db_query(NYX_DB, "CREATE TABLE IF NOT EXISTS profile (name TEXT PRIMARY KEY, age INTEGER DEFAULT 18, city INTEGER DEFAULT 0, fightmode INTEGER DEFAULT 0, discord TEXT DEFAULT '', email TEXT DEFAULT '')");
    return 1;
}

stock NYX_ProfileLoad(playerid)
{
    if(!NYX_DBReady) return 0;
    new name[MAX_PLAYER_NAME + 1], esc[64], query[256];
    GetPlayerName(playerid, name, sizeof name);
    NYX_SQLQuote(name, esc, sizeof esc);
    format(query, sizeof query, "SELECT age,city,fightmode,discord,email FROM profile WHERE name='%s' LIMIT 1", esc);
    new DBResult:r = db_query(NYX_DB, query);
    NYX_ProfileExists[playerid] = false;
    if(r == DBResult:0) return 0;
    if(db_num_rows(r) > 0)
    {
        NYX_ProfileAge[playerid] = db_get_field_assoc_int(r, "age");
        NYX_ProfileCity[playerid] = db_get_field_assoc_int(r, "city");
        NYX_ProfileFight[playerid] = db_get_field_assoc_int(r, "fightmode");
        db_get_field_assoc(r, "discord", NYX_ProfileDiscord[playerid], sizeof NYX_ProfileDiscord[]);
        db_get_field_assoc(r, "email", NYX_ProfileEmail[playerid], sizeof NYX_ProfileEmail[]);
        NYX_ProfileExists[playerid] = true;
    }
    db_free_result(r);
    return 1;
}

stock NYX_ProfileSave(playerid)
{
    if(!NYX_DBReady || !NYX_AccountExists[playerid]) return 0;
    new name[MAX_PLAYER_NAME + 1], escName[64], escDiscord[192], escEmail[320], query[768];
    GetPlayerName(playerid, name, sizeof name);
    NYX_SQLQuote(name, escName, sizeof escName);
    NYX_SQLQuote(NYX_ProfileDiscord[playerid], escDiscord, sizeof escDiscord);
    NYX_SQLQuote(NYX_ProfileEmail[playerid], escEmail, sizeof escEmail);
    format(query, sizeof query, "INSERT OR REPLACE INTO profile (name,age,city,fightmode,discord,email) VALUES ('%s',%d,%d,%d,'%s','%s')", escName, NYX_ProfileAge[playerid], NYX_ProfileCity[playerid], NYX_ProfileFight[playerid], escDiscord, escEmail);
    new DBResult:r = db_query(NYX_DB, query);
    if(r != DBResult:0) db_free_result(r);
    return r != DBResult:0;
}

stock NYX_ProfileReset(playerid)
{
    NYX_ProfileExists[playerid] = false;
    NYX_ProfileAge[playerid] = 18;
    NYX_ProfileCity[playerid] = 0;
    NYX_ProfileFight[playerid] = 0;
    NYX_ProfileDiscord[playerid][0] = EOS;
    NYX_ProfileEmail[playerid][0] = EOS;
    return 1;
}
