#include "SmuTreap.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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
    BB BBinfo;
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
    newSmu->prioMax = priorityMax;
    newSmu->promoRateConfig = promotionRate;
    newSmu->epsilon = epsilon;

    return newSmu;
}

//aux
Node newNode(double x, double y, int priorityMax, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb){
    node_internal *newNode = (node_internal*) malloc(sizeof(node_internal));
    if (!newNode) {
        perror("(newNode) Erro: falha ao alocar memoria para no"); // Adicionado perror para mais info
        return NULL;
    }

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
        fCalcBb(d, i, &(newNode->BBinfo.x), &(newNode->BBinfo.y), &(newNode->BBinfo.w), &(newNode->BBinfo.h));
    } else {
        fprintf(stderr, "(newNode) Erro: fCalcBb aponta NULL. Bounding box não calculado para o novo nó.\n");
        newNode->BBinfo.w = -1.0; // Marcar como inválido
        newNode->BBinfo.h = -1.0;
    }

    if (newNode->BBinfo.w >= 0 && newNode->BBinfo.h >= 0) {
        newNode->BBsubTree = newNode->BBinfo; // Cópia de struct
    } else {
        newNode->BBsubTree.x = newNode->x;
        newNode->BBsubTree.y = newNode->y;
        newNode->BBsubTree.w = 0;
        newNode->BBsubTree.h = 0;
    }

    return newNode;
}

//--------------------------------------TALVEZ, PARTE DE boundingBox.h!------------------------------------------//
/**
 * @brief Calcula a união de dois bounding boxes.
 * O resultado é armazenado em bb_res.
 */
void uniaoBB(BB *bb_dest, BB *bb1, BB *bb2){
    // VERIFICACAO VALIDADE DE AMBOS OS BB.
    bool bb1_valido = bb1 && bb1->w >= 0 && bb1->h >= 0;
    bool bb2_valido = bb2 && bb2->w >= 0 && bb2->h >= 0;

    if (!bb1_valido) {
        if (bb2_valido) {
            *bb_dest = *bb2;
        } else { // Ambos inválidos
            bb_dest->w = -1.0; bb_dest->h = -1.0; // Marcar resultado como inválido
        }
        return;
    }
    if (!bb2_valido) { // bb1 é valido, bb2 é invalido
        *bb_dest = *bb1;
        return;
    }

    double min_x = fmin(bb1->x, bb2->x); // Ambos sao as coordenadas ancora da nova BB
    double min_y = fmin(bb1->y, bb2->y);
    double max_x = fmax(bb1->x + bb1->w, bb2->x + bb2->w);
    double max_y = fmax(bb1->y + bb1->h, bb2->y + bb2->h);
    
    bb_dest->x = min_x;
    bb_dest->y = min_y;
    bb_dest->w = max_x - min_x;
    bb_dest->h = max_y - min_y;

    //DEFINE A ANCORA DO NOVO BB COMO A PARTE ESQUERDA MAIS ALTA NA TELA(parte inferior em relacao ao y, que cresce para baixo).
}

/* 
* @brief Atualiza o bounding box da subárvore de um nó.
*
*  Esta função será chamada recursivamente de baixo para cima após qualquer modificação estrutural na 
* árvore (inserção, remoção, rotações). Ela assume que o bb_info do nó n está correto e que os 
* bb_subtree dos filhos de n (se existirem) já foram corretamente atualizados.
*/
void atualizaBB_subtree(node_internal *n){
    if (!n){
        fprintf(stderr, "(atualizaBB_subtree) Erro: no eh invalido (NULL)");
        return;
    }

    if (n->BBinfo.w >= 0 && n->BBinfo.h >= 0) {
        n->BBsubTree = n->BBinfo;
    } else {
        // Se BBinfo do nó 'n' for inválido, seu BBsubTree inicial
        // será apenas sua âncora (tratada como um ponto).
        n->BBsubTree.x = n->x;
        n->BBsubTree.y = n->y; // Âncora é canto inferior-esquerdo
        n->BBsubTree.w = 0;
        n->BBsubTree.h = 0;
    }

    if (n->left) {
        // A função uniao_bb deve ser capaz de lidar com um dos BBs sendo
        // o "BB de ponto" (se o filho esquerdo for uma folha e seu bb_info
        // não for válido, e seu bb_subtree for apenas sua âncora).
        // Ela também deve ser capaz de lidar se n->left->bb_subtree for inválido (w < 0).
            uniao_bb(&(n->BBsubTree), &(n->BBsubTree), &(n->left->BBsubTree));
    }
    if (n->right) {
        uniao_bb(&(n->BBsubTree), &(n->BBsubTree), &(n->right->BBsubTree));
    }


}

//------------------------------------------------------------------------------------------------------------------//

/* "Eu" viro filho direito do meu filho esquerdo, 
*   e os flhos direitos dele viram meus filhos esquerdos.
*
* @returns sucesso - No' do ANTIGO filho direito \n
*          erro - NULL
*/
void rotacionaDir(node_internal **rootRef){
    if(!rootRef){
        fprintf(stderr, "(rotacionaDir) Erro: no' invalido.");
        return;
    }
    node_internal *root = *rootRef;
    if (!(root->left)){
        fprintf(stderr, "(rotacionaDir) Erro: tentativa de acesso a NULL (nd->left = NULL).");
        return;
    }
    
    node_internal *leftSon = root->left;

    // "filhos direitos dele viram meus filhos esquerdos"
    root->left = root->left->right;

    // "'Eu' viro filho direito do meu filho esquerdo"
    leftSon->right = root;

    atualizaBB_subtree(root); //atualiza root primeiro, pois agora é filho.
    atualizaBB_subtree(leftSon); // Atualiza leftSon apenas depois que seus filhos estiverem corrigidos.

    /* Faz o root da função que chamou a rotação  
    * apontar para leftSon, sendo então atualizado
    * na proxima volta da recursão.
    */
    *rootRef = leftSon;



    // meu filho esquerdo vira meu pai, e eu viro pai dos filhos direitos dele

}

void rotacionaEsq(node_internal **rootRef){
    if(!rootRef){
        fprintf(stderr, "(rotacionaDir) Erro: no' invalido.");
        return;
    }
    node_internal *root = *rootRef;
    if (!(root->right)){
        fprintf(stderr, "(rotacionaDir) Erro: tentativa de acesso a NULL (nd->left = NULL).");
        return;
    }

    node_internal *rightSon = root->right;

    // "filhos esquerdos dele viram meus filhos direitos"
    root->right = root->right->left;

    // "'Eu' viro filho esquerdo do meu filho direito"
    rightSon->left = root;

    atualizaBB_subtree(root); //atualiza root primeiro, pois agora é filho.
    atualizaBB_subtree(rightSon); // Atualiza rightSon apenas depois que seus filhos estiverem corrigidos.

    /* Faz o root da função que chamou a rotação  
    * apontar para rightSon, sendo então atualizado
    * na proxima volta da recursão.
    */
    *rootRef = rightSon;


    // meu filho direito vira meu pai, e eu viro pai dos filhos esquerdos dele

}

//aux
node_internal *insertSmuT_aux(node_internal *root, node_internal *new, SmuTreap_internal *t){
    double epsilon_i = t->epsilon;
    if(!root){
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

    bool podeIrDireita = (new->x < root->x - epsilon_i) || (fabs(new->x - root->x) < epsilon_i && (new->y < root->y - epsilon_i));
    bool podeIrEsquerda = (new->x > root->x + epsilon_i) || (fabs(new->x - root->x) < epsilon_i && (new->y > root->y + epsilon_i));

    if (podeIrDireita){
        // Vai para a direita.
        root->right = insertSmuT_aux(root->right, new, t);

        if(root->right->priority > root->priority){
            rotacionaEsq(&root);
            root->left->dad = root;
        }
        root->right->dad = root;
    } else if (podeIrEsquerda){
        // Vai para a esquerda.
        root->left = insertSmuT_aux(root->left, new, t);
                
        if (root->left->priority > root->priority){
            rotacionaDir(&root);
            root->right->dad = root;
        }
        root->left->dad = root;
    } else{
        fprintf(stderr, "(insertSmuT_aux) Erro: no' ja' existente.");
        free(new);
        return root;
    }

    if(root) atualizaBB_subtree(root);


    return root;
}

Node insertSmuT(SmuTreap t, double x, double y, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb){
    if (!t){
        fprintf(stderr, "(insertSmuT) Erro: arvore invalida.");
        return NULL;
    }
    SmuTreap_internal *t_i = (SmuTreap_internal*)t;

    // A versao do Gemini se preocupa em como passar fCalcBb para ca. Eu nao entendi, mas tomar nota!

    int prioridade = rand() % (t_i->prioMax + 1);

    node_internal *new = (node_internal*)newNode(x, y, prioridade, i, d, fCalcBb);
    if (!new) return NULL;
    t_i->root = insertSmuT_aux(t_i->root, new, t);
    
    if (t_i->root == new || (new->dad != NULL) ) {
         // Se o novo nó é a raiz, ou se tem um pai, ele foi inserido.
        return (Node)new;
    } else return NULL;
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