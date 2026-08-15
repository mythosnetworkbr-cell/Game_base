# NYX ROLEPLAY — Master Consolidation

## Production strategy

The NYX GameMode is being built from the user's submitted GameModes rather than by blindly concatenating them.

### Readable source bases audited
- New City Roleplay — primary functional base
- Galaxy RP — secondary system library
- GM Patriota — secondary system library

### Sources requiring source/password access
- After City — RibeiroScrit
- GM DO PLAY VICIO vFENIX

## Architecture rule

Only one implementation of each SA-MP/open.mp callback may exist in the production GameMode. Systems from secondary bases must be adapted behind the NYX architecture to avoid duplicate callbacks, globals, dialogs, commands and conflicting dependencies.

## Current integration targets

- Accounts and persistence
- Character/profile
- Inventory
- Jobs and routes
- Organizations and factions
- Police/DETRAN
- Vehicles
- Houses/properties
- Businesses
- Weapons/ammunition
- Families/marriage
- Events
- GPS/cellphone
- Android/mobile support
- VOIP
- Anti-crash/security
- Maps/dynamic objects
- Administration
- TextDraw/dialog UI
- Economy/NCoins

## Verification rule

No existing AMX is treated as a verified NYX build. The final AMX must be produced from the final NYX source with the target Pawn/open.mp toolchain and its compiler output checked.
