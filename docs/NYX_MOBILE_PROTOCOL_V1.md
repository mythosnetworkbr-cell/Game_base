# NYX Mobile Protocol v1

## Objetivo

Contrato comum entre `Game_base`, cliente Godot e futuro bridge nativo SA-MP/RakNet. O transporte é separado do estado: ENet/RPC serve apenas ao protótipo; o cliente SA-MP final deverá alimentar o mesmo modelo de estado.

## Frequências

- Player input/state: 20 Hz
- Remote interpolation target: 10–20 Hz
- Vehicle snapshots: 10 Hz
- Reliable events: login, spawn, vehicle create/remove, inventory, money and job/org changes

## Player state

Campos mínimos:

```text
playerId
sequence
position(x,y,z)
rotationY
velocity(x,y,z)
health
armour
skin
money
bank
jobId
organizationId
organizationRank
adminLevel
```

## Vehicle state

```text
vehicleId
sequence
modelId
position(x,y,z)
rotation(x,y,z)
velocity(x,y,z)
driverPlayerId
health
fuel
color1
color2
```

## Eventos

```text
HELLO
AUTH_REQUEST
AUTH_RESULT
CHARACTER_LIST
CHARACTER_SELECT
SPAWN
PLAYER_STATE
VEHICLE_CREATE
VEHICLE_STATE
VEHICLE_REMOVE
CHAT_MESSAGE
MONEY_UPDATE
INVENTORY_UPDATE
JOB_UPDATE
ORG_UPDATE
DISCONNECT
```

## Segurança

O servidor/GameMode é autoritativo para dinheiro, inventário, emprego, organização, saúde, veículos persistentes e permissões. O cliente nunca deve ser tratado como fonte de verdade desses campos.

## Compatibilidade

Este documento não afirma que ENet seja compatível com SA-MP/RakNet. O bridge nativo será responsável por converter o protocolo do cliente real para este contrato.
