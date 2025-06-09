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
#include "processaSvg.h"

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
    visitaProfundidadeSmuT(t, escreverFormaSvg, (void *)saidaSvg2);
    
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
