#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "comandosQry.h"
#include "formas.h"
#include "geometria.h"
#include "boundingBox.h"

typedef struct{

    // Para todos.
    FILE* arqTxt;
    SmuTreap tree;
    Lista lista_anotacoes_svg;
    Lista *array_selecoes;



} qryContext;

void killAnotacaoCallback(char *anotacao){
    if(!anotacao){
        fprintf(stderr, "(killAnotacaoCallback) Erro: parametros invalidos.");
    } else free(anotacao);
}

CONTEXTO iniciaContext(FILE *arqTxt, SmuTreap t, Lista lista_anotacoes_svg, Lista *array_selecoes){
    qryContext *ctxt = (qryContext*)malloc(sizeof(qryContext));
    if(!ctxt){
        fprintf(stderr, "(iniciaContext) Erro: falha ao alocar memoria para struct.\n");
        return NULL;
    }

    ctxt->arqTxt = arqTxt;
    ctxt->tree = t;
    ctxt->lista_anotacoes_svg = lista_anotacoes_svg;
    ctxt->array_selecoes = array_selecoes;

    return ctxt;
}

//------------------------------------------------------------HANDLE_SELR----------------------------------------------------------------//

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
        fprintf(stderr, "(processaNoParaSaidaSelr) Erro: falha ao alocar memoria.");
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


//------------------------------------------------------------HANDLE_SELI----------------------------------------------------------------//

void processaNoParaSaidaSeli(Item item_node, void *aux_context){
    if(!item_node || !aux_context){
        fprintf(stderr, "(processaNoParaSaidaSeli) Erro: parametros invalidos.");
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
    fprintf(context->arqTxt, "seli: forma %d (%s) selecionada.\n", id_forma, nome_forma);

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

void handle_seli(CONTEXTO ctxt, int n_id_selecao, double sel_x, double sel_y){
    if(!ctxt){
        fprintf(stderr, "(hanfle_selr) Erro: parametro invalido.\n");
        return;
    }

    qryContext *contexto = (qryContext*)ctxt;

    // Escreve o comando original no arquivo TXT
    fprintf(contexto->arqTxt, "[*] seli %d %.2f %.2f\n", n_id_selecao, sel_x, sel_y);
    printf("Processando comando seli: n=%d, x=%.2f, y=%.2f\n", n_id_selecao, sel_x, sel_y);

    Lista formasEncontradas_seli = criaLista();
    if (!formasEncontradas_seli) {
    fprintf(contexto->arqTxt, "(handle_seli) Erro: Falha ao alocar memoria para lista de selecao.\n");
    return;
    }

/*-------------------------------------------------------------DEBUG SVG----------------------------------------------------------------------------//

    char *nome_svg_debug = "/home/pedro/estrutura_de_dados_ii/projetoAVALIA1/pedroga-desenvolvimento/saidas/00-vaso-simples/TESTE_SELI_DEBUG.svg";
    FILE *debug_svg = fopen(nome_svg_debug, "w");
    if(!debug_svg){
        fprintf(stderr, "(handle_seli) Erro: falha ao criar arquivo para debug do svd");
        return;
    }

    fprintf(debug_svg, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(debug_svg, "<svg width=\"2000\" height=\"2000\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");

    visitaProfundidadeSmuT(t, escreverFormaSvg1, debug_svg);

    fprintf(debug_svg, "</svg>\n");
    fclose(debug_svg);

//--------------------------------------------------------------------------------------------------------------------------------------------------*/

    Node no_encontrado = getNodeSmuT(contexto->tree, sel_x, sel_y); // Como getNode tem impacto sobre a prioridade, optei por faze-lo apenas se tudo estiver valido.
    if(!no_encontrado){
        fprintf(contexto->arqTxt, "Nenhuma forma encontrada na coordenada (%.2f, %.2f) com a tolerancia epsilon.\n", sel_x, sel_y);
        return;  // A mensagem já aparece de getNode.  
    } 
    insereNaLista(formasEncontradas_seli, (Item)no_encontrado);



    // Adiciona a lista no array de selecoes se o id selecionado for valido.
    if (n_id_selecao >= 0 && n_id_selecao < 100){
    
        // Se selecao n ja' existe, apaga-se a existente, e a substitui.
        if (contexto->array_selecoes[n_id_selecao] != NULL){
            destroiLista(contexto->array_selecoes[n_id_selecao], NULL);
        }
        contexto->array_selecoes[n_id_selecao] = formasEncontradas_seli;

    } else {
        fprintf(contexto->arqTxt, "(handle_seli) Erro: Numero de identificacao de selecao 'n' (%d) invalido. Deve ser entre 0 e 99.\n", n_id_selecao);
        // Lógica para reportar a forma encontrada (sem armazenar na seleção inválida 'n')
        Info info_forma = getInfoSmuT(contexto->tree, no_encontrado);
        DescritorTipoInfo tipo_forma = getTypeInfoSmuT(contexto->tree, no_encontrado);
        int id_forma_report = -1;
        char* nome_tipo_report = NULL;

        if (tipo_forma == TIPO_CIRCULO) { id_forma_report = get_idC((CIRCLE)info_forma); nome_tipo_report = "circulo"; }
        else if (tipo_forma == TIPO_RETANGULO) { id_forma_report = get_idR((RECTANGLE)info_forma); nome_tipo_report = "retangulo"; }
        else if (tipo_forma == TIPO_LINHA) { id_forma_report = get_idL((LINHA)info_forma); nome_tipo_report = "linha"; }
        else if (tipo_forma == TIPO_TEXTO) { id_forma_report = get_idT((TEXTO)info_forma); nome_tipo_report = "texto"; }

        fprintf(contexto->arqTxt, "seli: forma %d (%s) selecionada. (aviso: 'n' de selecao invalido, forma nao armazenada na selecao)\n", id_forma_report, nome_tipo_report);

        // Adicionar anotação SVG mesmo com 'n' inválido, pois a forma foi buscada e encontrada.
        char* anot_svg_circ_ancora_report = (char*)malloc(200 * sizeof(char));
        if (anot_svg_circ_ancora_report) {
            sprintf(anot_svg_circ_ancora_report, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />", sel_x, sel_y);
            insereNaLista(contexto->lista_anotacoes_svg, (Item)anot_svg_circ_ancora_report);
        }
        return;
    }

    percorreLista(formasEncontradas_seli, processaNoParaSaidaSeli, contexto);
    return;
}

//---------------------------------------------------------------------------------------------------------------------------------------//




