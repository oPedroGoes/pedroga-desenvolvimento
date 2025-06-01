#include "SmuTreap.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"

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
    FCalculaBoundingBox fCalcBB;
} SmuTreap_internal;

/*DAR FREE EM:
* 
* newSmu - sempre que uma nova SmuTreap for alocada com sucesso.
* newNode - sempre que um novo no'foi alocado com sucesso.
*/

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
    newSmu->fCalcBB =   NULL;

    return newSmu;
}

//aux
Node newNode(double x, double y, int priority, Info i, DescritorTipoInfo d, FCalculaBoundingBox fCalcBb){
    node_internal *newNode = (node_internal*) malloc(sizeof(node_internal));
    if (!newNode) {
        perror("(newNode) Erro: falha ao alocar memoria para no"); // Adicionado perror para mais info
        return NULL;
    }

    newNode->x = x;
    newNode->y = y;


    if (priority <= 0){
        fprintf(stderr, "(newNode) Erro: prioridade invalida");
        return NULL;
    }
    /*Lembre-se de incluir <stdlib.h> para rand() e de inicializar a semente com srand(time(NULL)) em algum ponto no
    início do programa, preferencialmente no main, para não gerar sempre a mesma sequência de prioridades).*/
    newNode->priority = priority; //verificar isso.

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
    node_internal *grandpa = root->dad;

    leftSon->dad = grandpa;

    // "filhos direitos dele viram meus filhos esquerdos"
    root->left = root->left->right;

    if (leftSon->right){ // Se leftSin->right não for NULL.
        // "e eu viro pai dos filhos direitos dele"
        leftSon->right->dad = root;
    }


    // "'Eu' viro filho direito do meu filho esquerdo"
    leftSon->right = root;
    // "meu filho esquerdo vira meu pai"
    root->dad = leftSon;


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
        fprintf(stderr, "(rotacionaEsq) Erro: no' invalido.");
        return;
    }
    node_internal *root = *rootRef;
    if (!(root->right)){
        fprintf(stderr, "(rotacionaEsq) Erro: tentativa de acesso a NULL (nd->left = NULL).");
        return;
    }

    node_internal *rightSon = root->right;
    node_internal *grandpa = root->dad;

    rightSon->dad = grandpa;
    // "filhos esquerdos dele viram meus filhos direitos"
    root->right = root->right->left;

    if (rightSon->left){ // Se rightSon->right não for NULL.
        // "e eu viro pai dos filhos esquerdos dele"
        rightSon->left->dad = root;
    }

    // "'Eu' viro filho esquerdo do meu filho direito"
    rightSon->left = root;
    // "meu filho direito vira meu pai"
    root->dad = rightSon;

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

    bool podeIrDireita = (new->x > root->x + epsilon_i) || (fabs(new->x - root->x) < epsilon_i && (new->y > root->y + epsilon_i));
    bool podeIrEsquerda = (new->x < root->x - epsilon_i) || (fabs(new->x - root->x) < epsilon_i && (new->y < root->y - epsilon_i));

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
        if (new) free(new);
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

    // Registra ou atualiza a função de cálculo de BB para a árvore
    if (fCalcBb){
        if (t_i->fCalcBB == NULL){
            t_i->fCalcBB = fCalcBb;
        } else if (t_i->fCalcBB != fCalcBb){ 
            fprintf(stderr, "Aviso (insertSmuT): FCalculaBoundingBox esta sendo alterada para a arvore.\n");
            t_i->fCalcBB = fCalcBb;
        }
    }

    int prioridade = rand() % (t_i->prioMax + 1);

    node_internal *new = (node_internal*)newNode(x, y, prioridade, i, d, fCalcBb);
    if (!new) return NULL;
    t_i->root = insertSmuT_aux(t_i->root, new, t);
    
    if (t_i->root == new || (new->dad != NULL) ) {
         // Se o novo nó é a raiz, ou se tem um pai, ele foi inserido.
        return (Node)new;
    } else return NULL;
}

//aux
node_internal *getNodeSmuT_aux(SmuTreap_internal *t, node_internal* root, node_internal **resultRef, double x, double y){
    if(root == NULL){
        fprintf(stderr, "(fetNodeSmuT_aux) Erro: no'nao encontrado.");
        exit (1); // Se retornasse NULL, a arvore seria destruida.
    }

    bool xDentroDeEpsulon = fabs(x - root->x) < t->epsilon;
    bool yDentroDeEpsulon = fabs(y - root->y) < t->epsilon;
    bool podeIrDireita = (x > root->x + t->epsilon) || (fabs(x - root->x) < t->epsilon && (y > root->y + t->epsilon));
    bool podeIrEsquerda = (x < root->x - t->epsilon) || (fabs(x - root->x) < t->epsilon && (y < root->y - t->epsilon));


    if (xDentroDeEpsulon && yDentroDeEpsulon){ // Encontrou!
        root->hitCountCounter++;

        if(root->hitCountCounter >= t->hitCountConfig){
            promoteNodeSmuT(t, (Node)root, t->promoRateConfig);
        }
        *resultRef = root;
        return root;
    } else 

    if(podeIrDireita){
        root->right = getNodeSmuT_aux(t, root->right, resultRef, x, y);
    } else{
        root->left = getNodeSmuT_aux(t, root->left, resultRef, x, y);
    }
}

Node getNodeSmuT(SmuTreap t, double x, double y){
    if(!t){
        fprintf(stderr, "(getNodeSmuT) Erro: ponteiro para arvore invalido.");
        return NULL;
    }

    SmuTreap_internal *t_i = (SmuTreap_internal*)t;
    node_internal *result;

    t_i->root = getNodeSmuT_aux(t_i, t_i->root, &result, x, y);

    return result;
}

DescritorTipoInfo getTypeInfoSmuT(SmuTreap t, Node n);
/* 
 * Retorna o tipo da informacao associada ao no' n 
*/

void promoteNodeSmuT(SmuTreap t, Node n, double promotionRate){

    if (!t || !n || promotionRate <= 0){
        fprintf(stderr, "(promoteNodeSmuT) Erro: parametros invalidos.");
        return;
    } 
    if (promotionRate <= 0.0) {
         fprintf(stderr, "(promoteNodeSmuT) Erro: promotionRate deve ser > 0.0.\n");
        return;
    }

    SmuTreap_internal *treap = (SmuTreap_internal *)t;
    node_internal *ni = (node_internal *)n;


    int antiga_prioridade = ni->priority;
    int nova_prioridade = (int)(ni->priority * promotionRate);

    if (promotionRate > 1.0 && nova_prioridade == ni->priority){
        nova_prioridade++; // Se no cast para int o valor permaneceu o mesmo, arredonda para cima.
    } else if (promotionRate < 1.0 && nova_prioridade == ni->priority && nova_prioridade > 0) {
        nova_prioridade--; //Se no cast para int o valor permaneceu o mesmo, arredonda para baixo.
    }

    // Tetos
    if (nova_prioridade > treap->prioMax) nova_prioridade = treap->prioMax;
    if (nova_prioridade < 0) nova_prioridade = 0;

    // Salvar principais dados.
    double x_copia = ni->x;
    double y_copia = ni->y;
    Info info_copia = ni->info;
    DescritorTipoInfo descritor_copia = ni->descritor;
    int hit_copia = ni->hitCountCounter;
    BB BBinfo_copia = ni->BBinfo;
    BB BBsubTree_copia = ni->BBsubTree;

    removeNoSmuT_SemLiberarInfo(t, n); // 'n' (e 'ni_original') agora é um ponteiro para memória liberada!

    // 4. Criar um novo nó com os dados salvos e a NOVA prioridade.
    //    Precisamos de FCalculaBoundingBox para newNode.
    //    Se não tivermos fCalcBb, passamos NULL, e newNode lidará com isso (marcando BBinfo como inválido).
    //    Isso é problemático. Idealmente, a treap armazenaria fCalcBb.
    FCalculaBoundingBox func_calc_bb_para_novo_no = NULL; // SUBSTITUA PELO MECANISMO CORRETO PARA OBTER fCalcBb
                                                       // Ex: Se SmuTreapInternal tivesse um campo para isso:
                                                       // func_calc_bb_para_novo_no = treap->func_calc_bb_armazenada;

    // Criar novo no newNode: node_internal *no_promovido = ...
    // Verificar se deu bom no return

    // Reatribuir hitCountCounter para o novo no.

    //Reinserir o nó "promovido" na árvore.




    ni->priority = nova_prioridade;



    node_internal *flag = removeNoSmuT_aux(treap, treap->root, ni); // Remove o no' desejado e guarda o resultado em uma flag para comparacao. 

    if (flag){
        insertSmuT_aux(treap->root, aux, t);
        
    } else {
        // Algo deu errado na remoção, restaurar a prioridade original?
        // Ou tentar reinserir mesmo assim? Para evitar perda de dados,
        // vamos tentar reinserir, mas isso pode levar a duplicatas se a remoção falhou e o nó ainda existe.
        // É mais seguro se a remoção for garantida.
        // Se o nó 'n' veio de getNodeSmuT, ele existe.
        fprintf(stderr, "Aviso (promoteNodeSmuT): falha ao remover no para promocao. O no pode nao ser reposicionado corretamente ou dados podem ser perdidos.\n");
        // Para evitar perda, se remove_recursive falhar, poderíamos apenas deixar a prioridade atualizada
        // e esperar que futuras operações corrijam a ordem do heap, ou ter uma função "heapify_up".
        // A abordagem de remover e reinserir é mais simples se a remoção for robusta.
    }
}

//aux
void killNode(node_internal *n){
    if (!n || !n->info){
        fprintf(stderr, "(killNode) Erro: parametros invalidos.");
        return ;
    }

    switch (n->descritor)
    {
    case TIPO_CIRCULO:
        kill_circ((CIRCLE) n->info);
        free(n);
        break;
    
    case TIPO_RETANGULO:
        kill_rectangle((RECTANGLE) n->info);
        free(n);
        break;

    case TIPO_TEXTO:
        kill_texto((TEXTO) n->info);
        free(n);
        break;

    default:
        kill_linha((LINHA) n->info);
        free(n);
        break;
    }
}

//aux
node_internal* removeNoSmuT_SemLiberarInfo_aux(SmuTreap_internal *t, node_internal *root, node_internal *n){
    if (!root){
        printf("(removeNoSmuT_aux) Erro: no nao encontrado.");
        return NULL;
    }
    double epsilon_i = t->epsilon;

    bool podeIrDireita = (n->x > root->x + epsilon_i) || (fabs(n->x - root->x) < epsilon_i && (n->y > root->y + epsilon_i));
    bool podeIrEsquerda = (n->x < root->x - epsilon_i) || (fabs(n->x - root->x) < epsilon_i && (n->y < root->y - epsilon_i));

    if (podeIrDireita){
        root->right = removeNoSmuT_aux(t, root->right, n);
    } else if(podeIrEsquerda){
        root->left = removeNoSmuT_aux(t, root->left, n);
    } else{ // Encontra o no' que se quer remover
        // Aqui, ao que parece, a recomendacao é que se use uma flag bool que retorna true se o no'foi achado.

        // Caso 1: no' folha.
        if (!root->right && !root->left){
            free(root);
            return NULL;
        }

        // Caso 2: no' com apenas um filho.
        else if(root->right == NULL){
            node_internal *aux = root->left;
            free(root);
            return aux;
        } else if(root->left == NULL){
            node_internal *aux = root->right;
            free(root);
            return aux;
        }

        // Caso 3: no' com dois filhos
        // Rotaciona o nó para baixo até que se torne uma folha ou tenha um filho,
        // mantendo a propriedade de heap.
        else{
            if(root->right->priority > root->left->priority){
                rotacionaEsq(&root);
                root->right = removeNoSmuT_aux(t, root->right, n);
            } else{
                rotacionaDir(&root);
                root->left = removeNoSmuT_aux(t, root->left, n);
            }
        }
    }

    if(root){
        atualizaBB_subtree(root);
    }

    return root;
}
void removeNoSmuT_SemLiberarInfo(SmuTreap t, Node n){
    if (!t || !n){
        fprintf(stderr, "(removeNoSmuT) Erro: parametros invalidos.\n");
        return;
    }
    
    SmuTreap_internal *ti = (SmuTreap_internal*)t;
    node_internal *ni = (node_internal*)n;

    ti->root = removeNoSmuT_aux(ti, ti->root, ni);
}

//aux
node_internal* removeNoSmuT_aux(SmuTreap_internal *t, node_internal *root, node_internal *n){
    if (!root){
        printf("(removeNoSmuT_aux) Erro: no nao encontrado.");
        exit (1); // Se retornasse NULL, a arvore seria destruida.
    }
    double epsilon_i = t->epsilon;

    bool podeIrDireita = (n->x > root->x + epsilon_i) || (fabs(n->x - root->x) < epsilon_i && (n->y > root->y + epsilon_i));
    bool podeIrEsquerda = (n->x < root->x - epsilon_i) || (fabs(n->x - root->x) < epsilon_i && (n->y < root->y - epsilon_i));

    if (podeIrDireita){
        root->right = removeNoSmuT_aux(t, root->right, n);
    } else if(podeIrEsquerda){
        root->left = removeNoSmuT_aux(t, root->left, n);
    } else{ // Encontra o no' que se quer remover
        // Aqui, ao que parece, a recomendacao é que se use uma flag bool que retorna true se o no'foi achado.

        // Caso 1: no' folha.
        if (!root->right && !root->left){
            killNode(root);
            return NULL;
        }

        // Caso 2: no' com apenas um filho.
        else if(root->right == NULL){
            node_internal *aux = root->left;
            killNode(root);
            return aux;
        } else if(root->left == NULL){
            node_internal *aux = root->right;
            killNode(root);
            return aux;
        }

        // Caso 3: no' com dois filhos
        // Rotaciona o nó para baixo até que se torne uma folha ou tenha um filho,
        // mantendo a propriedade de heap.
        else{
            if(root->right->priority > root->left->priority){
                rotacionaEsq(&root);
                root->right = removeNoSmuT_aux(t, root->right, n);
            } else{
                rotacionaDir(&root);
                root->left = removeNoSmuT_aux(t, root->left, n);
            }
        }
    }

    if(root){
        atualizaBB_subtree(root);
    }

    return root;
}
void* removeNoSmuT(SmuTreap t, Node n){
    if (!t || !n){
        fprintf(stderr, "(removeNoSmuT) Erro: parametros invalidos.\n");
        return;
    }
    
    SmuTreap_internal *ti = (SmuTreap_internal*)t;
    node_internal *ni = (node_internal*)n;

    ti->root = removeNoSmuT_aux(ti, ti->root, ni);
}

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