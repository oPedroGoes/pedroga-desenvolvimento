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
#include "formas.h"
#include "comandosQry.h"

//----------------------------HANDLE_SELR---------------------------------------------------------------------//

void escreverFormaSvg1(SmuTreap t, Node n, Info i, double x_ancora_no, double y_ancora_no, void *aux_file_ptr) {
    if (!i || !aux_file_ptr) {
        return;
    }

    FILE *arq_svg = (FILE *)aux_file_ptr;
    DescritorTipoInfo tipo_forma = getTypeInfoSmuT(t, n);

    switch (tipo_forma) {
        case TIPO_CIRCULO: {
            CIRCLE c = (CIRCLE)i;
            int id = get_idC(c);
            double xc = get_XC(c);
            double yc = get_YC(c);
            double r_circ = get_rC(c);
            char *corb = get_cbC(c);
            char *corp = get_cpC(c);
            double strokeWidth = get_strkWC(c);

            fprintf(arq_svg, "\t<circle id=\"%d\" cx=\"%lf\" cy=\"%lf\" r=\"%lf\" stroke=\"%s\" fill=\"%s\" stroke-width=\"%.2f\" fill-opacity=\"0.5\" />\n", id, xc, yc, r_circ, corb, corp, strokeWidth);
            break;
        }
        case TIPO_RETANGULO: {
            RECTANGLE r_fig = (RECTANGLE)i;
            int id = get_idR(r_fig); 
            double xr = get_XR(r_fig); 
            double yr = get_YR(r_fig); 
            double w = get_wR(r_fig); 
            double h = get_hR(r_fig); 
            char *corb = get_cbR(r_fig); 
            char *corp = get_cpR(r_fig); 
            double strokeWidth = get_strkWR(r_fig);

            fprintf(arq_svg, "\t<rect id=\"%d\" x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" stroke=\"%s\" fill=\"%s\" stroke-width=\"%.2f\" fill-opacity=\"0.5\" />\n", id, xr, yr, w, h, corb, corp, strokeWidth);
            // Posicionar ID no centro da âncora original do projeto (canto inferior esquerdo) pode ser uma opção, ou no centro visual.
            // Para centro visual:
            break;
        }
        case TIPO_LINHA: {
            LINHA l = (LINHA)i;
            int id = get_idL(l);
            double x1 = get_X1L(l);
            double y1 = get_Y1L(l);
            double x2 = get_X2L(l);
            double y2 = get_Y2L(l);
            char *cor = get_cL(l);
            double strokeWidth = get_strkWL(l);

            fprintf(arq_svg, "\t<line id=\"%d\" x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\" stroke-width=\"%.2f\" />\n",id, x1, y1, x2, y2, cor, strokeWidth);
            // O ID da linha será desenhado na âncora da árvore para esta linha (x_ancora_no, y_ancora_no)
            break;
        }
        case TIPO_TEXTO: {
            TEXTO txt_fig = (TEXTO)i;
            int id = get_idT(txt_fig);
            double xt = get_XT(txt_fig);
            double yt = get_YT(txt_fig);
            char *corb = get_cbT(txt_fig);
            char *corp = get_cpT(txt_fig);
            double strokeWidth = get_strkWT(txt_fig);
            char *ancora_svg = get_ancoraT(txt_fig); // Retorna "start", "middle", ou "end"
            char *conteudo_txt = get_txtT(txt_fig);
            char *ff = get_ffT(txt_fig);
            char *fw = get_fwT(txt_fig);
            char *fs = get_fsT(txt_fig);

            // A cor da borda do texto em SVG é controlada por 'stroke', não 'fill' da borda.
            // O preenchimento do texto é 'fill'.
            fprintf(arq_svg, "\t<text id=\"%d\" x=\"%.2f\" y=\"%.2f\" stroke=\"%s\" fill=\"%s\" stroke-width=\"%.2f\" font-family=\"%s\" font-weight=\"%s\" font-size=\"%s\" text-anchor=\"%s\" dominant-baseline=\"middle\">",
                    id, xt, yt, corb, corp, strokeWidth, ff, fw, fs, ancora_svg);

            // Itera pelo conteudo_txt e escapa os caracteres especiais
            if (conteudo_txt) {
                char *p = conteudo_txt;
                while (*p) {
                    switch (*p) {
                        case '<':
                            fprintf(arq_svg, "&lt;");
                            break;
                        case '>':
                            fprintf(arq_svg, "&gt;");
                            break;
                        case '&':
                            fprintf(arq_svg, "&amp;");
                            break;
                        case '"': // Embora não estritamente necessário para o conteúdo da tag, é seguro escapar.
                            fprintf(arq_svg, "&quot;");
                            break;
                        case '\'': // Similarmente, seguro escapar.
                            fprintf(arq_svg, "&apos;");
                            break;
                        default:
                            fputc(*p, arq_svg);
                            break;
                    }
                    p++;
                }
            }
            // Imprime a tag de fechamento do texto
            fprintf(arq_svg, "</text>\n");
            break;
        }
        default:
            fprintf(stderr, "Aviso (escreverFormaSvg1): Tipo de forma desconhecido (%d) encontrado no no com ancora (%.2f, %.2f).\n", tipo_forma, x_ancora_no, y_ancora_no);
            break;
    }
}

void visitaListaSvg(Item anotacao, void *aux){
    if(!anotacao || !aux){
        fprintf(stderr, "(visitaListaSvg) Erro: parametros invalidos");
        return;
    }

    FILE *arqSvg2 = (FILE*)aux;

    const char *buff = (const char*)anotacao;
    fprintf(arqSvg2, "%s\n", buff);
    return;
}


//------------------------------------------------------------------------------------------------------------//

/*



//-----------------------------------------------HANDLE_DISP-----------------------------------------------------------//


//---------------------------------------------------------------------------------------------------------------------//



//-----------------------------------------------HANDLE_CLN-----------------------------------------------------------//

// Definições dos tipos de formas (já devem existir)
// #define TIPO_CIRCULO 1
// #define TIPO_RETANGULO 2
// #define TIPO_TEXTO 3
// #define TIPO_LINHA 4

// Função auxiliar para criar um clone de uma Info de forma, transladá-la e trocar cores.
// Retorna a nova Info clonada, ou NULL em caso de falha.
// O ID do clone é atribuído usando p_proximo_id_unico.

Info criarInfoClonada(SmuTreap tree, Node no_original, double dx, double dy, int* p_proximo_id_unico) {
    if (!no_original || !p_proximo_id_unico) return NULL;

    Info info_original = getInfoSmuT(tree, no_original);
    DescritorTipoInfo tipo_original = getTypeInfoSmuT(tree, no_original);
    Info clone_info = NULL;
    int novo_id_clone = (*p_proximo_id_unico)++; // Pega o próximo ID único e incrementa o contador

    switch (tipo_original) {
        case TIPO_CIRCULO: {
            CIRCLE original_c = (CIRCLE)info_original;
            double novo_x = get_XC(original_c) + dx;
            double novo_y = get_YC(original_c) + dy;
            // Troca de cores para o clone
            char* nova_cor_borda = get_cpC(original_c); // Borda do clone é preenchimento do original
            char* nova_cor_preenchimento = get_cbC(original_c); // Preenchimento do clone é borda do original
            clone_info = (Info)create_circle(novo_id_clone, novo_x, novo_y, get_rC(original_c), nova_cor_borda, nova_cor_preenchimento);
            break;
        }
        case TIPO_RETANGULO: {
            RECTANGLE original_r = (RECTANGLE)info_original;
            double novo_x = get_XR(original_r) + dx;
            double novo_y = get_YR(original_r) + dy;
            char* nova_cor_borda = get_cpR(original_r);
            char* nova_cor_preenchimento = get_cbR(original_r);
            clone_info = (Info)create_rectangle(novo_id_clone, novo_x, novo_y, get_wR(original_r), get_hR(original_r), nova_cor_borda, nova_cor_preenchimento);
            break;
        }
        case TIPO_LINHA: {
            LINHA original_l = (LINHA)info_original;
            // Para a linha, ambos os pontos são transladados
            double novo_x1 = get_X1L(original_l) + dx;
            double novo_y1 = get_Y1L(original_l) + dy;
            double novo_x2 = get_X2L(original_l) + dx;
            double novo_y2 = get_Y2L(original_l) + dy;
            // Linhas têm apenas uma cor, então a cor do clone é a mesma da original.
            char* cor_original_linha = get_cL(original_l);
            clone_info = (Info)cria_linha(novo_id_clone, novo_x1, novo_y1, novo_x2, novo_y2, cor_original_linha);
            break;
        }
        case TIPO_TEXTO: {
            TEXTO original_t = (TEXTO)info_original;
            double novo_x = get_XT(original_t) + dx;
            double novo_y = get_YT(original_t) + dy;
            char* nova_cor_borda = get_cpT(original_t);
            char* nova_cor_preenchimento = get_cbT(original_t);
            // O clone herda as propriedades de fonte (família, peso, tamanho) e o conteúdo do texto original.
            clone_info = (Info)cria_texto(novo_id_clone, novo_x, novo_y,
                                          nova_cor_borda, nova_cor_preenchimento,
                                          get_charancoraT(original_t), // Mesmo tipo de âncora
                                          get_txtT(original_t),        // Mesmo conteúdo de texto
                                          get_ffT(original_t),         // Mesma família de fonte
                                          get_fwT(original_t),         // Mesmo peso de fonte
                                          get_fsT(original_t));        // Mesmo tamanho de fonte
            break;
        }
        default:
            // Tipo de forma desconhecido, não pode clonar.
            fprintf(stderr, "Erro (criarInfoClonada): Tipo de forma desconhecido %d.\n", tipo_original);
            (*p_proximo_id_unico)--; // Reverte o incremento do ID, pois não foi usado.
            return NULL;
    }
    return clone_info;
}

// Estrutura de dados para passar informações para a função de callback de percorreLista
typedef struct {
    SmuTreap tree_param;
    FILE* pathTxt_param;
    double dx_param, dy_param;
    int* p_id_unico_param;             // Ponteiro para o contador global de IDs
    FCalculaBoundingBox func_calc_bb_param; // Função para calcular BB ao inserir na SmuTreap
} ProcessaCloneCallbackData;

// Função de callback para ser usada com percorreLista, processa cada forma da seleção
void clonarEInserirFormaCallback(Item item_no_original, void* aux_data) {
    ProcessaCloneCallbackData* data = (ProcessaCloneCallbackData*)aux_data;
    Node no_original = (Node)item_no_original;

    DescritorTipoInfo tipo_original = getTypeInfoSmuT(data->tree_param, no_original);
    Info info_original_para_id = getInfoSmuT(data->tree_param, no_original);
    int id_original = -1;

    // Obter ID da forma original para o relatório TXT
    if (tipo_original == TIPO_CIRCULO) id_original = get_idC((CIRCLE)info_original_para_id);
    else if (tipo_original == TIPO_RETANGULO) id_original = get_idR((RECTANGLE)info_original_para_id);
    else if (tipo_original == TIPO_LINHA) id_original = get_idL((LINHA)info_original_para_id);
    else if (tipo_original == TIPO_TEXTO) id_original = get_idT((TEXTO)info_original_para_id);

    // 1. Criar a Info clonada (já transladada, com cores trocadas e novo ID)
    Info info_clonada = criarInfoClonada(data->tree_param, no_original, data->dx_param, data->dy_param, data->p_id_unico_param);

    if (info_clonada) {
        double ancora_clone_x, ancora_clone_y;
        int id_clone = -1; // O ID já foi atribuído dentro de criarInfoClonada

        // 2. Determinar as coordenadas da âncora do clone para inserção na SmuTreap
        //    e obter o ID do clone para o relatório.
        if (tipo_original == TIPO_CIRCULO) {
            CIRCLE c_clone = (CIRCLE)info_clonada;
            ancora_clone_x = get_XC(c_clone);
            ancora_clone_y = get_YC(c_clone);
            id_clone = get_idC(c_clone);
        } else if (tipo_original == TIPO_RETANGULO) {
            RECTANGLE r_clone = (RECTANGLE)info_clonada;
            ancora_clone_x = get_XR(r_clone);
            ancora_clone_y = get_YR(r_clone);
            id_clone = get_idR(r_clone);
        } else if (tipo_original == TIPO_LINHA) {
            LINHA l_clone = (LINHA)info_clonada;
            id_clone = get_idL(l_clone);
            // A âncora da linha na SmuTreap é um de seus pontos finais.
            // A escolha é baseada em qual ponto vem "primeiro" (menor x, ou menor y se x for igual).
            // Esta lógica está em processaGeo.c e deve ser replicada aqui para consistência.
            double x1_c = get_X1L(l_clone); double y1_c = get_Y1L(l_clone);
            double x2_c = get_X2L(l_clone); double y2_c = get_Y2L(l_clone);
            if (x1_c < x2_c - 1e-9 || (fabs(x1_c - x2_c) < 1e-9 && y1_c < y2_c - 1e-9)) { // Tolerância para comparação de float
                 ancora_clone_x = x1_c; ancora_clone_y = y1_c;
            } else if (fabs(x1_c - x2_c) < 1e-9 && fabs(y1_c - y2_c) < 1e-9) { // Pontos são iguais
                 ancora_clone_x = x1_c; ancora_clone_y = y1_c;
            }
             else {
                 ancora_clone_x = x2_c; ancora_clone_y = y2_c;
            }
        } else if (tipo_original == TIPO_TEXTO) {
            TEXTO t_clone = (TEXTO)info_clonada;
            ancora_clone_x = get_XT(t_clone);
            ancora_clone_y = get_YT(t_clone);
            id_clone = get_idT(t_clone);
        } else {
             fprintf(data->pathTxt_param, "  Alerta: Tipo de forma original %d desconhecido ao determinar âncora para clone.\n", tipo_original);
             // Libera a info_clonada, pois não pode ser inserida sem âncora correta.
             // killInfo(info_clonada, tipo_original); // Supondo que uma função killInfo(Info, Descritor) exista
             return;
        }

        // 3. Inserir o clone na SmuTreap
        Node no_inserido_clonado = insertSmuT(data->tree_param, ancora_clone_x, ancora_clone_y, info_clonada, tipo_original, data->func_calc_bb_param);

        if (no_inserido_clonado) {
            fprintf(data->pathTxt_param, "  Forma original ID %d clonada. Novo clone ID %d inserido em (%.2f, %.2f).\n",
                    id_original, id_clone, ancora_clone_x, ancora_clone_y);
        } else {
            fprintf(data->pathTxt_param, "  Falha ao inserir clone da forma original ID %d (novo ID seria %d) na SmuTreap.\n", id_original, id_clone);
            // Se a inserção falhar, a memória alocada para 'info_clonada' precisa ser liberada.
            // Esta é uma responsabilidade importante. Se SmuTreap.c->killNode existe e é acessível:
            // killInfo(info_clonada, tipo_original); // Você precisaria de uma função killInfo que só libere a Info.
            // Por exemplo:
            // if (tipo_original == TIPO_CIRCULO) kill_circ(info_clonada); else if ...
            // É mais seguro que as funções `create_` retornem NULL em caso de falha de malloc para as strings de cor.
            // Se `info_clonada` foi criada, mas a inserção falhou, ela vaza memória se não for liberada.
            // A função killNode em SmuTreap.c libera info + node. Precisamos apenas liberar a Info.
        }
    } else {
        fprintf(data->pathTxt_param, "  Falha ao criar estrutura de clone para forma original ID %d.\n", id_original);
    }
}

// Função principal para o comando cln
void handle_cln(SmuTreap tree, FILE* pathTxtOut, int n_id_selecao, double dx, double dy, Lista* array_selecoes, int* p_proximo_id_unico, FCalculaBoundingBox func_calc_bb_global) {
    fprintf(pathTxtOut, "[*] cln %d %.2f %.2f\n", n_id_selecao, dx, dy);
    printf("Processando comando cln: n_selecao=%d, dx=%.2f, dy=%.2f\n", n_id_selecao, dx, dy);

    // 1. Validar e obter a lista de formas da seleção 'n'
    if (n_id_selecao < 0 || n_id_selecao >= 100 || array_selecoes[n_id_selecao] == NULL) {
        fprintf(pathTxtOut, "Erro: Selecao 'n' (%d) invalida ou nao existe.\n", n_id_selecao);
        return;
    }
    Lista lista_formas_a_clonar = array_selecoes[n_id_selecao];
    if (listaEstaVazia(lista_formas_a_clonar)) {
        fprintf(pathTxtOut, "Selecao 'n' (%d) esta vazia. Nada a clonar.\n", n_id_selecao);
        return;
    }

    // 2. Preparar a estrutura de dados para o callback
    ProcessaCloneCallbackData data_callback;
    data_callback.tree_param = tree;
    data_callback.pathTxt_param = pathTxtOut;
    data_callback.dx_param = dx;
    data_callback.dy_param = dy;
    data_callback.p_id_unico_param = p_proximo_id_unico; // Passa o ponteiro para o contador de ID
    data_callback.func_calc_bb_param = func_calc_bb_global; // Passa a função de cálculo de BB

    // 3. Iterar sobre a lista de formas a serem clonadas
    fprintf(pathTxtOut, "Clonando formas da selecao %d:\n", n_id_selecao);
    percorreLista(lista_formas_a_clonar, clonarEInserirFormaCallback, &data_callback);

    fprintf(pathTxtOut, "Clonagem da selecao %d concluida.\n", n_id_selecao);
}
//---------------------------------------------------------------------------------------------------------------------//



//-----------------------------------------------HANDLE_SPY-----------------------------------------------------------//
// Estrutura para auxiliar na busca de qualquer forma por ID
typedef struct {
    int id_procurado;
    Node no_encontrado;         // Ponteiro para o nó encontrado
    DescritorTipoInfo tipo_encontrado; // Tipo da forma encontrada
    Info info_encontrada;       // Ponteiro para a Info da forma encontrada
} SearchDataById;

// Callback para SmuTreap->procuraNoSmuT: encontrar qualquer forma pelo seu ID
bool findFormByIdCallback(SmuTreap t, Node n_atual, Info info_atual, double x_ancora_no, double y_ancora_no, void *aux_data) {
    SearchDataById *data_busca = (SearchDataById *)aux_data;
    DescritorTipoInfo tipo_forma_atual = getTypeInfoSmuT(t, n_atual);
    int id_forma_atual = -1;

    // Obtém o ID da forma atual, dependendo do seu tipo
    if (tipo_forma_atual == TIPO_CIRCULO) id_forma_atual = get_idC((CIRCLE)info_atual);
    else if (tipo_forma_atual == TIPO_RETANGULO) id_forma_atual = get_idR((RECTANGLE)info_atual);
    else if (tipo_forma_atual == TIPO_LINHA) id_forma_atual = get_idL((LINHA)info_atual);
    else if (tipo_forma_atual == TIPO_TEXTO) id_forma_atual = get_idT((TEXTO)info_atual);

    if (id_forma_atual == data_busca->id_procurado) {
        data_busca->no_encontrado = n_atual;
        data_busca->tipo_encontrado = tipo_forma_atual;
        data_busca->info_encontrada = info_atual;
        return true; // Encontrou, para a busca
    }
    return false; // Não é esta forma, continua a busca
}

// Reutilização da função de callback de handle_selr
// Esta função verifica se uma forma está totalmente contida em uma região retangular.
// bool formaTotalmenteContidaCallback(SmuTreap t, Node n_node, Info forma_info, double reg_x1, double reg_y1, double reg_x2, double reg_y2)
// { ... implementação como definida anteriormente em handle_selr ... }
// Certifique-se de que esta função esteja definida e acessível.
// Você precisará de:
// #include "boundingBox.h" // para fCalcBB_individual
// void fCalcBB_individual(DescritorTipoInfo tp, Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h); // protótipo

// Estrutura para passar dados para a função de callback de percorreLista (para reportar formas)
typedef struct {
    FILE* pathTxt;
    SmuTreap smutreap_tree;
} ReportSpyFormData;

// Função de callback para percorrer a lista de formas encontradas e reportá-las
void reportarFormaSpyCallback(Item item_no_forma, void *aux_data) {
    ReportSpyFormData *data_reporte = (ReportSpyFormData *)aux_data;
    Node no_forma_reportar = (Node)item_no_forma;
    Info info_forma = getInfoSmuT(data_reporte->smutreap_tree, no_forma_reportar);
    DescritorTipoInfo tipo_forma = getTypeInfoSmuT(data_reporte->smutreap_tree, no_forma_reportar);

    int id_forma_reportar = -1;
    char* nome_tipo_reportar = "Desconhecido";

    if (tipo_forma == TIPO_CIRCULO) { id_forma_reportar = get_idC((CIRCLE)info_forma); nome_tipo_reportar = "circulo"; }
    else if (tipo_forma == TIPO_RETANGULO) { id_forma_reportar = get_idR((RECTANGLE)info_forma); nome_tipo_reportar = "retangulo"; }
    else if (tipo_forma == TIPO_LINHA) { id_forma_reportar = get_idL((LINHA)info_forma); nome_tipo_reportar = "linha"; }
    else if (tipo_forma == TIPO_TEXTO) { id_forma_reportar = get_idT((TEXTO)info_forma); nome_tipo_reportar = "texto"; }

    fprintf(data_reporte->pathTxt, "  ID %d: %s\n", id_forma_reportar, nome_tipo_reportar);
}

// Função principal para o comando spy
void handle_spy(SmuTreap tree, FILE* pathTxtOut, int id_referencia) {
    fprintf(pathTxtOut, "[*] spy %d\n", id_referencia);
    printf("Processando comando spy: id_referencia=%d\n", id_referencia);

    // 1. Encontrar a forma de referência (retângulo ou texto) com o ID fornecido
    SearchDataById dados_forma_ref;
    dados_forma_ref.id_procurado = id_referencia;
    dados_forma_ref.no_encontrado = NULL; // Inicializa como não encontrado

    procuraNoSmuT(tree, findFormByIdCallback, &dados_forma_ref);

    if (!dados_forma_ref.no_encontrado) {
        fprintf(pathTxtOut, "Erro: Forma de referencia com ID %d nao encontrada na SmuTreap.\n", id_referencia);
        return;
    }

    // Lista para armazenar as formas que serão reportadas
    Lista lista_formas_a_reportar = criaLista();
    if (!lista_formas_a_reportar) {
        fprintf(pathTxtOut, "Erro critico: Falha ao criar lista para reportar formas do comando spy.\n");
        return;
    }

    // 2. Verificar o tipo da forma de referência e agir de acordo
    if (dados_forma_ref.tipo_encontrado == TIPO_RETANGULO) {
        RECTANGLE retangulo_ref = (RECTANGLE)dados_forma_ref.info_encontrada;
        double r_ref_x = get_XR(retangulo_ref);
        double r_ref_y = get_YR(retangulo_ref);
        double r_ref_w = get_wR(retangulo_ref);
        double r_ref_h = get_hR(retangulo_ref);

        fprintf(pathTxtOut, "Formas inteiramente contidas dentro do retangulo de referencia ID %d (x:%.2f, y:%.2f, w:%.2f, h:%.2f):\n",
                id_referencia, r_ref_x, r_ref_y, r_ref_w, r_ref_h);

        // Usa getInfosDentroRegiaoSmuT com a callback formaTotalmenteContidaCallback (de handle_selr)
        // A região é definida pelo retângulo de referência.
        getInfosDentroRegiaoSmuT(tree, r_ref_x, r_ref_y, r_ref_x + r_ref_w, r_ref_y + r_ref_h,
                                 formaTotalmenteContidaCallback, lista_formas_a_reportar);

    } else if (dados_forma_ref.tipo_encontrado == TIPO_TEXTO) {
        TEXTO texto_ref = (TEXTO)dados_forma_ref.info_encontrada;
        double txt_ref_x = get_XT(texto_ref); // Coordenada da âncora do texto de referência
        double txt_ref_y = get_YT(texto_ref);

        fprintf(pathTxtOut, "Forma(s) cuja ancora esta em (%.2f, %.2f) (coordenada do texto de referencia ID %d):\n",
                txt_ref_x, txt_ref_y, id_referencia);

        // Encontra o nó (ou nós, se SmuTreap permitir múltiplas formas na mesma âncora exata)
        // usando getNodeSmuT, que considera epsilon.
        Node no_na_coordenada = getNodeSmuT(tree, txt_ref_x, txt_ref_y);
        if (no_na_coordenada) {
            // getNodeSmuT retorna um único nó. Se múltiplas formas pudessem ter a mesma âncora
            // (o que não deve acontecer com a implementação atual de insertSmuT se x e y forem idênticos
            // dentro de epsilon), seria necessário usar getNodesDentroRegiaoSmuT com uma pequena caixa.
            // Como getNodeSmuT é mais direto para "exatamente na coordenada", vamos usá-lo.
            insereNaLista(lista_formas_a_reportar, (Item)no_na_coordenada);
        }

    } else {
        fprintf(pathTxtOut, "Erro: Forma de referencia ID %d (tipo %d) nao e um retangulo nem um texto. Comando spy nao aplicavel para este tipo.\n",
                id_referencia, dados_forma_ref.tipo_encontrado);
        destroiLista(lista_formas_a_reportar, NULL);
        return;
    }

    // 3. Reportar as formas encontradas (se houver)
    if (listaEstaVazia(lista_formas_a_reportar)) {
        if (dados_forma_ref.tipo_encontrado == TIPO_RETANGULO) {
            fprintf(pathTxtOut, "  Nenhuma forma encontrada dentro do retangulo de referencia.\n");
        } else if (dados_forma_ref.tipo_encontrado == TIPO_TEXTO) {
            fprintf(pathTxtOut, "  Nenhuma forma encontrada exatamente na coordenada do texto de referencia (considerando epsilon).\n");
        }
    } else {
        ReportSpyFormData data_para_reporte;
        data_para_reporte.pathTxt = pathTxtOut;
        data_para_reporte.smutreap_tree = tree;
        percorreLista(lista_formas_a_reportar, reportarFormaSpyCallback, &data_para_reporte);
    }

    // 4. Liberar a lista usada para coletar as formas
    destroiLista(lista_formas_a_reportar, NULL); // Os itens (Node*) não são liberados aqui, pois pertencem à SmuTreap
}
//---------------------------------------------------------------------------------------------------------------------//


//-----------------------------------------------HANDLE_BLOW-----------------------------------------------------------//

// Protótipos de funções auxiliares (definidas em respostas anteriores ou que você precisa ter)
// bool findFormByIdCallback(SmuTreap t, Node n, Info i, double x_anc, double y_anc, void *aux_data);
// bool pontoFinalInternoAFormaAlvoCallback(SmuTreap t, Node n_forma_alvo, Info info_forma_alvo, double p_final_x, double p_final_y);
// void fCalcBB_individual(DescritorTipoInfo tp, Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h); // Se usado por pontoFinalInternoAFormaAlvoCallback


// Estrutura para callback de percorreLista ao processar alvos atingidos pelo blow
typedef struct {
    SmuTreap tree_ctx;
    FILE* file_txt_ctx;
    Lista lista_svg_ctx;
    Lista nos_geral_remover_ctx; // Lista para acumular todos os nós a serem removidos
    Node no_ogiva_explodida_ctx; // Para evitar que a ogiva se "atinja" a si mesma na lista de alvos
} ProcessaAlvoBlowDataCallback;

// Callback para processar cada alvo atingido pela explosão do "blow"
void processaAlvoAtingidoBlowCallback(Item item_no_alvo, void* aux_data) {
    ProcessaAlvoBlowDataCallback* data = (ProcessaAlvoBlowDataCallback*)aux_data;
    Node no_alvo_atual = (Node)item_no_alvo;

    // A ogiva que explodiu já será adicionada à lista de remoção principal.
    // Não precisamos fazer nada se o "alvo" for a própria ogiva.
    if (no_alvo_atual == data->no_ogiva_explodida_ctx) {
        return;
    }

    // Adicionar o alvo à lista de nós a serem removidos
    // (Idealmente, verificar se já está na lista para evitar duplicatas antes de inserir)
    insereNaLista(data->nos_geral_remover_ctx, no_alvo_atual);

    // Obter informações do alvo para TXT e SVG
    Info info_alvo = getInfoSmuT(data->tree_ctx, no_alvo_atual);
    DescritorTipoInfo tipo_alvo = getTypeInfoSmuT(data->tree_ctx, no_alvo_atual);
    int id_alvo = -1;
    char* nome_tipo_alvo = "Desconhecido";
    double ancora_alvo_svg_x = 0.0, ancora_alvo_svg_y = 0.0; // Âncora da FORMA ALVO para o 'x' do SVG

    if (tipo_alvo == TIPO_CIRCULO) { CIRCLE c = (CIRCLE)info_alvo; id_alvo = get_idC(c); nome_tipo_alvo = "circulo"; ancora_alvo_svg_x = get_XC(c); ancora_alvo_svg_y = get_YC(c); }
    else if (tipo_alvo == TIPO_RETANGULO) { RECTANGLE r_ = (RECTANGLE)info_alvo; id_alvo = get_idR(r_); nome_tipo_alvo = "retangulo"; ancora_alvo_svg_x = get_XR(r_); ancora_alvo_svg_y = get_YR(r_); }
    else if (tipo_alvo == TIPO_LINHA) { LINHA l = (LINHA)info_alvo; id_alvo = get_idL(l); nome_tipo_alvo = "linha"; ancora_alvo_svg_x = get_X1L(l); ancora_alvo_svg_y = get_Y1L(l); }
    else if (tipo_alvo == TIPO_TEXTO) { TEXTO t_ = (TEXTO)info_alvo; id_alvo = get_idT(t_); nome_tipo_alvo = "texto"; ancora_alvo_svg_x = get_XT(t_); ancora_alvo_svg_y = get_YT(t_); }

    fprintf(data->file_txt_ctx, "    ID %d (%s) na PosAncora (%.2f, %.2f)\n", id_alvo, nome_tipo_alvo, ancora_alvo_svg_x, ancora_alvo_svg_y);

    // Anotação SVG para forma destruída ('x')
    char* anot_svg_destruido = (char*)malloc(200 * sizeof(char));
    if (anot_svg_destruido) {
        sprintf(anot_svg_destruido, "<text x=\"%.2f\" y=\"%.2f\" fill=\"red\" font-size=\"12\" font-weight=\"bold\">x</text>",
                ancora_alvo_svg_x, ancora_alvo_svg_y);
        insereNaLista(data->lista_svg_ctx, (Item)anot_svg_destruido);
    }
}


void handle_blow(SmuTreap tree, FILE* pathTxtOut, int id_ogiva_a_explodir, Lista lista_anotacoes_svg, Lista* array_selecoes) {
    fprintf(pathTxtOut, "[*] blow %d\n", id_ogiva_a_explodir);
    printf("Processando comando blow: id_ogiva_a_explodir=%d\n", id_ogiva_a_explodir);

    // 1. Encontrar a "ogiva" (forma) com o ID fornecido
    SearchDataById dados_ogiva; // Reutilizando a struct definida para handle_spy
    dados_ogiva.id_procurado = id_ogiva_a_explodir;
    dados_ogiva.no_encontrado = NULL;

    // Reutiliza o findFormByIdCallback (definido em handle_spy)
    procuraNoSmuT(tree, findFormByIdCallback, &dados_ogiva);

    if (!dados_ogiva.no_encontrado) {
        fprintf(pathTxtOut, "Erro: Ogiva com ID %d nao encontrada para explodir.\n", id_ogiva_a_explodir);
        return;
    }

    Node no_ogiva = dados_ogiva.no_encontrado;
    Info info_ogiva = dados_ogiva.info_encontrada;
    DescritorTipoInfo tipo_ogiva = dados_ogiva.tipo_encontrado;

    // 2. Determinar o local da explosão (âncora da ogiva)
    double local_explosao_x, local_explosao_y;
    char* nome_tipo_ogiva = "Desconhecido";
    int id_ogiva_real = -1; // Para o relatório TXT, o ID já é id_ogiva_a_explodir

    // Obtém a âncora do nó da SmuTreap (que é o local da explosão)
    // E o tipo/id para o relatório.
    // Se sua SmuTreap.h não tiver getCoordenadasAncoraNo, use a âncora da Info:
    if (tipo_ogiva == TIPO_CIRCULO) { CIRCLE c = (CIRCLE)info_ogiva; id_ogiva_real = get_idC(c); nome_tipo_ogiva = "circulo"; local_explosao_x = get_XC(c); local_explosao_y = get_YC(c); }
    else if (tipo_ogiva == TIPO_RETANGULO) { RECTANGLE r_ = (RECTANGLE)info_ogiva; id_ogiva_real = get_idR(r_); nome_tipo_ogiva = "retangulo"; local_explosao_x = get_XR(r_); local_explosao_y = get_YR(r_); }
    else if (tipo_ogiva == TIPO_LINHA) { LINHA l = (LINHA)info_ogiva; id_ogiva_real = get_idL(l); nome_tipo_ogiva = "linha"; local_explosao_x = get_X1L(l); local_explosao_y = get_Y1L(l); // Ou a âncora efetiva do nó na SmuTreap  }
    else if (tipo_ogiva == TIPO_TEXTO) { TEXTO t_ = (TEXTO)info_ogiva; id_ogiva_real = get_idT(t_); nome_tipo_ogiva = "texto"; local_explosao_x = get_XT(t_); local_explosao_y = get_YT(t_); }
    else {
        fprintf(pathTxtOut, "Erro: Ogiva ID %d tem tipo desconhecido (%d).\n", id_ogiva_a_explodir, tipo_ogiva);
        return;
    }

    // 3. Reportar no TXT que a ogiva explodiu
    fprintf(pathTxtOut, "Ogiva ID %d (%s) explodiu em sua posicao (%.2f, %.2f).\n",
            id_ogiva_a_explodir, nome_tipo_ogiva, local_explosao_x, local_explosao_y);

    // 4. Adicionar anotação SVG para o local da explosão ('#')
    char* anot_svg_explosao = (char*)malloc(200 * sizeof(char));
    if (anot_svg_explosao) {
        sprintf(anot_svg_explosao, "<text x=\"%.2f\" y=\"%.2f\" fill=\"orange\" font-size=\"12\" font-weight=\"bold\">#</text>",
                local_explosao_x, local_explosao_y);
        insereNaLista(lista_anotacoes_svg, (Item)anot_svg_explosao);
    }

    // 5. Encontrar formas atingidas pela explosão no local (local_explosao_x, local_explosao_y)
    //    Reutiliza pontoFinalInternoAFormaAlvoCallback de handle_disp
    Lista lista_temporal_alvos_atingidos = criaLista();
    if (!lista_temporal_alvos_atingidos) {
        fprintf(pathTxtOut, "  Erro critico: Falha ao criar lista para alvos atingidos.\n");
        // A ogiva ainda precisa ser removida, mesmo que não possamos verificar alvos.
        // (Lógica de remoção da ogiva aqui se a função terminar prematuramente)
        return;
    }
    getInfosAtingidoPontoSmuT(tree, local_explosao_x, local_explosao_y, pontoFinalInternoAFormaAlvoCallback, lista_temporal_alvos_atingidos);

    // 6. Preparar lista de todos os nós a serem removidos e processar alvos
    Lista nos_a_remover_desta_explosao = criaLista();
    if (!nos_a_remover_desta_explosao) {
        fprintf(pathTxtOut, "  Erro critico: Falha ao criar lista final para remocao de nos.\n");
        destroiLista(lista_temporal_alvos_atingidos, NULL);
        return;
    }
    // A ogiva que explodiu é sempre destruída.
    insereNaLista(nos_a_remover_desta_explosao, (Item)no_ogiva);

    if (listaEstaVazia(lista_temporal_alvos_atingidos)) {
        fprintf(pathTxtOut, "  Nenhuma forma adicional foi atingida pela explosao.\n");
    } else {
        fprintf(pathTxtOut, "  Formas atingidas pela explosao:\n");
        ProcessaAlvoBlowDataCallback data_para_callback_alvos;
        data_para_callback_alvos.tree_ctx = tree;
        data_para_callback_alvos.file_txt_ctx = pathTxtOut;
        data_para_callback_alvos.lista_svg_ctx = lista_anotacoes_svg;
        data_para_callback_alvos.nos_geral_remover_ctx = nos_a_remover_desta_explosao; // Todos os alvos vão para esta lista
        data_para_callback_alvos.no_ogiva_explodida_ctx = no_ogiva;

        percorreLista(lista_temporal_alvos_atingidos, processaAlvoAtingidoBlowCallback, &data_para_callback_alvos);
    }
    destroiLista(lista_temporal_alvos_atingidos, NULL); // Libera a lista temporária de coleta de alvos

    // 7. Remover efetivamente todos os nós coletados (ogiva + alvos únicos) da SmuTreap
    //    É importante garantir que cada nó seja removido apenas uma vez.
    Lista unicos_para_remover = criaLista(); // Para garantir remoção única
    if (unicos_para_remover) {
        NoLista* iter_coleta = getPrimeiroNoLista(nos_a_remover_desta_explosao); // Requer funções de iteração do TAD Lista
        while (iter_coleta) {
            Node no_candidato = getItemLista(iter_coleta);
            bool ja_em_unicos = false;
            // Verifica se no_candidato já está em unicos_para_remover
            // (Requer uma função ListaContemItem ou iteração manual em unicos_para_remover)
            // Exemplo de verificação manual (ineficiente para listas grandes):
            NoLista* check_unico = getPrimeiroNoLista(unicos_para_remover);
            while(check_unico) {
                if(getItemLista(check_unico) == no_candidato) {
                    ja_em_unicos = true;
                    break;
                }
                check_unico = getProximoNoLista(check_unico);
            }

            if (!ja_em_unicos) {
                insereNaLista(unicos_para_remover, no_candidato);
            }
            iter_coleta = getProximoNoLista(iter_coleta);
        }

        // Agora, remove os nós da lista de únicos
        NoLista* iter_remocao = getPrimeiroNoLista(unicos_para_remover);
        while (iter_remocao) {
            Node no_a_remover_final = (Node)getItemLista(iter_remocao);
            removeNoSmuT(tree, no_a_remover_final); // removeNoSmuT deve liberar Info e Node

            // Se o nó removido estava em alguma lista de seleção, essa referência se torna inválida.
            // O projeto não especifica limpar seleções, mas seria bom.
            // (Opcional: iterar array_selecoes e remover no_a_remover_final de cada lista de seleção)
            if (array_selecoes) { // Verifica se array_selecoes é válido
                for (int i = 0; i < 100; i++) {
                    if (array_selecoes[i]) {
                        // Se você tiver uma função como:
                        // ListaRemoveTodasOcorrenciasDoItem(array_selecoes[i], (Item)no_a_remover_final);
                        // Chame-a aqui.
                    }
                }
            }
            iter_remocao = getProximoNoLista(iter_remocao);
        }
        destroiLista(unicos_para_remover, NULL);
    }
    destroiLista(nos_a_remover_desta_explosao, NULL);
}
//---------------------------------------------------------------------------------------------------------------------//
*/

void leitura_qry(SmuTreap t, FILE *arqQry, FILE *pathTxt, Lista *array_selecoes, Lista lista_anotacoes_svg, int *idMax, FCalculaBoundingBox fCalcBb, double epsilon){
    CONTEXTO con = iniciaContext(pathTxt, t, lista_anotacoes_svg, array_selecoes, idMax, fCalcBb, epsilon);

    char *str = (char*)malloc(sizeof(char)*1024);
    if(!str){
        fprintf(stderr, "(leitura_qry) Erro: falha ao alocar memoria para str.");
        return;
    }
    
    char *comm = (char*)malloc(sizeof(char)*7);
    if(!str){
        fprintf(stderr, "(leitura_qry) Erro: falha ao alocar memoria para comm.");
        return;
    }

    while (fgets(str, 1024, arqQry)) {
        // Remover nova linha do final, se houver
        str[strcspn(str, "\n")] = 0;
        str[strcspn(str, "\r")] = 0; // Para compatibilidade Windows/Linux
        
        //printf("str ANTES = %s\n\n", str);
        sscanf(str, "%7s", comm);


        // Processar com base no comando
        if (strcmp(comm, "selr") == 0) {
            int id;
            double x, y, w, h;
            // sscanf é uma boa forma de parsear o restante da str_original
            if (sscanf(str, "selr %d %lf %lf %lf %lf", &id, &x, &y, &w, &h) == 5) {
                handle_selr(con, id, x, y, w, h);
            } else {
                fprintf(pathTxt, "[*] %s\nErro: parametros invalidos para selr\n", str);
            }
        } else if (strcmp(comm, "seli") == 0) {
            int id;
            double x, y;
            if (sscanf(str, "seli %d %lf %lf", &id, &x, &y) == 3) {
                handle_seli(con, id, x, y);
            } else {
                fprintf(pathTxt, "[*] %s\nErro: parametros invalidos para seli\n", str);
            }
        } else if (strcmp(comm, "disp") == 0) {
            int id, n_sel;
            if (sscanf(str, "disp %d %d", &id, &n_sel) == 2) {
                handle_disp(con, id, n_sel);
            } else {
                fprintf(pathTxt, "[*] %s\nErro: parametros invalidos para disp\n", str);
            }
        } else if (strcmp(comm, "transp") == 0) {
            int id;
            double x, y;
            if (sscanf(str, "transp %d %lf %lf", &id, &x, &y) == 3) {
                handle_transp(con, id, x, y);
            } else {
                fprintf(pathTxt, "[*] %s\nErro: parametros invalidos para transp\n", str);
            }
        } else if (strcmp(comm, "cln") == 0) {
            int n;
            double dx, dy;
            if (sscanf(str, "cln %d %lf %lf", &n, &dx, &dy) == 3) {
                handle_cln(con, n, dx, dy);
            } else {
                fprintf(pathTxt, "[*] %s\nErro: parametros invalidos para cln\n", str);
            }
        } else if (strcmp(comm, "spy") == 0) {
            int id;
            if (sscanf(str, "spy %d", &id) == 1) {
                handle_spy(con, id);
            } else {
                fprintf(pathTxt, "[*] %s\nErro: parametros invalidos para spy\n", str);
            }
        } else if (strcmp(comm, "cmflg") == 0) {
            int id, w_px;
            char cb[50], cp[50];
            // Formato: cmflg id corBorda corPreenchimento larguraPx
            if (sscanf(str, "cmflg %d %s %s %d", &id, cb, cp, &w_px) == 4) {
                handle_cmflg(con, id, cb, cp, w_px);
            } else {
                fprintf(pathTxt, "[*] %s\nErro: parametros invalidos para cmflg\n", str);
            }
        } else if (strcmp(comm, "blow") == 0) {
            int id;
            if (sscanf(str, "blow %d", &id) == 1) {
                handle_blow(con, id);
            } else {
                fprintf(pathTxt, "[*] %s\nErro: parametros invalidos para blow\n", str);
            }
        } else {
            // Comando desconhecido
            fprintf(pathTxt, "[*] %s\nComando desconhecido: %s\n", str, comm);
            printf("Comando desconhecido: %s\n", comm);
        }
    }
}

SmuTreap processa_qry(SmuTreap t, const char *pathQry, const char *pathSaida, const char *nomeQry, Lista *array_anotacoes, Lista lista_anotacoes_svg, int *idMax, double epsilon){
    if(!pathQry || !pathSaida || !nomeQry || !array_anotacoes || !lista_anotacoes_svg){
        fprintf(stderr, "\n(processa_qry) Erro: parametros invalidos.\n");
        return NULL;
    }


    FILE *arqQry = fopen(pathQry, "r");
    if (!arqQry){
        fprintf(stderr, "\n(processa_qry) Erro: falha ao abrir arquivo qry.\n");
        exit (1);
    }

    // PENSAR EM COMO PASSAR NOMEGEO DE MODO QUE NAO SEJA NECESSARIO ESSA PARTE NESSA FUNCAO.
    // Remover ".qry" de nomeQry.
    char nomeBaseQry[512];
    int lenNomeQry = strlen(nomeQry);

    if (lenNomeQry > 4 && strcmp(nomeQry + lenNomeQry - 4, ".qry") == 0) {
        strncpy(nomeBaseQry, nomeQry, lenNomeQry - 4); // Copia a parte antes de ".geo"
        nomeBaseQry[lenNomeQry - 4] = '\0';
    } else {
        fprintf(stderr, "\n(processa_qry) Erro: erro ao tratar nomeQry.\n");
        fclose(arqQry);
        exit(1);
    }

    char nome_saidasvg2[512];
    int chars_escritos_svg2 = snprintf(nome_saidasvg2, sizeof(nome_saidasvg2), "%s/%s.svg", pathSaida, nomeBaseQry);

if (chars_escritos_svg2 >= (int)sizeof(nome_saidasvg2)) {
    fprintf(stderr, "\nAlerta (processa_qry): Nome do arquivo SVG '%s/%s.svg' foi truncado para '%s'.\n", pathQry, nomeBaseQry, nome_saidasvg2);
    exit(1);
}
    printf("Diretório do arquivo svg2: %s\n", nome_saidasvg2);

    FILE* saidaSvg2 = fopen(nome_saidasvg2, "w");
    if (saidaSvg2 == NULL) {
        fprintf(stderr, "\n(processa_qry) Erro na criação do arquivo SVG: %s\n", nome_saidasvg2);
        fclose(arqQry);
        exit(1);
    }

    fprintf(saidaSvg2, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(saidaSvg2, "<svg width=\"2000\" height=\"2000\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");



    char nome_saidaTxt[512];
    int chars_escritos_Txt = snprintf(nome_saidaTxt, sizeof(nome_saidaTxt), "%s/%s.txt", pathSaida, nomeBaseQry);

    if(chars_escritos_Txt >= (int)sizeof(nome_saidaTxt)){
        fprintf(stderr, "\nAlerta (processa_qry): Nome do arquivo SVG '%s/%s.svg' foi truncado para '%s'.\n", pathQry, nomeBaseQry, nome_saidasvg2);
        exit(1);
    }
    printf("Diretório do arquivo stxt: %s\n", nome_saidaTxt);

    FILE* saidaTxt = fopen(nome_saidaTxt, "w");
    if (saidaTxt == NULL) {
        fprintf(stderr, "\nErro na criação do arquivo txt: %s\n", nome_saidaTxt);
        fclose(arqQry);
        fclose(saidaSvg2);
        exit(1);
    }

    leitura_qry(t, arqQry, saidaTxt, array_anotacoes, lista_anotacoes_svg, idMax, fCalcBB_individual, epsilon);

    // Escreve a arvore modificada no svg 2.
    visitaProfundidadeSmuT(t, escreverFormaSvg1, (void *)saidaSvg2);
    
    if(!listaEstaVazia(lista_anotacoes_svg)){ // Se há anotacoes, as escreve no svg 2.
        printf("Escrevendo anotacoes no svg 2...\n");
        percorreLista(lista_anotacoes_svg, visitaListaSvg, (void*)saidaSvg2);
    } else printf("Não há anotacoes a serem escritas no svg 2.\n");

    destroiLista(lista_anotacoes_svg, killAnotacaoCallback);

    fprintf(saidaSvg2, "</svg>\n");
    fclose(arqQry);
    fclose(saidaTxt);
    fclose(saidaSvg2);
    return t;


}
