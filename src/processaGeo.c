#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linha.h"
#include "circulo.h"
#include "texto.h"
#include "retangulo.h"
#include "SmuTreap.h"
#include "boundingBox.h"
#include "formas.h"
#include "processaSvg.h"

#define DEFAULT_PRIOMAX 10000
#define DEFAULT_HITCOUNT 3
#define DEFAULT_PROMORATE 1.10

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


SmuTreap leitura_geo(FILE *arqGeo, SmuTreap t, FCalculaBoundingBox funcCalcBb, int *idMax) {

    char str[500];
    char tipo[4]; //Para "c ", "r ", "l ", "t ", "ts" + null
    char fam[50] = "sans-serif"; // Default
    char wei[50] = "normal";     // Default
    char size[50] = "10";        // Default

    //FORMASGEO f1;
    while (fgets(str, 500, arqGeo)) {
        //printf("str no geo = %s\n", str);

        // Remove newline characters
        str[strcspn(str, "\n")] = 0;
        str[strcspn(str, "\r")] = 0;
        
        sscanf(str, "%3s", tipo); // Lê até 3 caracteres para o comando.


        if (strcmp(tipo, "ts") == 0) {
            if (sscanf(str + 2, "%s %s %s", fam, wei, size) == 3){  //+2 pula "ts"
                //printf("\n\n Ts lido!\nlinha lida: %s %s %s %s\n", tipo, fam, wei, size);
            } else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para ts.");
        }else

        if (strcmp(tipo, "c") == 0) {
            int i;
            double x, y, r;

            char cb[50];
            char cp[50];

            if (sscanf(str + 2, "%d %lf %lf %lf %s %s", &i, &x, &y, &r, cb, cp)){
                CIRCLE c = create_circle(i, x, y, r, cb, cp);
                if (i > *idMax) { *idMax = i; } // Atualiza o ID
                insertSmuT(t, x, y, (CIRCLE)c, TIPO_CIRCULO, funcCalcBb);
                
                //printf("\n\n Circulo criado!\nlinha lida: %s %d %.4lf %.4lf %.4lf %s %s\n", tipo, i, x, y, r, cb, cp);
            } else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para c.");

        } else

        if (strcmp(tipo, "r") == 0) {
            int i;
            double x, y, h, w;

            char cb[50];
            char cp[50];

            if (sscanf(str + 2, "%d %lf %lf %lf %lf %s %s", &i, &x, &y, &w, &h, cb, cp)){
                RECTANGLE r = create_rectangle(i, x, y, w, h, cb, cp);
                if (i > *idMax) { *idMax = i; } // Atualiza o ID
                insertSmuT(t, x, y, (RECTANGLE)r, TIPO_RETANGULO, funcCalcBb);
    
                //printf("\n\n Retangulo criado!\nlinha lida: %s %d %.4lf %.4lf %.4lf %.4lf %s %s\n", tipo, i, x, y, w, h, cb, cp);
            } else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para r.");
        } else

        if (strcmp(tipo, "l") == 0) {
            int i;
            double x1, y1, x2, y2;

            char c_cor[50];

            if (sscanf(str + 2, "%d %lf %lf %lf %lf %s", &i, &x1, &y1, &x2, &y2, c_cor)){
                LINHA l = cria_linha(i, x1, y1, x2, y2, c_cor);
                if (i > *idMax) { *idMax = i; } // Atualiza o ID
    
                double anc_x, anc_y;
                int maior = ma(x1, x2, y1, y2);
                if(maior == 1 || maior == 3){
                    anc_x = x1; anc_y = y1;
                } else {
                    anc_x = x2; anc_y = y2;
                }
    
                insertSmuT(t, anc_x, anc_y, (LINHA) l, TIPO_LINHA, funcCalcBb);
    
                //printf("\n\n Linha criada!\nlinha lida: %s %d %.4lf %.4lf %.4lf %.4lf %s\n", tipo, i, x1, y1, x2, y2, c_cor);
            } else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para l.");
        } else

        if (strcmp(tipo, "t") == 0) {
            int i;
            double x, y;

            char* cb = (char*)malloc(15 * sizeof(char));
            char* cp = (char*)malloc(15 * sizeof(char));

            char a;

            char* txt = (char*)malloc(300 * sizeof(char));
            if (sscanf(str + 2, "%d %lf %lf %s %s %c \"%[^\"]\"", &i, &x, &y, cb, cp, &a, txt)){
                TEXTO tx = cria_texto(i, x, y, cb, cp, a, txt, fam, wei, size);
                if (i > *idMax) { *idMax = i; } // Atualiza o ID
                insertSmuT(t, x, y, (TEXTO)tx, TIPO_TEXTO, funcCalcBb);
    
                //printf("\n\n Texto criado!\nlinha lida: %s %d %.2lf %.2lf %s %s %c %s\n", tipo, i, x, y, cb, cp, a, txt);
    
                free(cb);
                free(cp);
                free(txt);
            } else fprintf(stderr, "(leitura_geo) Erro: parametros invalidos para t.");
        }
    }

    return t;
}
// Não esquecer de processar os default, e nem de consultar o que os documentos fala sobre isso.
SmuTreap processa_geo(const char* pathgeo, const char* dirsaida, const char* nomeBaseGeo, int *idMax, int *prioMax, int *hc, double *promoRate, double epsilonConfig){

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

    SmuTreap t = newSmuTreap(def_hc, def_prioMax, def_promoRate, epsilonConfig);
    if (t == NULL) {
        fprintf(stderr, "(processa_geo) Erro: falha ao criar SmuTreap.\n");
        fclose(arqGeo);
        fclose(saidaSvg1);
        exit(1);
    }

    // Popula a arvore t.
    leitura_geo(arqGeo, t, fCalcBB_individual, idMax);
    (*idMax)++;
    if(t) printf("\nSucesso na populacao da arvore!\n");
    printf("Saindo de leitura_geo...\n");

    // Gera o SVG inicial percorrendo a SmuTreap
    visitaProfundidadeSmuT(t, escreverFormaSvg, (void *)saidaSvg1); // Ou visitaLarguraSmuT
    printf("\nSaindo da escrita do svg1...\n");


    fprintf(saidaSvg1, "</svg>\n");
    fclose(arqGeo);
    fclose(saidaSvg1);
    return t;
}