#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "SmuTreap.h"
#include "leituraTerminal.h"
#include "processaGeo.h"
#include "processaQry.h"
#include "Lista.h"


int main(int argc, char *argv[]) {
    // --- 1. Declaração de Variáveis ---
    char *dirEntrada = NULL, *dirSaida = NULL;
    char *nomeGeo = NULL, *nomeQry = NULL;
    
    int *prioMax = NULL, *hc = NULL;
    double *promoRate = NULL;
    double epsilon = 0.0001; // Valor padrão para epsilon
    
    SmuTreap t = NULL;
    Lista array_selecoes[100];
    int idMax = 0;

    // Inicializa o array de seleções
    for (int i = 0; i < 100; i++) {
        array_selecoes[i] = NULL;
    }

    // Prepara a criação pseudoaleatória para prioridades
    srand(time(NULL));

    // --- 2. Processamento dos Parâmetros da Linha de Comando ---
    printf("Lendo parametros da linha de comando...\n");
    int i = 1;
    while (i < argc) {
        if (strcmp(argv[i], "-e") == 0) {
            i++;
            if (i < argc) trataPath(&dirEntrada, argv[i]);
        } else if (strcmp(argv[i], "-f") == 0) {
            i++;
            if (i < argc) trataNomeArquivo(&nomeGeo, argv[i]);
        } else if (strcmp(argv[i], "-q") == 0) {
            i++;
            if (i < argc) trataNomeArquivo(&nomeQry, argv[i]);
        } else if (strcmp(argv[i], "-o") == 0) {
            i++;
            if (i < argc) trataPath(&dirSaida, argv[i]);
        } else if (strcmp(argv[i], "-p") == 0) {
            i++;
            if (i < argc) trataParamNumericoInt(&prioMax, argv[i]);
        } else if (strcmp(argv[i], "-hc") == 0) {
            i++;
            if (i < argc) trataParamNumericoInt(&hc, argv[i]);
        } else if (strcmp(argv[i], "-pr") == 0) {
            i++;
            if (i < argc) trataParamNumericoDouble(&promoRate, argv[i]);
        }
        i++;
    }

    // --- 3. Validação de Parâmetros e Construção de Paths ---
    if (!dirSaida || !nomeGeo) {
        fprintf(stderr, "ERRO: Os parametros -o (diretorio de saida) e -f (arquivo .geo) são obrigatorios.\n");
        goto cleanup; // Pula para a limpeza e encerra
    }

    char *nomeBaseGeo = NULL;
    removeExtensaoArq(nomeGeo, &nomeBaseGeo);

    char *nomeBaseQry = NULL;
    removeExtensaoArq(nomeQry, &nomeBaseQry);
    
    // Constrói os caminhos completos para os arquivos de entrada
    char *fullNomeGeo = NULL;
    char *fullNomeQry = NULL;
    completaNomeGeo(dirEntrada, nomeBaseGeo, &fullNomeGeo);
    if (nomeQry) {
        completaNomeQry(dirEntrada, nomeBaseGeo, nomeBaseQry, &fullNomeQry);
    }
    
    printf("\n--- Caminhos Processados ---\n");
    printf("Entrada: %s\n", fullNomeGeo);
    printf("Saida: %s\n", dirSaida);
    if(fullNomeQry) printf("Consultas: %s\n", fullNomeQry);
    printf("---------------------------\n\n");


    // --- 4. Lógica Principal da Aplicação ---
    
    // Processa o arquivo .geo e cria a árvore
    t = processa_geo(fullNomeGeo, dirSaida, nomeBaseGeo, &idMax, prioMax, hc, promoRate, epsilon);
    if (!t) {
        fprintf(stderr, "ERRO: Falha ao processar o arquivo .geo. Encerrando.\n");
        goto cleanup;
    }

    // Processa o arquivo .qry, se foi fornecido
    Lista lista_anotacoes_svg = criaLista();
    if (fullNomeQry) {
        processa_qry(t, fullNomeQry, dirSaida, nomeBaseGeo, nomeBaseQry, array_selecoes, lista_anotacoes_svg, &idMax, epsilon);
    }

    // >> GERA O ARQUIVO .DOT IMEDIATAMENTE APÓS PROCESSAR O .GEO <<
    char *fullNomeDot = NULL;
    trataArqDot(dirSaida, nomeBaseGeo, nomeBaseQry, &fullNomeDot);
    if (fullNomeDot) {
        printf("Gerando arquivo .dot: %s\n", fullNomeDot);
        if (!printDotSmuTreap(t, fullNomeDot)) {
            fprintf(stderr, "AVISO: Nao foi possivel criar o arquivo .dot.\n");
        }
    }

    // --- 5. Limpeza Final (Cleanup) ---
cleanup:
    printf("PROGRAMA FINALIZADO. Liberando toda a memoria...\n");

    // Libera a árvore SmuTreap e todo o seu conteúdo
    if (t) {
        killSmuTreap(t);
    }

    // Libera todas as listas de seleção que foram criadas
    for (int j = 0; j < 100; j++) {
        if (array_selecoes[j] != NULL) {
            destroiLista(array_selecoes[j], NULL);
        }
    }

    // Libera as strings de caminhos e parâmetros
    if (dirEntrada) free(dirEntrada);
    if (dirSaida) free(dirSaida);
    if (nomeGeo) free(nomeGeo);
    if (nomeBaseGeo) free(nomeBaseGeo);
    if (nomeBaseQry) free(nomeBaseQry);
    if (nomeQry) free(nomeQry);
    if (fullNomeGeo) free(fullNomeGeo);
    if (fullNomeQry) free(fullNomeQry);
    if (fullNomeDot) free(fullNomeDot);
    if (prioMax) free(prioMax);
    if (hc) free(hc);
    if (promoRate) free(promoRate);

    return 0;
}