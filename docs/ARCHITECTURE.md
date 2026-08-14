# NYX ROLEPLAY — Arquitetura

## Regra principal

A GameMode que roda no servidor SA-MP/open.mp continua sendo **Pawn**. Java/Kotlin não substituem o Pawn do servidor.

## Stack

### Pawn
Núcleo do servidor:
- jogadores
- organizações
- economia
- empregos
- veículos
- casas/empresas
- comandos
- dialogs/TextDraw
- persistência e regras de RP

### Java/Kotlin
Camada de cliente Android/mobile, quando houver cliente/launcher próprio:
- launcher
- autenticação visual
- download/atualização de assets
- instalação de skins/modificações do cliente
- HUD mobile e integração com APIs do cliente
- telemetria não sensível e diagnóstico

### C/C++
Quando necessário para integração nativa do cliente:
- bibliotecas nativas
- bridge JNI/NDK
- otimização de renderização/arquivos
- integração específica com o cliente SA-MP mobile

### SQL
Persistência:
- contas
- personagens
- dinheiro
- organizações
- cargos
- veículos
- propriedades
- empresas
- inventário
- logs administrativos

## Skins

A GameMode usa os IDs nativos disponíveis no ambiente SA-MP. O servidor controla o ID com `SetPlayerSkin`.

- ID 1 = personagem inicial masculino, definido pela NYX como mendigo masculino
- ID 2 = personagem inicial feminino, definido pela NYX como mendigo feminino
- catálogo configurado: 0–311

Para skins visualmente modificadas, os arquivos de cliente (`.dff/.txd` e equivalentes do cliente mobile) pertencem ao pacote do cliente/launcher. A GameMode não consegue alterar a textura 3D nativa apenas com Pawn.

## Objetivo

Uma GameMode única e modular, com cliente mobile opcional separado, compartilhando o mesmo protocolo e regras de servidor. Isso evita colocar código Android dentro da GameMode e mantém compatibilidade com SA-MP.
