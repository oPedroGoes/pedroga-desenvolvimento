#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "comandosQry.h"
#include "formas.h"
#include "geometria.h"
#include "boundingBox.h"

typedef struct{

    // Para todos.
    FILE* arqTxt;
    SmuTreap tree;
    double epsilon;
    Lista lista_anotacoes_svg;
    Lista *array_selecoes;
    int *idMax;
    FCalculaBoundingBox fCalcBB;

    // Para cln.
    double dx, dy;

    // Para cmflg
    char* cor_borda;
    char* cor_preenchimento;
    double largura_borda;


} qryContext;

void killAnotacaoCallback(char *anotacao){
    if(!anotacao){
        fprintf(stderr, "(killAnotacaoCallback) Erro: parametros invalidos.");
    } else free(anotacao);
}

CONTEXTO iniciaContext(FILE *arqTxt, SmuTreap t, Lista lista_anotacoes_svg, Lista *array_selecoes, int *idMax, FCalculaBoundingBox fCalcBB, double epsilon){
    qryContext *ctxt = (qryContext*)malloc(sizeof(qryContext));
    if(!ctxt){
        fprintf(stderr, "(iniciaContext) Erro: falha ao alocar memoria para struct.\n");
        return NULL;
    }

    ctxt->arqTxt = arqTxt;
    ctxt->tree = t;
    ctxt->epsilon = epsilon;
    ctxt->lista_anotacoes_svg = lista_anotacoes_svg;
    ctxt->array_selecoes = array_selecoes;
    ctxt->idMax = idMax;
    //printf("DEBUG (iniciaContext) id_clone = %d\n", *idMax);
    ctxt->fCalcBB = fCalcBB;

    ctxt->dx = 0;
    ctxt->dy = 0;

    return ctxt;
}

bool findFormByIdCallback(SmuTreap t, Node n, Info i, double x, double y, void *aux) {
    if(!t || !n || !i || !aux){
        fprintf(stderr, "(findFormByIdCallback) Erro: parametros invalidos.\n");
        return false;
    }

    if(*((int*)aux) < 0){
        fprintf(stderr, "(findFormByIdCallback) Erro: id (%d) que se busca e invalido.\n", *((int*)aux));
        exit(1);
        return false;
    }

    int id_procurado = *((int*) aux);
    int id_atual = get_idF(i, getTypeInfoSmuT(t, n));
    if (id_atual == id_procurado) {
        return true; // Encontrou
    }
    return false; // Não encontrou, continua a busca
}

bool buscaTipoSpy(Info info_forma, DescritorTipoInfo tipo_forma, Lista L, void *aux_context){
    if(!info_forma || !tipo_forma || !L || !aux_context){
       fprintf(stderr, "(buscaTipoSpy) Erro: parametros invalidos."); 
       return false;
    }

    qryContext *contexto = (qryContext*)aux_context;

    if (tipo_forma == TIPO_RETANGULO){
        RECTANGLE r = (RECTANGLE)info_forma;
        double rx = get_XR(r);
        double ry = get_YR(r);
        double rw = get_wR(r);
        double rh = get_hR(r);
        double rXMax = rx + rw;
        double rYMax = ry + rh;

        getInfosDentroRegiaoSmuT(contexto->tree, rx, ry, rXMax, rYMax, formaTotalmenteContidaCallback, L);
    } else if(tipo_forma == TIPO_TEXTO){
        TEXTO t = (TEXTO)info_forma;

        double tx = get_XT(t);
        double ty = get_YT(t);
        double epsilon = contexto->epsilon;

        // Define uma pequena região quadrada em torno da âncora do texto
        double x1 = tx - epsilon;
        double y1 = ty - epsilon;
        double x2 = tx + epsilon;
        double y2 = ty + epsilon;

        // Procura por TODAS as âncoras de nós dentro dessa pequena região, e as adiciona na lista.
        getNodesDentroRegiaoSmuT(contexto->tree, x1, y1, x2, y2, L);
    } else{
        fprintf(contexto->arqTxt, "Erro: O comando spy/cmflg so e aplicavel a formas de referencia do tipo retangulo ou texto.\n");
        return false;
    }

    return true;
}

//------------------------------------------------------------HANDLE_SELR----------------------------------------------------------------//

void processaNoParaSaidaSelr(Item item_node, void *aux_context){
    printf("\n\n DEBUG Entrando em processaNoParaSaidaSelr...\n");
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
    printf("DEBUG (handle_selr) id_clone = %d\n", *(contexto->idMax));

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
        percorreLista(formasEncontradas_selr, processaNoParaSaidaSelr, contexto);
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
    printf("\n\n DEBUG Entrando em processaNoParaSaidaSeli...\n");
    if(!item_node || !aux_context){
        fprintf(stderr, "\n(processaNoParaSaidaSeli) Erro: parametros invalidos.\n");
        return;
    }
    printf("DEBUG (processaNoParaSaidaSeli) item = %p\n", item_node);

    qryContext* context = (qryContext*)aux_context;
    Node nd = (Node)item_node; // O Item da lista é o Node da SmuTreap
    Info forma = getInfoSmuT(context->tree, nd);
    DescritorTipoInfo tipo = getTypeInfoSmuT(context->tree, nd);
    printf("DEBUG (processaNoParaSaidaSeli) tipo = %d\n", tipo);

    double x1_DEBUG, y1_DEBUG, x2_DEBUG, y2_DEBUG;
    int contaAncora = get_anchorF(forma, tipo, &x1_DEBUG, &y1_DEBUG, &x2_DEBUG, &y2_DEBUG);
    printf("DEBUG (processaNoParaSaidaSeli) contaAncora = %d x1 = %lf, y1 = %lf, x2 = %lf, y2 = %lf\n", contaAncora, x1_DEBUG, y1_DEBUG, x2_DEBUG, y2_DEBUG);

    double xa, ya; // Âncora da FORMA para o círculo SVG
    double xl, yl; // Equivalentes para x2 e y2 para linhas.

    int id_forma = get_idF(forma, tipo);
    printf("\nDEBUG (processaNoParaSaidaSeli) id_forma = %d", id_forma);
    const char *nome_forma = get_NameStrF(tipo);

    int ancoraCount = get_anchorF(forma, tipo, &xa, &ya, &xl, &yl);

    // Escrever no arquivo TXT
    fprintf(context->arqTxt, "seli: forma %d (%s) selecionada.\n", id_forma, nome_forma);

    // Adicionar anotação para SVG: pequena circunferência vermelha na âncora da forma
    char* anota_ancora_svg = (char*)malloc(200 * sizeof(char));
    if(!anota_ancora_svg){
        fprintf(stderr, "\n(processaNoParaSaidaSeli) Erro: falha ao alocar memoria.\n");
        return;
    } 

    if(ancoraCount == 2){
        sprintf(anota_ancora_svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />\n<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />", xa, ya, xl, yl);
        printf("\nInserindo anotacao na lista_anotacoes_svg para seli...\n");
        insereNaLista(context->lista_anotacoes_svg, (Item)anota_ancora_svg);
    } else if(ancoraCount == 1){
        printf("\nInserindo anotacao na lista_anotacoes_svg para seli...\n");
        sprintf(anota_ancora_svg, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />", xa, ya);
        insereNaLista(context->lista_anotacoes_svg, (Item)anota_ancora_svg); 
    } else return;

    return;
}

void handle_seli(CONTEXTO ctxt, int n_id_selecao, double sel_x, double sel_y){
    
    if(!ctxt){
        fprintf(stderr, "\n(hanfle_selr) Erro: parametro invalido.\n");
        return;
    }

    printf("\nProcessando comando seli: n=%d, x=%.2f, y=%.2f\n", n_id_selecao, sel_x, sel_y);

    qryContext *contexto = (qryContext*)ctxt;

    // Escreve o comando original no arquivo TXT
    fprintf(contexto->arqTxt, "[*] seli %d %.2f %.2f\n", n_id_selecao, sel_x, sel_y);


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
    printf("DEBUG ANTES (handle_seli) sel_x = %lf, sel_y = %lf\n", sel_x, sel_y);
    Node no_encontrado = getNodeSmuT(contexto->tree, sel_x, sel_y); // Como getNode tem impacto sobre a prioridade, optei por faze-lo apenas se tudo estiver valido.
    if(!no_encontrado){
        fprintf(contexto->arqTxt, "(seli) Nenhuma forma encontrada na coordenada (%.2f, %.2f) com a tolerancia epsilon.\n", sel_x, sel_y);
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


    printf("Processando saida para seli.\n");
    percorreLista(formasEncontradas_seli, processaNoParaSaidaSeli, contexto);
    return;
}

//---------------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------HANDLE_CLN-----------------------------------------------------------------//

void clonaEInsereCallback(Item item_origiNode, void *aux_context){
    printf("DEBUG Entrando em clonaEInsereCallback\n");
    if (!item_origiNode || !aux_context){
        fprintf(stderr, "(clonaEInsereCallback) Erro: parametros invalidos.\n");
        return;
    }

    qryContext *context = (qryContext*)aux_context;
    Node no_original = (Node)item_origiNode;
    Info info_original = getInfoSmuT(context->tree, no_original);
    DescritorTipoInfo tipo = getTypeInfoSmuT(context->tree, no_original);

    int id_original = get_idF(info_original, tipo);
    int id_clone = *(context->idMax);
    if(id_clone < 0) {printf("ide_clone eh menor que 1.\n"); exit(1);}
    Info info_clonada = NULL;
    double xAnch_clone, yAnch_clone;

    // Cria a info da forma clonada com base no tipo
    switch(tipo) {
        case TIPO_CIRCULO: {
            CIRCLE c_original = (CIRCLE)info_original;
            xAnch_clone = get_XC(c_original) + context->dx;
            yAnch_clone = get_YC(c_original) + context->dy;
            printf("Clonando circulo ID %d. Original-> corp:[%s], corb:[%s]\n", get_idC(c_original), get_cpC(c_original), get_cbC(c_original));
            info_clonada = create_circle(id_clone, xAnch_clone, yAnch_clone, get_rC(c_original), get_cpC(c_original), get_cbC(c_original));
            (*(context->idMax))++;
            break;
        }
        case TIPO_RETANGULO: {
            RECTANGLE r_original = (RECTANGLE)info_original;
            xAnch_clone = get_XR(r_original) + context->dx;
            yAnch_clone = get_YR(r_original) + context->dy;
            info_clonada = create_rectangle(id_clone, xAnch_clone, yAnch_clone, get_wR(r_original), get_hR(r_original), get_cpR(r_original), get_cbR(r_original));
            (*(context->idMax))++;
            break;
        }
        case TIPO_TEXTO: {
            TEXTO t_original = (TEXTO)info_original;
            xAnch_clone = get_XT(t_original) + context->dx;
            yAnch_clone = get_YT(t_original) + context->dy;
            info_clonada = cria_texto(id_clone, xAnch_clone, yAnch_clone, get_cpT(t_original), get_cbT(t_original), get_charancoraT(t_original), get_txtT(t_original), get_ffT(t_original), get_fwT(t_original), get_fsT(t_original));
            (*(context->idMax))++;
            break;
        }
        case TIPO_LINHA: {
            LINHA l_original = (LINHA)info_original;
            double x1l = get_X1L(l_original) + context->dx;
            double y1l = get_Y1L(l_original) + context->dy;
            double x2l = get_X2L(l_original) + context->dx;
            double y2l = get_Y2L(l_original) + context->dy;
            // Linha não troca cores, apenas translada
            info_clonada = cria_linha(id_clone, x1l, y1l, x2l, y2l, get_cL(l_original));

            // A âncora da linha é seu ponto inicial (menor X, ou menor Y para desempate)
            if (x1l < x2l || (x1l == x2l && y1l <= y2l)){
                xAnch_clone = x1l;
                yAnch_clone = y1l;
            } else {
                xAnch_clone = x2l;
                yAnch_clone = y2l;
            }
            (*(context->idMax))++;
            break;
        }
    }

    if (info_clonada){
        Node noClone = insertSmuT(context->tree, xAnch_clone, yAnch_clone, info_clonada, tipo, context->fCalcBB);
        if(noClone){
            fprintf(context->arqTxt, "cln: Forma ID %d clonada -> Novo clone ID %d.\n", id_original, id_clone);
        } else{
            fprintf(context->arqTxt, "cln: Falha ao inserir clone da forma ID %d na arvore.\n", id_original);
            killF(info_clonada, tipo);
        }
    }
}

void handle_cln(CONTEXTO ctxt, int n_id_selecao, double dx, double dy){
    if(!ctxt){
        fprintf(stderr, "(handle_cln) Erro: parametro invalido.\n");
        return;
    }

    printf("Processando comando cln: n=%d, dx=%.2lf, dy=%.2lf\n", n_id_selecao, dx, dy);

    qryContext *contexto = (qryContext*)ctxt;
    fprintf(contexto->arqTxt, "[*] cln %d %.2f %.2f\n", n_id_selecao, dx, dy);

    // Valida o índice da seleção e verifica se a lista existe
    if (n_id_selecao < 0 || n_id_selecao >= 100 || contexto->array_selecoes[n_id_selecao] == NULL) {
        fprintf(contexto->arqTxt, "Erro: Selecao 'n' (%d) e invalida ou nao existe.\n", n_id_selecao);
        return;
    }

    Lista cloneList = contexto->array_selecoes[n_id_selecao];
    if (listaEstaVazia(cloneList)) {
        fprintf(contexto->arqTxt, "Aviso: Selecao 'n' (%d) esta vazia. Nada a clonar.\n", n_id_selecao);
        return;
    }

    contexto->dx = dx;
    contexto->dy = dy;
                        printf("DEBUGDEBUDEBUGDEBUG\n");
    percorreLista(cloneList, clonaEInsereCallback, contexto);
}

//---------------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------HANDLE_CMFLG---------------------------------------------------------------//

void modificaCorELarguraCallback(Item item_node, void *aux_context){
    if(!item_node || !aux_context){
        fprintf(stderr, "(modificaCorELarguraCallback) Erro: parametros invalidos.");
        return;
    }

    qryContext *contexto = (qryContext*)aux_context;
    Node node_alvo = (Node)item_node;
    Info info_alvo = getInfoSmuT(contexto->tree, node_alvo);
    DescritorTipoInfo tipo_alvo = getTypeInfoSmuT(contexto->tree, node_alvo);

    set_corF(info_alvo, tipo_alvo, contexto->cor_borda, contexto->cor_preenchimento);
    set_strkWF(info_alvo, tipo_alvo, contexto->largura_borda);

    fprintf(contexto->arqTxt, "cmflg: Forma ID %d modificada.\n", get_idF(info_alvo, tipo_alvo));
}

void handle_cmflg(CONTEXTO ctxt, int id_ref, char *cb, char* cp, double w){
    if(!ctxt){
        fprintf(stderr, "(handle_cmflg) Erro: parametro invalido.\n");
        return; 
    }

    printf("\nProcessando comando cmflg: id=%d, cb=%s, cp=%s, w=%.2f\n", id_ref, cb, cp, w);

    qryContext* contexto = (qryContext*)ctxt;

    fprintf(contexto->arqTxt, "[*] cmflg %d %s %s %.2f\n", id_ref, cb, cp, w);

    Node node_ref = procuraNoSmuT(contexto->tree, findFormByIdCallback, &id_ref);
    if (!node_ref) {
        fprintf(contexto->arqTxt, "Erro: Forma de referencia com ID %d nao encontrada.\n", id_ref);
        return;
    }

    Info info_ref = getInfoSmuT(contexto->tree, node_ref);
    DescritorTipoInfo tipo_ref = getTypeInfoSmuT(contexto->tree, node_ref);

    Lista alvos = criaLista();

    if(!buscaTipoSpy(info_ref, tipo_ref, alvos, contexto)){
        fprintf(contexto->arqTxt, "(handle_cmflg) Erro: buscaTipoSpy falhou!\n");
        destroiLista(alvos, NULL);
        return;
    }

    if(listaEstaVazia(alvos)){
        fprintf(contexto->arqTxt, "Nenhuma forma encontrada para modificar.\n");
        destroiLista(alvos, NULL);
    } else{
        contexto->cor_borda = cb;
        contexto->cor_preenchimento = cp;
        contexto->largura_borda = w;

        percorreLista(alvos, modificaCorELarguraCallback, contexto);
    }

    destroiLista(alvos, NULL);
}

//---------------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------HANDLE_SPY-----------------------------------------------------------------//

void reportaSpyCallback(Item item_node, void *aux_context) {
    if(!item_node || !aux_context){
        fprintf(stderr, "(reportaSpyCallback) Erro: parametros invalidos.");
        return;
    }
    
    qryContext *contexto = (qryContext*)aux_context;
    
    // Extrai as informações do nó
    Node no_alvo = (Node)item_node;
    Info info_alvo = getInfoSmuT(contexto->tree, no_alvo);
    DescritorTipoInfo tipo_alvo = getTypeInfoSmuT(contexto->tree, no_alvo);

    // Obtém o ID e o nome do tipo da forma para o relatório
    int id_alvo = get_idF(info_alvo, tipo_alvo);
    const char* nome_tipo = get_NameStrF(tipo_alvo);

    // Imprime no arquivo de texto
    fprintf(contexto->arqTxt, "  Forma encontrada -> ID: %d, Tipo: %s\n", id_alvo, nome_tipo);
}

void handle_spy(CONTEXTO ctxt, int id_ref){
    if(!ctxt){
        fprintf(stderr, "(handle_cmflg) Erro: parametro invalido.\n");
        return; 
    }

    printf("Processando comando spy: id=%d", id_ref);

    qryContext* contexto = (qryContext*)ctxt;

    fprintf(contexto->arqTxt, "[*] spy %d\n", id_ref);

    // Encontra o no' por id.
    Node node_ref = procuraNoSmuT(contexto->tree, findFormByIdCallback, &id_ref);
    if (!node_ref) {
        fprintf(contexto->arqTxt, "Erro: Forma de referencia com ID %d nao encontrada.\n", id_ref);
        return;
    }
    Info info_ref = getInfoSmuT(contexto->tree, node_ref);
    DescritorTipoInfo tipo_ref = getTypeInfoSmuT(contexto->tree, node_ref);

    // Busca os alvos
    Lista alvos = criaLista();
    if(!buscaTipoSpy(info_ref, tipo_ref, alvos, contexto)){
        fprintf(contexto->arqTxt, "(handle_spy) Erro: buscaTipoSpy falhou!\n");
        destroiLista(alvos, NULL);
        return;
    }

    // Relata os resultados
    if(listaEstaVazia(alvos)){
        fprintf(contexto->arqTxt, "Nenhuma forma encontrada na area de espionagem.\n");
    } else {
        fprintf(contexto->arqTxt, "spy: Relatorio de formas encontradas:\n");

        // Usa o novo callback para apenas relatar, não modificar
        percorreLista(alvos, reportaSpyCallback, contexto);
    }

    // 4. Libera a memória da lista de alvos
    destroiLista(alvos, NULL);


}

//---------------------------------------------------------------------------------------------------------------------------------------//


//------------------------------------------------------------HANDLE_BLOW----------------------------------------------------------------//

bool pontoInternoAFormaCallback(SmuTreap t, Node n, Info i, double px, double py){
    printf("\nDEBUG entrando em pontoInternoAFormaCallback...\n");
    if(!t || !n || !i || px < 0 || py < 0){
        fprintf(stderr, "(pontoInternoAFormaCallback) Erro: parametros invalidos.\n");
        return false;
    }

    DescritorTipoInfo tipo = getTypeInfoSmuT(t, n);

    double bbx, bby, bbw, bbh;
    fCalcBB_individual(tipo, i, &bbx, &bby, &bbw, &bbh);
    
    // Otimizacao com BB
    if (!ponto_interno_retangulo(px, py, bbx, bby, bbx + bbw, bby + bbh, 0.0)) {
        return false; // Rejeição rápida!
    }

    if(tipo == TIPO_CIRCULO){
        // Verifica se o ponto nao esta em alguma parte "fantasma" do circulo.
        CIRCLE c = (CIRCLE)i;
        double cx = get_XC(c);
        double cy = get_YC(c);
        double r = get_rC(c);
        double dist = sqrt(pow(px - cx, 2) + pow(py - cy, 2));
        return dist <= r;
    }

    // Para retangulos, textos e linhas, se estiver no BB, considera-se interno.
    return true;
}

void handle_blow(CONTEXTO ctxt, int id_ogiva){
    if(!ctxt){
        fprintf(stderr, "(handle_blow) Erro: parametro invalido.\n");
        return; 
    }

    printf("Processando o comando blow: id=%d\n", id_ogiva);

    qryContext* contexto = (qryContext*)ctxt;

    fprintf(contexto->arqTxt, "[*] blow %d\n", id_ogiva);

    // Encontra a ogiva
    Node no_ogiva = procuraNoSmuT(contexto->tree, findFormByIdCallback, &id_ogiva);
    if (!no_ogiva) {
        fprintf(contexto->arqTxt, "Erro: Ogiva com ID %d nao encontrada.\n", id_ogiva);
        return;
    }

    Info info_ogiva = getInfoSmuT(contexto->tree, no_ogiva);
    double x_exp, y_exp;
    
    get_anchorF(info_ogiva, getTypeInfoSmuT(contexto->tree, no_ogiva), &x_exp, &y_exp, NULL, NULL);
    fprintf(contexto->arqTxt, "blow: Ogiva ID %d explodiu em (%.2f, %.2f).\n", id_ogiva, x_exp, y_exp);


    // Adiciona anotação SVG '#' para a explosão
    char* anot_explosao = (char*)malloc(128 * sizeof(char));
    if (anot_explosao) {
        sprintf(anot_explosao, "<text x=\"%.2f\" y=\"%.2f\" fill=\"orange\" text-anchor=\"middle\" dominant-baseline=\"middle\" font-size=\"8\" font-weight=\"bold\">#</text>", x_exp, y_exp);
        insereNaLista(contexto->lista_anotacoes_svg, (Item)anot_explosao);
    }


    // Encontra todos os alvos atingidos pelo ponto de explosão
    Lista alvos_atingidos = criaLista();
    getInfosAtingidoPontoSmuT(contexto->tree, x_exp, y_exp, pontoInternoAFormaCallback, alvos_atingidos);
    printf("DEBUG saindo de getInfosAtingidoPontoSmuT\n");

    // Cria lista final de todos os nos a serem removidos
    Lista nos_a_remover = criaLista();
    insereNaLista(nos_a_remover, (Item)no_ogiva);

    while(!listaEstaVazia(alvos_atingidos)){
        Node alvo_a_remover = (Node)removePrimeiroDaLista(alvos_atingidos);

        if(alvo_a_remover != no_ogiva){
            insereNaLista(nos_a_remover, (Item)alvo_a_remover);
        }
    }
    destroiLista(alvos_atingidos, NULL);

    // Adicionar anotacoes no svg e txt.
    fprintf(contexto->arqTxt, "  Formas destruidas pela explosao:\n");

    while(!listaEstaVazia(nos_a_remover)){
        Node no_para_remocao = (Node)removePrimeiroDaLista(nos_a_remover);
        Info info_removida = getInfoSmuT(contexto->tree, no_para_remocao);
        DescritorTipoInfo tipo_removido = getTypeInfoSmuT(contexto->tree, no_para_remocao);
        
        // Relata no TXT
        int id_removido = get_idF(info_removida, tipo_removido);
        const char* nome_forma_removida = get_NameStrF(tipo_removido);
        fprintf(contexto->arqTxt, "    - ID: %d, Tipo: %s\n", id_removido, nome_forma_removida);

        // Anota o 'x' no svg
        double xAnch_removido, yAnch_removido;
        get_anchorF(info_removida, tipo_removido, &xAnch_removido, &yAnch_removido, NULL, NULL);

        char* anot_x = (char*)malloc(128 * sizeof(char));
        if (anot_x) {
            sprintf(anot_x, "<text x=\"%.2f\" y=\"%.2f\" fill=\"red\" text-anchor=\"middle\" dominant-baseline=\"middle\" font-size=\"8\" font-weight=\"bold\">x</text>", xAnch_removido, yAnch_removido);
            insereNaLista(contexto->lista_anotacoes_svg, (Item)anot_x);
        }

        // Remove o no atingido da arvore
        printf("DEBUG (handle_blow) PAROU AQUI?\n");
        removeNoSmuT(contexto->tree, no_para_remocao);
    }

    destroiLista(nos_a_remover, NULL);
}

//---------------------------------------------------------------------------------------------------------------------------------------//