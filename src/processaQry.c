#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SmuTreap.h"
#include "boundingBox.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"
#include "Lista.h"

#ifndef TIPO_FORMAS_DEFINED
#define TIPO_FORMAS_DEFINED
#define TIPO_CIRCULO 1
#define TIPO_RETANGULO 2
#define TIPO_TEXTO 3
#define TIPO_LINHA 4
#endif


//----------------------------HANDLE_SELR---------------------------------------------------------------------//
// Estrutura auxiliar para passar dados para a função de callback de percorreLista
typedef struct {
    FILE* file_txt_out;
    SmuTreap smutreap_tree;
    Lista lista_anotacoes_svg;
} VisitaInfoSelrData;

// Função de callback para getInfosDentroRegiaoSmuT: verifica se uma forma está totalmente contida na região
bool formaTotalmenteContidaCallback(SmuTreap t, Node n_node, Info forma_info, double reg_x1, double reg_y1, double reg_x2, double reg_y2) {
    DescritorTipoInfo tipo_forma = getTypeInfoSmuT(t, n_node);

    if (tipo_forma == TIPO_CIRCULO) {
        CIRCLE c = (CIRCLE)forma_info;
        double cx = get_XC(c);
        double cy = get_YC(c);
        double cr = get_rC(c);
        // Verifica se o círculo está totalmente dentro da região retangular (reg_x1, reg_y1) a (reg_x2, reg_y2)
        return (cx - cr >= reg_x1 && cx + cr <= reg_x2 &&
                cy - cr >= reg_y1 && cy + cr <= reg_y2);
    } else if (tipo_forma == TIPO_RETANGULO) {
        RECTANGLE r = (RECTANGLE)forma_info;
        double fx = get_XR(r); // Coordenada x da âncora da forma (canto superior esquerdo)
        double fy = get_YR(r); // Coordenada y da âncora da forma
        double fw = get_wR(r);
        double fh = get_hR(r);
        return (fx >= reg_x1 && fx + fw <= reg_x2 &&
                fy >= reg_y1 && fy + fh <= reg_y2);
    } else if (tipo_forma == TIPO_LINHA) {
        LINHA l = (LINHA)forma_info;
        double x1 = get_X1L(l);
        double y1 = get_Y1L(l);
        double x2 = get_X2L(l);
        double y2 = get_Y2L(l);
        // Ambos os pontos da linha devem estar dentro da região
        bool p1_dentro = (x1 >= reg_x1 && x1 <= reg_x2 && y1 >= reg_y1 && y1 <= reg_y2);
        bool p2_dentro = (x2 >= reg_x1 && x2 <= reg_x2 && y2 >= reg_y1 && y2 <= reg_y2);
        return p1_dentro && p2_dentro;
    } else if (tipo_forma == TIPO_TEXTO) {
        // Para o texto, verificamos se seu bounding box está contido na região
        double bb_texto_x, bb_texto_y, bb_texto_w, bb_texto_h;
        // Usamos a função fCalcBB_individual que você implementou em boundingBox.c
        fCalcBB_individual(tipo_forma, forma_info, &bb_texto_x, &bb_texto_y, &bb_texto_w, &bb_texto_h);
        return (bb_texto_x >= reg_x1 && bb_texto_x + bb_texto_w <= reg_x2 &&
                bb_texto_y >= reg_y1 && bb_texto_y + bb_texto_h <= reg_y2);
    }
    return false; // Tipo desconhecido não está contido
}

// Função de callback para percorreLista (para processar nós selecionados)
void processaNoSelecionadoParaTXTESVG(Item item_node, void *aux_data) {
    VisitaInfoSelrData* visita_data = (VisitaInfoSelrData*)aux_data;
    Node no_selecionado = (Node)item_node; // O Item da lista é o Node da SmuTreap
    Info info_forma = getInfoSmuT(visita_data->smutreap_tree, no_selecionado);
    DescritorTipoInfo tipo_forma = getTypeInfoSmuT(visita_data->smutreap_tree, no_selecionado);

    int id_forma = -1;
    char* nome_tipo_forma = "Desconhecido";
    double ancora_svg_x, ancora_svg_y; // Âncora da FORMA para o círculo SVG

    if (tipo_forma == TIPO_CIRCULO) {
        CIRCLE c = (CIRCLE)info_forma;
        id_forma = get_idC(c);
        nome_tipo_forma = "circulo";
        ancora_svg_x = get_XC(c); // Centro do círculo é sua âncora
        ancora_svg_y = get_YC(c);
    } else if (tipo_forma == TIPO_RETANGULO) {
        RECTANGLE r = (RECTANGLE)info_forma;
        id_forma = get_idR(r);
        nome_tipo_forma = "retangulo";
        ancora_svg_x = get_XR(r); // Âncora do retângulo (canto superior esquerdo)
        ancora_svg_y = get_YR(r);
    } else if (tipo_forma == TIPO_LINHA) {
        LINHA l = (LINHA)info_forma;
        id_forma = get_idL(l);
        nome_tipo_forma = "linha";
        // A "âncora da forma" para a linha pode ser o primeiro ponto (x1,y1)
        // ou a âncora do nó na SmuTreap, que é um dos seus pontos finais.
        // Usaremos (x1,y1) para consistência com outras formas.
        ancora_svg_x = get_X1L(l);
        ancora_svg_y = get_Y1L(l);
    } else if (tipo_forma == TIPO_TEXTO) {
        TEXTO t = (TEXTO)info_forma;
        id_forma = get_idT(t);
        nome_tipo_forma = "texto";
        ancora_svg_x = get_XT(t); // Âncora original do texto
        ancora_svg_y = get_YT(t);
    }

    // Escrever no arquivo TXT
    fprintf(visita_data->file_txt_out, "%d: %s\n", id_forma, nome_tipo_forma);

    // Adicionar anotação para SVG: pequena circunferência vermelha na âncora da forma
    char* anotacao_circ_ancora = (char*)malloc(200 * sizeof(char));
    if (anotacao_circ_ancora) {
        sprintf(anotacao_circ_ancora, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />", ancora_svg_x, ancora_svg_y);
        insereNaLista(visita_data->lista_anotacoes_svg, (Item)anotacao_circ_ancora);
    }
}

void handle_selr(SmuTreap tree, FILE* pathTxtOut, int n_id_selecao, double sel_x, double sel_y, double sel_w, double sel_h, Lista* array_selecoes, Lista lista_anotacoes_svg) {
    // Escreve o comando original no arquivo TXT, precedido por "[*]"
    fprintf(pathTxtOut, "[*] selr %d %.2f %.2f %.2f %.2f\n", n_id_selecao, sel_x, sel_y, sel_w, sel_h);
    printf("Processando comando selr: n=%d, x=%.2f, y=%.2f, w=%.2f, h=%.2f\n", n_id_selecao, sel_x, sel_y, sel_w, sel_h);

    // 1. Criar uma lista para armazenar os nós (formas) selecionados nesta operação
    Lista lista_formas_encontradas = criaLista();
    if (!lista_formas_encontradas) {
        fprintf(pathTxtOut, "Erro critico: Falha ao alocar memoria para lista de selecao.\n");
        return;
    }

    // 2. Definir a região de seleção para a SmuTreap
    // A SmuTreap espera (x1,y1) e (x2,y2) como cantos opostos.
    // (sel_x, sel_y) é o canto superior esquerdo.
    double reg_x1 = sel_x;
    double reg_y1 = sel_y;
    double reg_x2 = sel_x + sel_w;
    double reg_y2 = sel_y + sel_h;

    // 3. Chamar a função da SmuTreap para obter as formas dentro da região
    // A função getInfosDentroRegiaoSmuT vai usar formaTotalmenteContidaCallback
    // para verificar cada forma.
    getInfosDentroRegiaoSmuT(tree, reg_x1, reg_y1, reg_x2, reg_y2, formaTotalmenteContidaCallback, lista_formas_encontradas);

    // 4. Armazenar a lista de formas encontradas no array de seleções
    // O índice 'n_id_selecao' deve estar entre 0 e 99 [cite: 165]
    if (n_id_selecao >= 0 && n_id_selecao < 100) { // Assumindo que o array_selecoes tem tamanho 100
        // Se já existe uma seleção com este 'n_id_selecao', ela deve ser substituída.
        // Primeiro, liberamos a lista antiga, se houver.
        if (array_selecoes[n_id_selecao] != NULL) {
            // Os itens da lista são ponteiros para Node da SmuTreap,
            // não devemos liberar os Nodes em si, apenas a estrutura da lista.
            destroiLista(array_selecoes[n_id_selecao], NULL);
        }
        array_selecoes[n_id_selecao] = lista_formas_encontradas;
    } else {
        fprintf(pathTxtOut, "Erro: Numero de identificacao de selecao 'n' (%d) invalido. Deve ser entre 0 e 99.\n", n_id_selecao);
        // Como a seleção não será armazenada, destruímos a lista que criamos.
        destroiLista(lista_formas_encontradas, NULL);
        // Adicionar anotação SVG para o retângulo de seleção mesmo se 'n' for inválido, pois o comando foi dado
        char* anot_rect_sel_svg = (char*)malloc(256 * sizeof(char));
        if (anot_rect_sel_svg) {
            sprintf(anot_rect_sel_svg, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" stroke=\"red\" fill=\"none\" stroke-dasharray=\"5,5\" />",
                    sel_x, sel_y, sel_w, sel_h);
            insereNaLista(lista_anotacoes_svg, (Item)anot_rect_sel_svg);
        }
        return; // Interrompe o processamento deste comando selr
    }

    // 5. Gerar a saída TXT e preparar anotações SVG para as formas selecionadas
    if (listaEstaVazia(lista_formas_encontradas)) {
        fprintf(pathTxtOut, "Nenhuma forma inteiramente contida na regiao especificada.\n");
    } else {
        VisitaInfoSelrData data_visita;
        data_visita.file_txt_out = pathTxtOut;
        data_visita.smutreap_tree = tree;
        data_visita.lista_anotacoes_svg = lista_anotacoes_svg;

        // Percorre a lista de nós selecionados para gerar saída TXT e anotações SVG
        percorreLista(lista_formas_encontradas, processaNoSelecionadoParaTXTESVG, &data_visita);
    }

    // 6. Adicionar anotação SVG para o retângulo de seleção
    // Esta string será adicionada à lista_anotacoes_svg e depois escrita no arquivo SVG principal.
    char* anot_rect_sel_svg = (char*)malloc(256 * sizeof(char));
    if (anot_rect_sel_svg) {
        // Desenha o retângulo de seleção (x,y,w,h) em vermelho tracejado
        sprintf(anot_rect_sel_svg, "<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" stroke=\"red\" fill=\"none\" stroke-dasharray=\"5,5\" />",
                sel_x, sel_y, sel_w, sel_h);
        insereNaLista(lista_anotacoes_svg, (Item)anot_rect_sel_svg);
    }
    // Nota: A lista 'lista_formas_encontradas' agora é de responsabilidade do 'array_selecoes[n_id_selecao]'.
    // Não a destrua aqui se 'n_id_selecao' for válido.

    //COMO INTEGRAR:
    //1. Colocar array de selecoes na MAIN
    //2. Inicializar uma lista de anotacoes para svg na MAIN
    //3. Funcao para cuidar das anotacoes para svg, que sao alocadas dinamicamente.
//------------------------------------------------------------------------------------------------------------//


//--------------------------------------HANDLE_SELI-----------------------------------------------------------//
// Definições dos tipos de formas (assumindo que já existem em algum lugar acessível)
// #define TIPO_CIRCULO 1
// #define TIPO_RETANGULO 2
// #define TIPO_TEXTO 3
// #define TIPO_LINHA 4

void handle_seli(SmuTreap tree, FILE* pathTxtOut, int n_id_selecao, double sel_x, double sel_y, Lista* array_selecoes, Lista lista_anotacoes_svg) {
    // Escreve o comando original no arquivo TXT
    fprintf(pathTxtOut, "[*] seli %d %.2f %.2f\n", n_id_selecao, sel_x, sel_y);
    printf("Processando comando seli: n=%d, x=%.2f, y=%.2f\n", n_id_selecao, sel_x, sel_y);

    // 1. Buscar o nó na SmuTreap usando as coordenadas (sel_x, sel_y)
    // A função getNodeSmuT já considera a tolerância epsilon.
    // Ela também pode promover o nó se o hitCount for atingido.
    Node no_encontrado = getNodeSmuT(tree, sel_x, sel_y);

    // 2. Validar o número de seleção 'n_id_selecao' e preparar a lista de seleção
    Lista lista_para_esta_selecao = NULL;
    if (n_id_selecao >= 0 && n_id_selecao < 100) { // Assumindo que o array_selecoes tem tamanho 100
        // Se já existe uma seleção com este 'n_id_selecao', ela deve ser substituída.
        if (array_selecoes[n_id_selecao] != NULL) {
            destroiLista(array_selecoes[n_id_selecao], NULL); // Libera a estrutura da lista anterior
        }
        lista_para_esta_selecao = criaLista();
        if (!lista_para_esta_selecao) {
            fprintf(pathTxtOut, "Erro critico: Falha ao alocar memoria para lista da selecao %d.\n", n_id_selecao);
            // Mesmo com falha de lista, se o nó foi encontrado, o efeito de getNodeSmuT (promoção) já ocorreu.
            // Podemos tentar reportar a forma encontrada se n_id_selecao fosse válido, mas sem armazenar.
            // Por simplicidade aqui, se a lista não pode ser criada, não prosseguimos com o armazenamento.
            return;
        }
        array_selecoes[n_id_selecao] = lista_para_esta_selecao;
    } else {
        fprintf(pathTxtOut, "Erro: Numero de identificacao de selecao 'n' (%d) invalido. Deve ser entre 0 e 99.\n", n_id_selecao);
        // Se 'n' é inválido, não armazenaremos a seleção.
        // No entanto, a busca com getNodeSmuT já foi feita e pode ter tido efeitos (hit count).
        // A especificação do projeto não detalha o comportamento do TXT/SVG se 'n' é inválido mas uma forma é achada.
        // Vamos reportar a forma se encontrada, mas sem associá-la à seleção 'n'.
        if (no_encontrado == NULL) {
            fprintf(pathTxtOut, "Nenhuma forma encontrada na coordenada (%.2f, %.2f) com a tolerancia epsilon.\n", sel_x, sel_y);
        } else {
            // Lógica para reportar a forma encontrada (sem armazenar na seleção inválida 'n')
            Info info_forma = getInfoSmuT(tree, no_encontrado);
            DescritorTipoInfo tipo_forma = getTypeInfoSmuT(tree, no_encontrado);
            int id_forma_report = -1;
            char* nome_tipo_report = "Desconhecido";

            if (tipo_forma == TIPO_CIRCULO) { id_forma_report = get_idC((CIRCLE)info_forma); nome_tipo_report = "circulo"; }
            else if (tipo_forma == TIPO_RETANGULO) { id_forma_report = get_idR((RECTANGLE)info_forma); nome_tipo_report = "retangulo"; }
            else if (tipo_forma == TIPO_LINHA) { id_forma_report = get_idL((LINHA)info_forma); nome_tipo_report = "linha"; }
            else if (tipo_forma == TIPO_TEXTO) { id_forma_report = get_idT((TEXTO)info_forma); nome_tipo_report = "texto"; }

            fprintf(pathTxtOut, "%d: %s (aviso: 'n' de selecao invalido, forma nao armazenada na selecao)\n", id_forma_report, nome_tipo_report);

            // Adicionar anotação SVG mesmo com 'n' inválido, pois a forma foi "vista"
            char* anot_svg_circ_ancora_report = (char*)malloc(200 * sizeof(char));
            if (anot_svg_circ_ancora_report) {
                sprintf(anot_svg_circ_ancora_report, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />", sel_x, sel_y);
                insereNaLista(lista_anotacoes_svg, (Item)anot_svg_circ_ancora_report);
            }
        }
        return; // Interrompe o processamento normal para este comando seli
    }

    // 3. Processar o resultado da busca
    if (no_encontrado == NULL) {
        fprintf(pathTxtOut, "Nenhuma forma encontrada na coordenada (%.2f, %.2f) com a tolerancia epsilon.\n", sel_x, sel_y);
        // A lista em array_selecoes[n_id_selecao] permanecerá vazia.
    } else {
        // Adicionar o nó encontrado à lista de seleção (que já está em array_selecoes[n_id_selecao])
        insereNaLista(lista_para_esta_selecao, (Item)no_encontrado);

        // Obter informações da forma para saída TXT e anotação SVG
        Info info_forma = getInfoSmuT(tree, no_encontrado);
        DescritorTipoInfo tipo_forma = getTypeInfoSmuT(tree, no_encontrado);
        int id_forma = -1;
        char* nome_tipo_forma = "Desconhecido";

        // Determinar ID e tipo para o relatório TXT
        if (tipo_forma == TIPO_CIRCULO) {
            id_forma = get_idC((CIRCLE)info_forma);
            nome_tipo_forma = "circulo";
        } else if (tipo_forma == TIPO_RETANGULO) {
            RECTANGLE r_info = (RECTANGLE)info_forma;
            id_forma = get_idR(r_info);
            nome_tipo_forma = "retangulo";
        } else if (tipo_forma == TIPO_LINHA) {
            LINHA l_info = (LINHA)info_forma;
            id_forma = get_idL(l_info);
            nome_tipo_forma = "linha";
        } else if (tipo_forma == TIPO_TEXTO) {
            TEXTO t_info = (TEXTO)info_forma;
            id_forma = get_idT(t_info);
            nome_tipo_forma = "texto";
        }

        // Escrever no arquivo TXT
        fprintf(pathTxtOut, "%d: %s\n", id_forma, nome_tipo_forma);

        // Adicionar anotação SVG: pequena circunferência vermelha na âncora da forma (sel_x, sel_y)
        char* anot_svg_circ_ancora = (char*)malloc(200 * sizeof(char));
        if (anot_svg_circ_ancora) {
            sprintf(anot_svg_circ_ancora, "<circle cx=\"%.2f\" cy=\"%.2f\" r=\"2\" fill=\"red\" />", sel_x, sel_y);
            insereNaLista(lista_anotacoes_svg, (Item)anot_svg_circ_ancora);
        }
    }
}
}