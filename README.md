# NYX ROLEPLAY

Gamemode oficial da NYX Roleplay / Mythøs Network.

Build CI da GameMode NYX habilitado para compilação Pawn 3.10.11.

## Arquitetura

- `gamemodes/nyx_roleplay.pwn` — núcleo da gamemode
- `pawno/include/nyx_*.inc` — módulos NYX
- `.github/workflows/build-nyx.yml` — build automático do AMX

## Estado atual

A branch `nyx-build-test` contém persistência local de contas/personagens, economia com banco, empregos, organizações, casamento, propriedades, leilões, famílias, NCoins e administração em evolução. O CI também instala as dependências Pawn e SA-MP necessárias para validar o AMX.
