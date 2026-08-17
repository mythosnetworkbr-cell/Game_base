# NYX Mobile Client Layer

Camada Android complementar do NYX. A GameMode SA-MP continua em Pawn e permanece autoritativa.

## Estrutura

- `client/godot/` — cliente 3D/mobile atual e protótipo de sincronização.
- `client/mobile/android/` — contratos Android/Kotlin para a integração nativa futura.
- `docs/NYX_MOBILE_PROTOCOL_V1.md` — contrato de estado/eventos.
- `docs/NYX_MOBILE_ASSET_PIPELINE.md` — pipeline dos assets GTA SA.

## Responsabilidades Android

- launcher e atualização de assets;
- verificação de versão;
- instalação/distribuição de conteúdo cliente;
- integração com o bridge nativo;
- diagnóstico local;
- suporte ao HUD e controles mobile.

## Responsabilidades do servidor

Pawn continua responsável por login, personagens, dinheiro, banco, inventário, empregos, organizações, propriedades, veículos persistentes, administração e regras de RP.

## Bridge nativo

`NyxNativeBridge.kt` define o contrato para uma futura implementação JNI/NDK/SA-MP Mobile. Ele **não** implementa RakNet por si só. A implementação nativa deverá converter o protocolo real do cliente SA-MP Mobile para o contrato NYX.

## Skins

Skins customizadas continuam sendo assets do cliente. A GameMode seleciona os IDs; o launcher/cliente distribui os arquivos visuais correspondentes.

IDs iniciais NYX:
- 1 — Mendigo masculino
- 2 — Mendigo feminino
