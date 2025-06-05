#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comandosQry.h"

void handle_selr(SmuTreap tree, FILE* pathTxtOut, int n_id_selecao, double sel_x, double sel_y, double sel_w, double sel_h, Lista* array_selecoes, Lista lista_anotacoes_svg) {
    // Escreve o comando original no arquivo TXT
    fprintf(pathTxtOut, "[*] selr %d %.2f %.2f %.2f %.2f\n", n_id_selecao, sel_x, sel_y, sel_w, sel_h);
    printf("Processando comando selr: n=%d, x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", n_id_selecao, sel_x, sel_y, sel_w, sel_h);

    // Cria uma lista para armazenar os nós (formas) selecionados nesta operação
    Lista formasEncontradas_selr = criaLista();
    if (!formasEncontradas_selr) {
        fprintf(pathTxtOut, "(handle_selr) Erro: Falha ao alocar memoria para lista de selecao.\n");
        return;
    }

    // Define (x1, y1) e (x2, y2) como pontos opostos.
    double reg_x1 = sel_x;
    double reg_y1 = sel_y;
    double reg_x2 = sel_x + sel_w;
    double reg_y2 = sel_y + sel_h;

    // Adiciona as formas dentro da lista.
    getInfosDentroRegiaoSmuT(tree, reg_x1, reg_y1, reg_x2, reg_y2, formaTotalmenteContidaCallback, formasEncontradas_selr);

    // Adiciona a lista no array de selecoes.
    if (n_id_selecao >= 0 && n_id_selecao < 100){

        // Se selecao n ja' existe, apaga-se a existente, e a substitui.
        if (array_selecoes[n_id_selecao] != NULL){
            destroiLista(array_selecoes[n_id_selecao], NULL);
        }

        array_selecoes[n_id_selecao] = formasEncontradas_selr;
    } else {
        fprintf(pathTxtOut, "(handle_selr) Erro: Numero de identificacao de selecao 'n' (%d) invalido. Deve ser entre 0 e 99.\n", n_id_selecao);
        destroiLista(formasEncontradas_selr, NULL);
        return;
    }

    // 5. Gerar a saída TXT e preparar anotações SVG para as formas selecionadas
    if (listaEstaVazia(formasEncontradas_selr)) {
        fprintf(pathTxtOut, "Nenhuma forma inteiramente contida na regiao especificada.\n");
    } else {
        InfoSelrData data;
        data.pathTxt = pathTxtOut;
        data.smutreap_tree = tree;
        data.lista_anotacoes_svg = lista_anotacoes_svg;

        // Percorre a lista de nós selecionados para gerar saída TXT e anotações SVG
        percorreLista(formasEncontradas_selr, processaNoParaSaidaSelr, &data);
    }

    // Adiciona anotação SVG para o retângulo de seleção
    char* anota_selec_r = (char*)malloc(256 * sizeof(char));
    if(!anota_selec_r){
        fprintf(stderr, "(handle_selr) Erro: falha ao alocar memoria.");
        return;
    } else{
        sprintf(anota_selec_r, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" stroke=\"red\" fill=\"none\" stroke-dasharray=\"5.5\" />",
                sel_x, sel_y, sel_w, sel_h);

        insereNaLista(lista_anotacoes_svg, (Item)anota_selec_r);
    }
    // Nota: A lista 'formasEncontradas_selr' agora é de responsabilidade do 'array_selecoes[n_id_selecao]'.
    // Não a destrua aqui se 'n_id_selecao' for válido.


    //1. Colocar array de selecoes na MAIN
    //2. Inicializar uma lista de anotacoes para svg na MAIN
    //3. LEMBRAR DE DAR KILL NAS ANOTACOES!!!!!!!!