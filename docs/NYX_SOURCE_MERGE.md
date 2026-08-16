# NYX ROLEPLAY — Source Merge Map

The supplied RP bases are treated as reference implementations. Compiled AMX files are not decompiled; compatible functionality is reimplemented in NYX Pawn modules.

## Supplied sources

- Galaxy-RP BYLHZIN
  - org/corps commands, food/cafe, workshop, black-market flows, gang/morro/FAB interactions, GPS, effects, mobile/editor support.
- Brasil Paradise City RPG
  - garages, businesses, houses, furniture, elevator systems, vehicle utilities, tutorial/status systems and persistent economy patterns.
- GM Patriota
  - the same large RP foundation plus mobile include, RakNet hooks, garages, companies, houses, furniture and organization assets.
- GM do Dejavu
  - robbery/invasion flows, fire/medical assistance, vehicle theft, dynamic checkpoints, organization movement and gameplay timers.
- GM do Play Vicio vFENIX
  - legacy RP/vehicle/organization foundations and server-side utilities.
- StyleReal
  - weapon factory, drugs, family, garage, SAMU, mechanic organization, player restraint and death flows.
- South City v1.0
  - garage, needs, passport, notifications, interaction and Discord-oriented modular structure.
- Maldivas City Roleplay — SAMP Android
  - map modules, gangzones, player textdraws, map text, NPC routes and Android/mobile-oriented assets.
- Projeto Cidade Alta Mobile
  - MySQL-oriented persistence, DDCMD, ColAndreas, actors/objects, mobile/editor filterscripts and Android/mobile compatibility patterns.
- After City — RibeiroScrit
  - authentication/bot integration, contacts/conversations/number storage and server configuration patterns.

## Integrated NYX layer

`pawno/include/nyx_legacy.inc` is the compatibility layer. It currently reimplements and connects the first cross-source feature set:

- needs: hunger, thirst and stress;
- passport issuance/status;
- mobile profile flag;
- garage vehicle registration/spawn;
- business catalog;
- robbery cooldown/reward;
- player lifecycle initialization and periodic needs tick.

The layer is wired through `nyx_config.inc` and `nyx_player.inc`, so it is part of the NYX runtime rather than a documentation-only placeholder.

## Rule

A feature only counts toward NYX completion after it is integrated into the NYX code path and validated by the Pawn build. Source archives and AMX files are references, not completion by themselves.
