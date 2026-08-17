# NYX Mobile Asset Pipeline

## Formatos GTA SA

| Arquivo | Função | Mobile |
|---|---|---|
| `.dff` | modelo 3D | obrigatório quando usado pelo cliente GTA |
| `.txd` | textura | obrigatório para materiais do modelo |
| `.col` | colisão | necessário para colisão customizada |
| `.ide` | definição de objetos/modelos | map loader |
| `.ipl` | instâncias do mapa | map loader |
| `.map` | fonte de placement de algumas ferramentas | converter para placement final |

## Ordem de importação

1. Inventariar assets e hashes.
2. Validar nomes/model IDs.
3. Separar mundo base, interiores e props.
4. Converter placements para o formato usado pelo cliente.
5. Validar colisões.
6. Gerar LODs quando necessário.
7. Compactar texturas e remover duplicatas.
8. Testar em ARM64.
9. Só então distribuir pelo launcher.

## Pacotes recebidos para o NYX

Os pacotes de favela, casas, Nike, postes, retetexturização, `Fixed_Models`, `sfix`, `sfeship1` e gráficos entram como fontes de assets. Eles não devem ser importados cegamente: cada `.dff/.txd/.col` precisa passar pela etapa de inventário e validação.

## Meta Android

- ARM64 como arquitetura principal.
- streaming por região;
- textura com resolução adequada ao dispositivo;
- LOD para objetos distantes;
- colisão simplificada onde possível;
- limite de draw calls por região;
- fallback sem assets opcionais.
