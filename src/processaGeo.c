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


#define TIPO_CIRCULO 1
#define TIPO_RETANGULO 2
#define TIPO_TEXTO 3
#define TIPO_LINHA 4

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

SmuTreap leitura_geo(FILE *arqGeo, SmuTreap t, FCalculaBoundingBox funcCalcBb, int *instru, int *formcriadas/*, int *idUltimaForma*/) {

    char str[500];
    char tipo[4]; //Para "c ", "r ", "l ", "t ", "ts" + null
    char fam[50] = "sans-serif"; // Default
    char wei[50] = "normal";     // Default
    char size[50] = "10";        // Default

    *formcriadas = 0;
    *instru = 0;

    //FORMASGEO f1;
    while (fgets(str, 500, arqGeo)) {

        // Remove newline characters
        str[strcspn(str, "\n")] = 0;
        str[strcspn(str, "\r")] = 0;
        
        sscanf(str, "%3s", tipo); // Lê até 3 caracteres para o comando.
        (*instru)++;


        if (strcmp(tipo, "ts") == 0) {
            sscanf(str + 2, "%s %s %s", fam, wei, size); //+2 pula "ts"
            printf("\n\n Ts lido!\nlinha lida: %s %s %s %s", tipo, fam, wei, size);
        }else 

        if (strcmp(tipo, "c ") == 0) {
            int i;
            double x, y, r;

            char cb[50];
            char cp[50];

            sscanf(str + 2, "%d %lf %lf %lf %s %s", &i, &x, &y, &r, cb, cp);
            //idUltimaForma = i, por algum motivo! Pode ser util depois
            CIRCLE c = create_circle(i, x, y, r, cb, cp);
            insertSmuT(t, x, y, (CIRCLE)c, TIPO_CIRCULO, funcCalcBb);

            printf("\n\n Circulo criado!\nlinha lida: %s %d %.4lf %.4lf %.4lf %s %s", tipo, i, x, y, r, cb, cp);
            (*formcriadas)++;
        } else

        if (strcmp(tipo, "r ") == 0) {
            int i;
            double x, y, h, w;

            char cb[50];
            char cp[50];

            sscanf(str + 2, "%d %lf %lf %lf %lf %s %s", &i, &x, &y, &w, &h, cb, cp);
            //idUltimaForma = i, por algum motivo! Pode ser util depois
            RECTANGLE r = create_rectangle(i, x, y, w, h, cb, cp);
            insertSmuT(t, x, y, (RECTANGLE)r, TIPO_RETANGULO, funcCalcBb);

            printf("\n\n Retangulo criado!\nlinha lida: %s %d %.4lf %.4lf %.4lf %.4lf %s %s", tipo, i, x, y, w, h, cb, cp);
            (*formcriadas)++;
        } else

        if (strcmp(tipo, "l ") == 0) {
            int i;
            double x1, y1, x2, y2;

            char c_cor[50];

            sscanf(str + 2, "%d %lf %lf %lf %lf %s", &i, &x1, &y1, &x2, &y2, c_cor);
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
            (*formcriadas)++;
        } else

        if (strcmp(tipo, "t ") == 0) {
            int i;
            double x, y;

            char cb[50];
            char cp[50];

            char a;

            char txt_contnt[500];
            int consumed; // armazenar o número de caracteres consumidos (lidos) pela sscanf até ponto x;
            sscanf(str + 1, "%d %lf %lf %49s %49s %c %n", &i, &x, &y, cb, cp, &a, &consumed);

            char* p_inicio_texto = str + 1 + consumed; // Pula comando, id, x, y, cb, cp, a, e espaços

            while(*p_inicio_texto == ' ') p_inicio_texto++; // Pula espaços antes da aspa inicial

            if(*p_inicio_texto == '"') {
                p_inicio_texto++; // Pula a aspa inicial
                char* p_fim_texto = strrchr(p_inicio_texto, '"'); // Encontra a última aspa

                if(p_fim_texto) {
                    strncpy(txt_contnt, p_inicio_texto, p_fim_texto - p_inicio_texto);
                    txt_contnt[p_fim_texto - p_inicio_texto] = '\0';
                } else {
                    fprintf(stderr, "(leituraGeo) Erro: falha ao ler txt_cntnt para criacao de forma texto.");
                    strcpy(txt_contnt, "ERRO_PARSE_TEXTO"); // Fallback
                }
            } else {
                fprintf(stderr, "(leituraGeo) Erro: falha ao ler txt_cntnt para criacao de forma texto.");
                strcpy(txt_contnt, "ERRO_PARSE_TEXTO_ASPAS"); // Fallback
            }
            //idUltimaForma = i, por algum motivo! Pode ser util depois
            TEXTO t = cria_texto(i, x, y, cb, cp, a, txt_contnt, fam, wei, size);
            insertSmuT(t, x, y, (TEXTO)t, TIPO_TEXTO, funcCalcBb);

            printf("\n\n Texto criado!\nlinha lida: %s %d %.2lf %.2lf %s %s %c \"%s\"", tipo, i, x, y, cb, cp, a, txt_contnt);
            (*formcriadas)++;
        }
    }
    //percursoSimetrico(a1, &printasvg, ssvg1);

    return t;
}
// Não esquecer de processar os default, e nem de consultar o que os documentos fala sobre isso.
SmuTreap processa_geo(const char* pathgeo, const char* dirsaida, const char* nomegeo, int *instru, 
                    int *formcriadas, /*int *idUltimaForma,*/ int *prioMax, int *hc, double *promoRate, double epsilonConfig){

    int def_prioMax;
    int def_hc;
    double def_promoRate;
    if(prioMax == NULL){
        def_prioMax = DEFAULT_PRIOMAX;
    }
    if(hc == NULL){
        def_hc = DEFAULT_HITCOUNT;   
    }
    if(promoRate == NULL){
        def_promoRate = DEFAULT_PROMORATE;
    }

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
    leitura_geo(arqGeo, t, fCalcBB_individual, instru, formcriadas);

    // FALTA IMPLEMENTAR!
    // Gera o SVG inicial percorrendo a SmuTreap
    //visitaProfundidadeSmuT(arvoreFormas, escreverFormaSvg, (void *)ssvg1); // Ou visitaLarguraSmuT


    fprintf(saidaSvg1, "</svg>\n");
    fclose(arqGeo);
    fclose(saidaSvg1);
    return t;
}