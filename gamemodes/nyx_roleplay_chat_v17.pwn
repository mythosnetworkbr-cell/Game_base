/*
 * NYX ROLEPLAY - CHAT INTEGRATION STAGING
 *
 * This file marks the systems consolidated from the Game Mode work in ChatGPT.
 * The production source remains gamemodes/nyx_roleplay.pwn until the staged
 * implementation has been compiled against this repository's exact includes.
 *
 * Integrated requirements tracked here:
 * - PlayerTextDraw login/registration screen
 * - Clickable password field with password masking
 * - Registration validation before account creation
 * - Multi-step character registration
 * - Fight mode: Normal / Boxing
 * - Age validation: 13..100
 * - Birth city: Los Santos / Las Venturas / San Fierro
 * - Optional Discord and e-mail recovery fields
 * - Final account creation only after all required fields are complete
 * - Login on subsequent connections
 * - TextDraw mouse selection state management
 * - SQLite account persistence
 * - Economy, bank, jobs, hunger/thirst, vehicles, organizations,
 *   administration, logs, properties and RP utility commands
 *
 * IMPORTANT: Do not treat this staging marker as the compiled production GM.
 */

#define NYX_CHAT_INTEGRATION_V17 1
#define NYX_AUTH_REGISTER 1
#define NYX_AUTH_LOGIN 1
#define NYX_CHARACTER_SETUP 1
#define NYX_CITY_LS 0
#define NYX_CITY_LV 1
#define NYX_CITY_SF 2
#define NYX_MIN_AGE 13
#define NYX_MAX_AGE 100
