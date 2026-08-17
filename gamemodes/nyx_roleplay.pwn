#include <a_samp>
#include <nyx_config>
#include <nyx_player>
#include <nyx_admin_chat_migrated>
#include <nyx_medical>
#include <nyx_orgs>
#include <nyx_jobs>
#include <nyx_marriage>
#include <nyx_ncoins>
#include <nyx_world>
#include <nyx_properties>
#include <nyx_graphics>
#include <nyx_rp_complete>
#include <nyx_admin_complete>

#define D_LOGIN 1000
#define D_REGISTER 1001
#define D_SKINS 1002
#define D_ORGS 1003
#define D_HELP 1004
#define D_JOBS 1005
#define D_STORE 1006
#define D_BANK 1007
#define D_PROFILE 1008

new NYX_PendingMarriage[MAX_PLAYERS];
