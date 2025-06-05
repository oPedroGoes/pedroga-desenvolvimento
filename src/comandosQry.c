#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comandosQry.h"
#include "formas.h"
#include "geometria.h"

typedef struct{

    // Para selr e seli.
    FILE* arqTxt;
    SmuTreap tree;
    Lista lista_anotacoes_svg;
    Lista *array_selecoes;



} qryContext;

//------------------------------------------------------------HANDLE_SELR----------------------------------------------------------------//

// Função de callback para percorreLista (para processar nós selecionados)
void processaNoParaSaidaSelr(Item item_node, void *aux_context){
    if(!item_node || !aux_context){
        fprintf(stderr, "(processaNoParaSaidaSelr) Erro: parametros invalidos.");
        return;
    }

    qryContext* context = (qryContext*)aux_context;
    Node nd = (Node)item_node; // O Item da lista é o Node da SmuTreap
    Info forma = getInfoSmuT(context->tree, nd);
    DescritorTipoInfo tipo = getTypeInfoSmuT(context->tree, nd);

    double xa, ya; // Âncora da FORMA para o círculo SVG
    double xl, yl; // Equivalentes para x2 e y2 para linhas.

    int id_forma = get_idF(forma, tipo);
    const char *nome_forma = get_NameStrF(tipo);

    int ancoraCount = get_anchorF(forma, tipo, &xa, &ya, &xl, &yl);

    // Escrever no arquivo TXT
    fprintf(context->arqTxt, "selr: forma %d (%s) selecionada.\n", id_forma, nome_forma);

    // Adicionar anotação para SVG: pequena circunferência vermelha na âncora da forma
    char* anota_ancora_svg = (char*)malloc(200 * sizeof(char));
    if(!anota_ancora_svg){
        fprintf(stderr, "(processaNoParaSaidaSeli) Erro: falha ao alocar memoria.");
        return;
    } 

    if(ancoraCount == 2){
        sprintf(anota_ancora_svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />\n<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />", xa, ya, xl, yl);
        insereNaLista(context->lista_anotacoes_svg, (Item)anota_ancora_svg);
    } else if(ancoraCount == 1){
        sprintf(anota_ancora_svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />", xa, ya);
        insereNaLista(context->lista_anotacoes_svg, (Item)anota_ancora_svg); 
    } else return;

    return;
}

//ADICIONAR NO TAD FORMAS!!!!!!!!!!!!
// Função de callback para getInfosDentroRegiaoSmuT: verifica se uma forma está totalmente contida na região
bool formaTotalmenteContidaCallback(SmuTreap t, Node n_node, Info forma_info, double reg_x1, double reg_y1, double reg_x2, double reg_y2) {
    DescritorTipoInfo tipo_forma = getTypeInfoSmuT(t, n_node);

    if (tipo_forma == TIPO_CIRCULO || tipo_forma == TIPO_RETANGULO || tipo_forma == TIPO_TEXTO || tipo_forma == TIPO_LINHA ){
        double ndx;
    double ndy;
    double ndw;
    double ndh;
    getBoundingBoxSmuT(t, n_node, &ndx, &ndy, &ndw, &ndh);

    double ndxMax = ndx + ndw;
    double ndyMax = ndy + ndh;

    return (retanguloInternoRetangulo(ndy, ndx, ndxMax, ndyMax, reg_x1, reg_y1, reg_x2, reg_y2));
    } else return false; // Tipo desconhecido.
}

void handle_selr(CONTEXTO ctxt, int selId, double sel_x, double sel_y, double sel_w, double sel_h) {
    if(!ctxt){
        fprintf(stderr, "(hanfle_selr) Erro: parametro invalido.\n");
        return;
    }

    qryContext *contexto = (qryContext*)ctxt;

    // Escreve o comando original no arquivo TXT
    fprintf(contexto->arqTxt, "[*] selr %d %.2f %.2f %.2f %.2f\n", selId, sel_x, sel_y, sel_w, sel_h);
    printf("Processando comando selr: n=%d, x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", selId, sel_x, sel_y, sel_w, sel_h);

    // Cria uma lista para armazenar os nós (formas) selecionados nesta operação
    Lista formasEncontradas_selr = criaLista();
    if (!formasEncontradas_selr) {
        fprintf(contexto->arqTxt, "(handle_selr) Erro: Falha ao alocar memoria para lista de selecao.\n");
        return;
    }

    // Define (x1, y1) e (x2, y2) como pontos opostos.
    double reg_x1 = sel_x;
    double reg_y1 = sel_y;
    double reg_x2 = sel_x + sel_w;
    double reg_y2 = sel_y + sel_h;

    // Adiciona as formas dentro da lista.
    getInfosDentroRegiaoSmuT(contexto->tree, reg_x1, reg_y1, reg_x2, reg_y2, formaTotalmenteContidaCallback, formasEncontradas_selr);

    // Adiciona a lista no array de selecoes.
    if (selId >= 0 && selId < 100){

        // Se selecao n ja' existe, apaga-se a existente, e a substitui.
        if (contexto->array_selecoes[selId] != NULL){
            destroiLista(contexto->array_selecoes[selId], NULL);
        }

        contexto->array_selecoes[selId] = formasEncontradas_selr;
    } else {
        fprintf(contexto->arqTxt, "(handle_selr) Erro: Numero de identificacao de selecao 'n' (%d) invalido. Deve ser entre 0 e 99.\n", selId);
        destroiLista(formasEncontradas_selr, NULL);
        return;
    }

    // 5. Gerar a saída TXT e preparar anotações SVG para as formas selecionadas
    if (listaEstaVazia(formasEncontradas_selr)) {
        fprintf(contexto->arqTxt, "Nenhuma forma inteiramente contida na regiao especificada.\n");
    } else {
        // Percorre a lista de nós selecionados para gerar saída TXT e anotações SVG
        percorreLista(formasEncontradas_selr, processaNoParaSaidaSelr, &contexto);
    }

    // Adiciona anotação SVG para o retângulo de seleção
    char* anota_selec_r = (char*)malloc(256 * sizeof(char));
    if(!anota_selec_r){
        fprintf(stderr, "(handle_selr) Erro: falha ao alocar memoria.");
        return;
    } else{
        sprintf(anota_selec_r, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" stroke=\"red\" fill=\"none\" stroke-dasharray=\"5.5\" />",
                sel_x, sel_y, sel_w, sel_h);

        insereNaLista(contexto->lista_anotacoes_svg, (Item)anota_selec_r);
    }
    // Nota: A lista 'formasEncontradas_selr' agora é de responsabilidade do 'array_selecoes[selId]'.
    // Não a destrua aqui se 'selId' for válido.


    //1. Colocar array de selecoes na MAIN
    //2. Inicializar uma lista de anotacoes para svg na MAIN
    //3. LEMBRAR DE DAR KILL NAS ANOTACOES!!!!!!!!
}

//---------------------------------------------------------------------------------------------------------------------------------------//






