#include "SmuTreap.h"

#include <stdio.h>
#include <stdlib.h>

#define TIPO_CIRCULO 1
#define TIPO_RETANGULO 2
#define TIPO_TEXTO 3
#define TIPO_LINHA 4

typedef struct{
    double x, y, w, h;
}BB;

typedef struct node_internal{
    //ancoras do nó
    double x;
    double y;

    //heap
    int priority;
    
    //void pointers
    Info info;                      //struct da forma que o nó armazena
    DescritorTipoInfo descritor;    //inteiro que define qual é a forma

    //Para gerar a árvore
    struct node_internal *dad;
    struct node_internal *left;
    struct node_internal *right;

    //Para propriedade Smu
    int hitCountCounter;

    //para armazenar bounding box
    BB node;
    BB subTree;
} node_internal;

typedef struct{
    //Dado a logica da àrvore
    node_internal *root;

    //Parâmetros de config.
    int hitCountConfig;
    int prioMax;
    double promoRateConfig;
    double epsilon;
} SmuTreap_internal;

/*DAR FREE EM:
* 
* newSmu - sempre que uma nova SmuTreap for alocada com sucesso.
* 
*/



//BUSCAR RESOLVER ERRO DA FALTA DO PARÂMETRO priorityMax.

// É esperado que newSmuTreap receba todos os parâmetros corretamente.
SmuTreap newSmuTreap(int hitCount, double promotionRate, double epsilon){
    SmuTreap_internal *newSmu = (SmuTreap_internal*) malloc(sizeof(SmuTreap_internal));
    if (!newSmu){
        printf("(newSmuTreap) Erro: falha ao alocar memoria.");
        return NULL;
    }

    newSmu->root = NULL;
    newSmu->hitCountConfig = hitCount;
    //newSmu->prioMax = ??;
    newSmu->promoRateConfig = promotionRate;
    newSmu->epsilon = epsilon;

    return newSmu;
}

Node newNode(double x, double y, int priorityMax, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb){
    node_internal *newNode = (node_internal*) malloc(sizeof(node_internal));
    if(!newNode) {printf("(newNode) Erro: falha ao alocar memoria."); return NULL;}

    newNode->x = x;
    newNode->y = y;

    /*Lembre-se de incluir <stdlib.h> para rand() e de inicializar a semente com srand(time(NULL)) em algum ponto no
    início do programa, preferencialmente no main, para não gerar sempre a mesma sequência de prioridades).*/
    
    newNode->priority = rand() % (priorityMax + 1); //verificar isso.
    newNode->info = i;
    newNode->descritor = d;
    newNode->dad = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    newNode->hitCountCounter = 0;

    //o que fazer com fCalcBb?

}

Node insertSmuT(SmuTreap t, double x, double y, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb){

}