#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "lancadores.h"
#include "linha.h"
#include "circulo.h"
#include "texto.h"
#include "retangulo.h"
#include "SmuTreap.h"
#include "boundingBox.h"
//#include "arvorebin.h"
//#include "formas.h"


#ifndef TIPO_FORMAS_DEFINED
#define TIPO_FORMAS_DEFINED
#define TIPO_CIRCULO 1
#define TIPO_RETANGULO 2
#define TIPO_TEXTO 3
#define TIPO_LINHA 4
#endif

#define DEFAULT_PRIOMAX 10000
#define DEFAULT_HITCOUNT 3
#define DEFAULT_PROMORATE 1.10

#define EPSILON_CONFIG 0.0001

int ma(double x1, double x2, double y1, double y2){
    if(x1<x2){
        return 1;
    } else if(x2<x1){
        return 2;
    } else{
        if(y1<=y2){
            return 3;
        } else{
            return 4;
        }
    }
}

// Função para ser usada com visitaProfundidadeSmuT ou visitaLarguraSmuT
void escreverFormaSvg(SmuTreap t, Node n, Info i, double x_ancora_no, double y_ancora_no, void *aux_file_ptr) {
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

            fprintf(arq_svg, "\t<circle cx=\"%.2f\" cy=\"%.2f\" r=\"%.2f\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"0.5\" />\n", xc, yc, r_circ, corb, corp);
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

            fprintf(arq_svg, "\t<rect x=\"%.2f\" y=\"%.2f\" width=\"%.2f\" height=\"%.2f\" stroke=\"%s\" fill=\"%s\" fill-opacity=\"0.5\" />\n", xr, yr, w, h, corb, corp);
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

            fprintf(arq_svg, "\t<line x1=\"%.2f\" y1=\"%.2f\" x2=\"%.2f\" y2=\"%.2f\" stroke=\"%s\" stroke-width=\"1\" />\n", x1, y1, x2, y2, cor);
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
            char *ancora_svg = get_ancoraT(txt_fig); // Retorna "start", "middle", ou "end"
            char *conteudo_txt = get_txtT(txt_fig);
            char *ff = get_ffT(txt_fig);
            char *fw = get_fwT(txt_fig);
            char *fs = get_fsT(txt_fig);

            // A cor da borda do texto em SVG é controlada por 'stroke', não 'fill' da borda.
            // O preenchimento do texto é 'fill'.
            fprintf(arq_svg, "\t<text x=\"%.2f\" y=\"%.2f\" stroke=\"%s\" fill=\"%s\" font-family=\"%s\" font-weight=\"%s\" font-size=\"%s\" text-anchor=\"%s\" dominant-baseline=\"middle\">",
                    xt, yt, corb, corp, ff, fw, fs, ancora_svg);

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
            fprintf(stderr, "Aviso (escreverFormaSvg): Tipo de forma desconhecido (%d) encontrado no no com ancora (%.2f, %.2f).\n", tipo_forma, x_ancora_no, y_ancora_no);
            break;
    }
}

SmuTreap leitura_geo(FILE *arqGeo, SmuTreap t, FCalculaBoundingBox funcCalcBb) {

    char str[500];
    char tipo[4]; //Para "c ", "r ", "l ", "t ", "ts" + null
    char fam[50] = "sans-serif"; // Default
    char wei[50] = "normal";     // Default
    char size[50] = "10";        // Default

    //FORMASGEO f1;
    while (fgets(str, 500, arqGeo)) {

        // Remove newline characters
        str[strcspn(str, "\n")] = 0;
        str[strcspn(str, "\r")] = 0;
        
        sscanf(str, "%3s", tipo); // Lê até 3 caracteres para o comando.


        if (strcmp(tipo, "ts") == 0) {
            if (sscanf(str + 2, "%s %s %s", fam, wei, size) == 3){  //+2 pula "ts"
                printf("\n\n Ts lido!\nlinha lida: %s %s %s %s", tipo, fam, wei, size);
            } 
        }else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para ts.");

        if (strcmp(tipo, "c") == 0) {
            int i;
            double x, y, r;

            char cb[50];
            char cp[50];

            if (sscanf(str + 2, "%d %lf %lf %lf %s %s", &i, &x, &y, &r, cb, cp)){
                //idUltimaForma = i, por algum motivo! Pode ser util depois
                CIRCLE c = create_circle(i, x, y, r, cb, cp);
                insertSmuT(t, x, y, (CIRCLE)c, TIPO_CIRCULO, funcCalcBb);
                
                printf("\n\n Circulo criado!\nlinha lida: %s %d %.4lf %.4lf %.4lf %s %s", tipo, i, x, y, r, cb, cp);
            } else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para c.");

        } else

        if (strcmp(tipo, "r") == 0) {
            int i;
            double x, y, h, w;

            char cb[50];
            char cp[50];

            if (sscanf(str + 2, "%d %lf %lf %lf %lf %s %s", &i, &x, &y, &w, &h, cb, cp)){
                //idUltimaForma = i, por algum motivo! Pode ser util depois
                RECTANGLE r = create_rectangle(i, x, y, w, h, cb, cp);
                insertSmuT(t, x, y, (RECTANGLE)r, TIPO_RETANGULO, funcCalcBb);
    
                printf("\n\n Retangulo criado!\nlinha lida: %s %d %.4lf %.4lf %.4lf %.4lf %s %s", tipo, i, x, y, w, h, cb, cp);
            } else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para r.");
        } else

        if (strcmp(tipo, "l") == 0) {
            int i;
            double x1, y1, x2, y2;

            char c_cor[50];

            if (sscanf(str + 2, "%d %lf %lf %lf %lf %s", &i, &x1, &y1, &x2, &y2, c_cor)){
                //idUltimaForma = i, por algum motivo! Pode ser util depois
                LINHA l = cria_linha(i, x1, y1, x2, y2, c_cor);
    
                double anc_x, anc_y;
                int maior = ma(x1, x2, y1, y2);
                if(maior == 1 || maior == 3){
                    anc_x = x1; anc_y = y1;
                } else {
                    anc_x = x2; anc_y = y2;
                }
    
                insertSmuT(t, anc_x, anc_y, (LINHA) l, TIPO_LINHA, funcCalcBb);
    
                printf("\n\n Linha criada!\nlinha lida: %s %d %.4lf %.4lf %.4lf %.4lf %s", tipo, i, x1, y1, x2, y2, c_cor);
            } else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para l.");
        } else

        if (strcmp(tipo, "t") == 0) {
            int i;
            double x, y;

            char* cb = (char*)malloc(15 * sizeof(char));
            char* cp = (char*)malloc(15 * sizeof(char));

            char a;

            char* txt = (char*)malloc(300 * sizeof(char));
            if (sscanf(str + 2, "%d %lf %lf %s %s %c %[^\n]", &i, &x, &y, cb, cp, &a, txt)){
                TEXTO tx = cria_texto(i, x, y, cb, cp, a, txt, fam, wei, size);
                insertSmuT(t, x, y, (TEXTO)tx, TIPO_TEXTO, funcCalcBb);
    
                printf("\n\n Texto criado!\nlinha lida: %s %d %.2lf %.2lf %s %s %c %s", tipo, i, x, y, cb, cp, a, txt);
    
                free(cb);
                free(cp);
                free(txt);
            } else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para t.");
        }
    }

    return t;
}
// Não esquecer de processar os default, e nem de consultar o que os documentos fala sobre isso.
SmuTreap processa_geo(const char* pathgeo, const char* dirsaida, const char* nomegeo, 
                    /*int *idUltimaForma,*/ int *prioMax, int *hc, double *promoRate, double epsilonConfig){

    int def_prioMax;
    int def_hc;
    double def_promoRate;
    if(prioMax == NULL){
        def_prioMax = DEFAULT_PRIOMAX;
    } else def_prioMax = *prioMax;

    if(hc == NULL){
        def_hc = DEFAULT_HITCOUNT;   
    } else def_hc = *hc;
    if(promoRate == NULL){
        def_promoRate = DEFAULT_PROMORATE;
    } else def_promoRate = *promoRate;

    FILE* arqGeo = fopen(pathgeo, "r");
    printf("Diretório do arquivo geo: %s\n", pathgeo);
    if (arqGeo==NULL) {
        fprintf(stderr, "Erro na abertura do arquivo GEO: %s\n", pathgeo);
        exit(1);
    }

    // PENSAR EM COMO PASSAR NOMEGEO DE MODO QUE NAO SEJA NECESSARIO ESSA PARTE NESSA FUNCAO.
    // Remover ".geo" de nomegeo.
    char nomeBaseGeo[512];
    int lenNomeGeo = strlen(nomegeo);

    if (lenNomeGeo > 4 && strcmp(nomegeo + lenNomeGeo - 4, ".geo") == 0) {
        strncpy(nomeBaseGeo, nomegeo, lenNomeGeo - 4); // Copia a parte antes de ".geo"
        nomeBaseGeo[lenNomeGeo - 4] = '\0';
    } else {
        fprintf(stderr, "(processa_geo) Erro: erro ao tratar nomegeo.");
        exit(1);
    }

    char nome_saidasvg1[512];
    int chars_escritos = snprintf(nome_saidasvg1, sizeof(nome_saidasvg1), "%s/%s.svg", dirsaida, nomeBaseGeo);

if (chars_escritos >= (int)sizeof(nome_saidasvg1)) {
    fprintf(stderr, "Alerta (processa_geo): Nome do arquivo SVG '%s/%s.svg' foi truncado para '%s'.\n", dirsaida, nomeBaseGeo, nome_saidasvg1);
    exit(1);
}
    printf("Diretório do arquivo svg1: %s\n", nome_saidasvg1);

    FILE* saidaSvg1 = fopen(nome_saidasvg1, "w");
    if (saidaSvg1 == NULL) {
        fprintf(stderr, "Erro na criação do arquivo SVG: %s\n", nome_saidasvg1);
        fclose(arqGeo);
        exit(1);
    }

    fprintf(saidaSvg1, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(saidaSvg1, "<svg width=\"2000\" height=\"2000\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");

    SmuTreap t = newSmuTreap(def_hc, def_prioMax, def_promoRate, EPSILON_CONFIG);
    if (t == NULL) {
        fprintf(stderr, "(processa_geo) Erro: falha ao criar SmuTreap.\n");
        fclose(arqGeo);
        fclose(saidaSvg1);
        exit(1);
    }

    // Popula a arvore t.
    leitura_geo(arqGeo, t, fCalcBB_individual);

    // Gera o SVG inicial percorrendo a SmuTreap
    visitaProfundidadeSmuT(t, escreverFormaSvg, (void *)saidaSvg1); // Ou visitaLarguraSmuT


    fprintf(saidaSvg1, "</svg>\n");
    fclose(arqGeo);
    fclose(saidaSvg1);
    return t;
}