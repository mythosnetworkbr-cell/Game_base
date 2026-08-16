#if defined _NYX_GRAPHICS_INCLUDED
    #endinput
#endif
#define _NYX_GRAPHICS_INCLUDED

// Perfil visual pensado para SA-MP/mobile: qualidade sem forcar filtros que borram modelos.
#define NYX_GRAPHICS_PROFILE "NYX_HIGH_CLARITY"
#define NYX_TEXTURE_PROFILE "HIGH"
#define NYX_MODEL_CLARITY "NATIVE_HIGH"
#define NYX_DISABLE_ARTIFICIAL_BLUR 1
#define NYX_USE_HIGH_DETAIL_ASSETS 1
#define NYX_MOBILE_LOD_BALANCED 1

// A GameMode nao altera a resolucao/textura do cliente diretamente.
// O pacote mobile/launcher deve fornecer os assets de alta qualidade.
stock NYX_ApplyGraphicsProfile(playerid)
{
    SetPlayerSkillLevel(playerid,WEAPONSKILL_PISTOL,100);
    return 1;
}

#include <nyx_police>
