#if defined _NYX_MOBILE_INCLUDED
    #endinput
#endif
#define _NYX_MOBILE_INCLUDED

// NYX Mobile capability/protocol contract.
// This does not replace SA-MP networking; it exposes a stable set of
// server-side values and commands for a future native/mobile bridge.

#define NYX_MOBILE_PROTOCOL_VERSION 1
#define NYX_MOBILE_CAP_HUD        (1 << 0)
#define NYX_MOBILE_CAP_TOUCH      (1 << 1)
#define NYX_MOBILE_CAP_MAP        (1 << 2)
#define NYX_MOBILE_CAP_INVENTORY  (1 << 3)
#define NYX_MOBILE_CAP_PHONE      (1 << 4)
#define NYX_MOBILE_CAP_VEHICLES   (1 << 5)
#define NYX_MOBILE_CAP_JOBS       (1 << 6)
#define NYX_MOBILE_CAP_ORGS       (1 << 7)

stock NYX_MobileCapabilities()
{
    return NYX_MOBILE_CAP_HUD
        | NYX_MOBILE_CAP_TOUCH
        | NYX_MOBILE_CAP_MAP
        | NYX_MOBILE_CAP_INVENTORY
        | NYX_MOBILE_CAP_PHONE
        | NYX_MOBILE_CAP_VEHICLES
        | NYX_MOBILE_CAP_JOBS
        | NYX_MOBILE_CAP_ORGS;
}

stock NYX_MobileBuildState(playerid, output[], size)
{
    new job[48], org[64];
    NYX_GetJobName(NYX_PlayerJob[playerid], job, sizeof job);
    format(org, sizeof org, "Civil");
    if (NYX_IsValidOrg(NYX_Player[playerid][NYX_Org]))
        format(org, sizeof org, "%s", NYX_OrgName[NYX_Player[playerid][NYX_Org]]);

    return format(output, size,
        "NYX1|state|logged=%d|skin=%d|money=%d|ncoins=%d|org=%d|rank=%d|admin=%d|job=%d|job_name=%s|org_name=%s",
        NYX_Player[playerid][NYX_Logged],
        NYX_Player[playerid][NYX_Skin],
        GetPlayerMoney(playerid),
        NYX_Player[playerid][NYX_NCoins],
        NYX_Player[playerid][NYX_Org],
        NYX_Player[playerid][NYX_OrgRank],
        NYX_Player[playerid][NYX_Admin],
        NYX_PlayerJob[playerid],
        job,
        org
    );
}
