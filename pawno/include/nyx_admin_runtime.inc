#if defined _NYX_ADMIN_RUNTIME_INCLUDED
    #endinput
#endif
#define _NYX_ADMIN_RUNTIME_INCLUDED

stock NYX_AdminLog(playerid, const action[])
{
    new name[MAX_PLAYER_NAME + 1], ip[16], stamp[32], line[256], file;
    GetPlayerName(playerid, name, sizeof name);
    GetPlayerIp(playerid, ip, sizeof ip);
    new year, month, day, hour, minute, second;
    getdate(year, month, day);
    gettime(hour, minute, second);
    format(stamp, sizeof stamp, "%04d-%02d-%02d %02d:%02d:%02d", year, month, day, hour, minute, second);
    format(line, sizeof line, "[%s] staff_id=%d staff=%s ip=%s action=%s\r\n", stamp, playerid, name, ip, action);
    file = fopen("scriptfiles/nyx_admin.log", io_append);
    if (file) { fwrite(file, line); fclose(file); }
    return 1;
}

stock NYX_AdminGodDamage(playerid)
{
    if (!GetPVarType(playerid, "NYX_GOD")) return 0;
    SetPlayerHealth(playerid, 99999.0);
    return 1;
}

stock NYX_AdminGodReset(playerid)
{
    if (GetPVarType(playerid, "NYX_GOD"))
    {
        DeletePVar(playerid, "NYX_GOD");
        SetPlayerHealth(playerid, 100.0);
    }
    return 1;
}
