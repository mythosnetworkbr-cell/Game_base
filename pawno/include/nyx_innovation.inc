#if defined _NYX_INNOVATION_INCLUDED
    #endinput
#endif
#define _NYX_INNOVATION_INCLUDED

#define NYX_EVENT_COUNT 8
new const NYX_EventName[NYX_EVENT_COUNT][] = {
    "Corrida Oficial NYX", "Caixa Misteriosa", "Feira de Veiculos", "Leilao Relampago",
    "Festival da Cidade", "Operacao Policial", "Regata NYX", "Copa de Pesca"
};

// Sistemas planejados para virar gameplay persistente:
// 1. Cidade viva: eventos mudam o movimento e os pontos ativos do mapa.
// 2. Veiculo com identidade: desgaste, manutencao, tuning e historico.
// 3. Mercado dinamico: precos de carros e imoveis variam conforme oferta/demanda.
// 4. Reputacao: jogador evolui por profissao, organizacao e atividades legais.
// 5. Familia: patrimonio, residencia, garagem e cofre compartilhados.
// 6. Temporadas: eventos com ranking e recompensas cosméticas.
// 7. Pesca competitiva: torneios com peso/tamanho dos peixes e ranking.
// 8. Leiloes publicos: casas, carros e itens com tempo limitado.
// 9. Identidade social: cinema, shopping, igreja, restaurantes e empregos conectados.
// 10. NCoins: somente itens premium definidos pelo catalogo oficial.

stock NYX_ShowInnovations(playerid)
{
    new list[1536], line[128]; list[0] = EOS;
    for(new i=0;i<NYX_EVENT_COUNT;i++)
    {
        format(line,sizeof line,"%s\n",NYX_EventName[i]);
        strcat(list,line,sizeof list);
    }
    return ShowPlayerDialog(playerid,1203,DIALOG_STYLE_LIST,"NYX | EXPERIENCIAS",list,"VER","FECHAR");
}
