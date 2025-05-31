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
    BB BBnode;
    BB BBsubTree;
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
* newNode - sempre que um novo no'foi alocado com sucesso.
*/



//BUSCAR RESOLVER ERRO DA FALTA DO PARÂMETRO priorityMax.

// É esperado que newSmuTreap receba todos os parâmetros corretamente.
SmuTreap newSmuTreap(int hitCount, int priorityMax, double promotionRate, double epsilon){
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

//aux
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

    //calcula o BB individual.
    if(fCalcBb){
        fCalcBb(d, i, &(newNode->BBnode.x), &(newNode->BBnode.y), (&newNode->BBnode.w), &(newNode->BBnode.h));
    } else fprintf(stderr, "(newNode) Erro: fCalcBb aponta NULL. Bounding box não calculado para o novo nó.\n");
    
    return newNode;
}

//aux
node_internal *insertSmuT_aux(node_internal *root, double x, double y, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb, SmuTreap_internal *t){
    double epsilon_i = t->epsilon;
    if(!root){
        node_internal *new = (node_internal*)newNode(x, y, t->prioMax, i, d, fCalcBb);
        if(!new){
            fprintf(stderr, "(insertSmuT_aux) Erro: falha ao alocar memoria para no'.");
            exit (1);
        }

        return new;
    }

    /* Casos em que ele deve ir para a esquerda:
    *  
    *  1. Quando x < rooti->x - epsilon
    *  2. Quando rooti->x - epsilon <= x <= rooti->x + epsilon, mas y < rooti->y
    * 
    * 
    *  Casos em que ele deve ir para a direita™
    * 
    *  1. Quando x > rooti->x + epsilon
    *  2. Quando Quando rooti->x - epsilon <= x <= rooti->x + epsilon, mas y > rooti->y
    */
    if(x < root->x - epsilon_i){ // x e' menor do que o x minimo para que seja considerado como igual.
        // Vai para a esquerda.
        root->left = insertSmuT_aux(root->left, x, y, i, d, fCalcBb, t);
        root->left->dad = root;
    } else{
        if (x > root->x + epsilon_i){ // x e' maior do que o x minimo para que seja considerado como igual.
            // Vai para a direita.
            root->right = insertSmuT_aux(root->right, x, y, i, d, fCalcBb, t);
            root->right->dad = root;
        } else{                   // Se chegou aqui, |px - noAtual_x| <= epsilon. Consideramos x = rooti->x.
            if (y < root->y - epsilon_i){ // Mesma logica dos demais.
                // Vai para a esquerda.
                root->left = insertSmuT_aux(root->left, x, y, i, d, fCalcBb, t);
                root->left->dad = root;
            } else{
                if (y > root->y + epsilon_i){ // Mesma logica dos demais.
                    // Vai para a direita.
                    root->right = insertSmuT_aux(root->right, x, y, i, d, fCalcBb, t);
                    root->right->dad = root;
                } else{
                    fprintf(stderr, "(insertSmuT_aux) Erro: no' ja' existente.");
                    exit (1);
                }
            }
        }
    }





}

Node insertSmuT(SmuTreap t, double x, double y, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb){

}

Node getNodeSmuT(SmuTreap t, double x, double y);
/*
 * Retorna o no' cuja ancora seja o ponto (x,y), admitida a discrepancia
 * epsilon definida na criacao da arvore.
 * Retorna NULL caso nao tenha encontrado o no'.
 */

DescritorTipoInfo getTypeInfoSmuT(SmuTreap t, Node n);
/* 
 * Retorna o tipo da informacao associada ao no' n 
*/

void promoteNodeSmuT(SmuTreap t, Node n, double promotionRate);
/*
 * Aumenta a prioridade do no' n pelo fator promotionRate.
 */

void removeNoSmuT(SmuTreap t, Node n);
/*
 * Remove o no' n da arvore. O no' n deve ser um no' valido.
 */

Info getInfoSmuT(SmuTreap t, Node n);
/* 
 * Retorna a informacao associada ao no' n 
 */

Info getBoundingBoxSmuT(SmuTreap t, Node n, double *x, double *y, double *w, double *h);
/* 
 * Retorna o bounding box associado ao no' n 
 */


bool getNodesDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2, Lista L);
/* Insere na lista L os nos (Node) da arvore t cujas ancoras estao dentro da regiao 
   delimitada pelos pontos (x1,y1) e (x2,y2).
   Retorna falso, caso nao existam nos dentro da regiao; verdadeiro, caso contrario.
 */

bool getInfosDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2,
				 FdentroDeRegiao f, Lista L);
/* Insere na lista L os nos cujas respectivas informacoes associadas estao inteiramente dentro da regiao
   delimitada pelos pontos (x1,y1) e (x2,y2). A funcao f e' usada para determinar se uma
   informacao armazenada na arvore esta' dentro da regiao.
   Retorna falso caso nao existam informacoes internas; verdadeiro, caso contrario.
 */

bool getInfosAtingidoPontoSmuT(SmuTreap t, double x, double y, FpontoInternoAInfo f, Lista L);
/* Insere na lista L  os nos para os quais o ponto (x,y) possa ser considerado
  interno 'as  informacoes associadas ao no'. A funcao f e' invocada para determinar
  se o ponto (x,y) e' interno a uma informacao especifica.
  Retorna falso caso nao existam informacoes internas; verdadeiro, caso contrario.
 */

void visitaProfundidadeSmuT(SmuTreap t, FvisitaNo f, void *aux);
/* Percorre a arvore em profundidade. Invoca a funcao f em cada no visitado.
   O apontador aux e' parametro em cada invocacao de f; assim alguns
   dados podem ser compartilhados entre as diversas invocacoes de f.
 */

void visitaLarguraSmuT(SmuTreap t, FvisitaNo f, void *aux);
/* Similar a visitaProfundidadeSmuT, porem, faz o percurso em largura.
 */

Node procuraNoSmuT(SmuTreap t, FsearchNo f, void *aux);
/* Procura o no' da arvore que contenha um dado especifico.
   Visita cada no' da arvore e invoca a funcao f. A funcao f
   retornara' verdadeiro se o no' contem o dado procurado.
   Neste caso, retorna o no' encontrado. Caso a busca falhe,
   retorna NULL.
 */

bool printDotSmuTreap(SmuTreap t, char *fn);
/* Gera representacao da arvore no arquivo fn, usando a Dot Language
   (ver https://graphviz.org/). Retorna falso, caso o arquivo nao possa
   ser criado (para escrita); true, caso contrario)
*/

void killSmuTreap(SmuTreap t);
/* Libera a memoria usada pela arvore t.
 */