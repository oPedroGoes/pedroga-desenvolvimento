#include "SmuTreap.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"
#include "Lista.h"
#include "geometria.h"
#include "formas.h"

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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


    if (priority < 0){
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
* BBsubTree dos filhos de n (se existirem) já foram corretamente atualizados.
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
        // não for válido, e seu BBsubTree for apenas sua âncora).
        // Ela também deve ser capaz de lidar se n->left->BBsubTree for inválido (w < 0).
            uniaoBB(&(n->BBsubTree), &(n->BBsubTree), &(n->left->BBsubTree));
    }
    if (n->right) {
        uniaoBB(&(n->BBsubTree), &(n->BBsubTree), &(n->right->BBsubTree));
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
void killInfo(Info info, DescritorTipoInfo descritor){
    if (!info || !descritor){
        fprintf(stderr, "(killInfo) Erro: parametros invalidos.");
        return ;
    }

    switch (descritor)
    {
    case TIPO_CIRCULO:
        kill_circ((CIRCLE) info);
        break;
    
    case TIPO_RETANGULO:
        kill_rectangle((RECTANGLE) info);
        break;

    case TIPO_TEXTO:
        kill_texto((TEXTO) info);
        break;

    default:
        kill_linha((LINHA) info);
        break;
    }
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

        // Define o pai do novo nó.
        if (root->right) root->right->dad = root;

        if(root->right->priority > root->priority){
            rotacionaEsq(&root);
            root->left->dad = root;
        }
    } else if (podeIrEsquerda){
        // Vai para a esquerda.
        root->left = insertSmuT_aux(root->left, new, t);

        // Define o pai do novo nó.
        if (root->left) root->left->dad = root;
                
        if (root->left->priority > root->priority){
            rotacionaDir(&root);
            root->right->dad = root;
        }
    } else{
        // Caso as coordenadas forem iguais:
        
        int id_new = get_idF(new->info, new->descritor);
        int id_root = get_idF(root->info, root->descritor);

        // Usa o id da forma como critério de desempate.
        if (id_new < id_root) {
            root->left = insertSmuT_aux(root->left, new, t);

            if (root->left) root->left->dad = root;

            // Após a inserção recursiva, a propriedade do heap deve ser verificada.
            if (root->left && root->left->priority > root->priority){
                rotacionaDir(&root);
                if(root->right) root->right->dad = root; // Atualiza o pai do nó que desceu
            }
        } else if(id_new > id_root){
            root->right = insertSmuT_aux(root->right, new, t);
             
            if (root->right) root->right->dad = root;

            // Após a inserção recursiva, a propriedade do heap deve ser verificada.
            if (root->right && root->right->priority > root->priority){
                rotacionaEsq(&root);
                if(root->left) root->left->dad = root; // Atualiza o pai do nó que desceu
            }
        } else{
            // Um nó com a mesma âncora E o mesmo ID já existe
            fprintf(stderr, "ERRO (insertSmuT): Tentativa de inserir no com ID %d e ancora (%.2f, %.2f) duplicados.\n", id_new, new->x, new->y);
            
            killInfo(new->info, new->descritor);
            free(new);
            
            exit(1);
            return root;
        }
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
            fCalcBb = t_i->fCalcBB;
        }
    }
    

    int prioridade = rand() % (t_i->prioMax + 1);
    //printf("prioridade = %d\n", prioridade);

    node_internal *new = (node_internal*)newNode(x, y, prioridade, i, d, fCalcBb);
    if (!new) return NULL;
    t_i->root = insertSmuT_aux(t_i->root, new, t);
    
    if (t_i->root == new || (new->dad != NULL) ) {
         // Se o novo nó é a raiz, ou se tem um pai, ele foi inserido.
        return (Node)new;
    } else return NULL;
}

//aux
node_internal *getNodeSmuT_aux_original(SmuTreap_internal *t, node_internal* root, node_internal **resultRef, double x, double y){
    if(root == NULL){
        fprintf(stderr, "(getNodeSmuT_aux) Erro: no' nao encontrado. Encerrando programa...\n");
        exit (1); // Se retornasse NULL, a arvore seria destruida.
    }

    bool xDentroDeEpsulon = fabs(x - root->x) < t->epsilon;
    bool yDentroDeEpsulon = fabs(y - root->y) < t->epsilon;
    bool podeIrDireita = (x > (root->x + t->epsilon)) || (fabs(x - root->x) < t->epsilon && (y > (root->y + t->epsilon)));
    DescritorTipoInfo d = root->descritor;
    Info forma = root->info;

    if(d == TIPO_LINHA){ 
        double x1l = get_X1L((LINHA)forma);
        double y1l = get_Y1L((LINHA)forma);
        double x2l = get_X2L((LINHA)forma);
        double y2l = get_Y2L((LINHA)forma);

        bool x1lDentroDeEpsulon = fabs(x1l - root->x) < t->epsilon;
        bool y1lDentroDeEpsulon = fabs(y1l - root->y) < t->epsilon;
        bool x2lDentroDeEpsulon = fabs(x2l - root->x) < t->epsilon;
        bool y2lDentroDeEpsulon = fabs(y2l - root->y) < t->epsilon;

        if((x1lDentroDeEpsulon && y1lDentroDeEpsulon) || (x2lDentroDeEpsulon && y2lDentroDeEpsulon)){ //Encontrou, e e' linha"
            root->hitCountCounter++;

            if(root->hitCountCounter >= t->hitCountConfig){
                promoteNodeSmuT(t, (Node)root, t->promoRateConfig);
            }
            printf("(getNodeSmuT_aux) No LINHA encontrado!\n");
            printf("\nDEBUG DEPOIS (getNodeSmuT) coord_x = %lf, coord_y = %lf\n", root->x, root->y);
            *resultRef = root;
            return root;
        }
    }else {

        if (xDentroDeEpsulon && yDentroDeEpsulon){ // Encontrou!
            root->hitCountCounter++;

            if(root->hitCountCounter >= t->hitCountConfig){
                promoteNodeSmuT(t, (Node)root, t->promoRateConfig);
            }
            printf("(getNodeSmuT_aux) No qualquer encontrado!\n");
            *resultRef = root;
            printf("\nDEBUG DEPOIS (getNodeSmuT) coord_x = %lf, coord_y = %lf\n", (*resultRef)->x, (*resultRef)->y);
            return root;
        }
    }

    if(podeIrDireita){
        root->right = getNodeSmuT_aux_original(t, root->right, resultRef, x, y);
    } else{
        root->left = getNodeSmuT_aux_original(t, root->left, resultRef, x, y);
    }

    printf("(getNodeSmuT_aux) No NAO FOI encontrado!\n");
    return root;
}

Node getNodeSmuT_original(SmuTreap t, double x, double y){
    if(!t){
        fprintf(stderr, "(getNodeSmuT) Erro: ponteiro para arvore invalido.");
        return NULL;
    }

    SmuTreap_internal *t_i = (SmuTreap_internal*)t;
    node_internal *result = NULL;

    t_i->root = getNodeSmuT_aux_original(t_i, t_i->root, &result, x, y);
    //ENCONTRADO O ERRO: COORDENADA DO NO ENCONTRADO MUDA EM getNodeSmuT_aux;
    //printf("\nDEBUG (getNodeSmuT) coord_x = %lf, coord_y = %lf\n", result->x, result->y);
    return result;
}

Node recoverNodePostPromotion(SmuTreap t, double x, double y){ // Recovery do no perdido.
    if (!t) {
        fprintf(stderr, "(getNodeSmuT) Erro: ponteiro para arvore invalido.\n");
        return NULL;
    }

    SmuTreap_internal *t_i = (SmuTreap_internal *)t;
    node_internal *current = t_i->root;
    node_internal *found_node = NULL;


    while (current != NULL) {
        bool xDentroDeEpsilon = fabs(x - current->x) < t_i->epsilon;
        bool yDentroDeEpsilon = fabs(y - current->y) < t_i->epsilon;

        if (xDentroDeEpsilon && yDentroDeEpsilon) {
            found_node = current;
            return current; // Encontrou o no'
        }

        bool podeIrDireita = (x > (current->x + t_i->epsilon)) || (xDentroDeEpsilon && (y > (current->y + t_i->epsilon)));
        current = podeIrDireita ? current->right : current->left;
    }
    // Não encontrou o no'
    return NULL;
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
        root->right = removeNoSmuT_SemLiberarInfo_aux(t, root->right, n);
    } else if(podeIrEsquerda){
        root->left = removeNoSmuT_SemLiberarInfo_aux(t, root->left, n);
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
                root->right = removeNoSmuT_SemLiberarInfo_aux(t, root->right, n);
            } else{
                rotacionaDir(&root);
                root->left = removeNoSmuT_SemLiberarInfo_aux(t, root->left, n);
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

    ti->root = removeNoSmuT_SemLiberarInfo_aux(ti, ti->root, ni);
}


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

    removeNoSmuT_SemLiberarInfo(t, n); // 'n' (e 'ni_original') agora é um ponteiro para memória liberada!

    node_internal *zombie_node = (Node)newNode(x_copia, y_copia, nova_prioridade, info_copia, descritor_copia, treap->fCalcBB);
    if (!zombie_node){
        fprintf(stderr, "(promoteNodeSmuT) Erro: falha ao criar novo no'");
        killInfo(info_copia, descritor_copia);
        return;
    }

    zombie_node->hitCountCounter = hit_copia;

    treap->root = insertSmuT_aux(treap->root, zombie_node, treap);
}

Node getNodeSmuT(SmuTreap t, double x, double y) {
    if (!t) {
        fprintf(stderr, "(getNodeSmuT) Erro: ponteiro para arvore invalido.\n");
        return NULL;
    }

    SmuTreap_internal *t_i = (SmuTreap_internal *)t;
    node_internal *current = t_i->root;
    node_internal *found_node = NULL;


    while (current != NULL) {
        bool xDentroDeEpsilon = fabs(x - current->x) < t_i->epsilon;
        bool yDentroDeEpsilon = fabs(y - current->y) < t_i->epsilon;

        if (xDentroDeEpsilon && yDentroDeEpsilon) {
            found_node = current;
            break; // Nó encontrado, sai do laço.
        }

        bool podeIrDireita = (x > (current->x + t_i->epsilon)) || (xDentroDeEpsilon && (y > (current->y + t_i->epsilon)));
        current = podeIrDireita ? current->right : current->left;
    }

    if (found_node){
        found_node->hitCountCounter++;

        if (found_node->hitCountCounter >= t_i->hitCountConfig) {
            found_node->hitCountCounter = 0;
            promoteNodeSmuT(t, (Node)found_node, t_i->promoRateConfig);

            return recoverNodePostPromotion(t, x, y);
        }
        return (Node)found_node; // Retorna o nó encontrado.
    }

    fprintf(stderr, "(getNodeSmuT) Erro: no' nao encontrado.");
    return NULL;
}

DescritorTipoInfo getTypeInfoSmuT(SmuTreap t, Node n){
    if (!n){
        fprintf(stderr, "(getTypeInfoSmuT) Erro: parametros invalidos");
        return 0; // Ou um valor de erro/inválido
    }
    node_internal *ni = (node_internal*)n;
    return ni->descritor;
}

//aux
void  killNode(node_internal *n){
    if (!n || !n->info){
        fprintf(stderr, "(killNode) Erro: parametros invalidos.");
        return ;
    }

    killF(n->info, n->descritor);
    free(n);
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
void removeNoSmuT(SmuTreap t, Node n){
    if (!t || !n){
        fprintf(stderr, "(removeNoSmuT) Erro: parametros invalidos.\n");
        return;
    }
    
    SmuTreap_internal *ti = (SmuTreap_internal*)t;
    node_internal *ni = (node_internal*)n;

    ti->root = removeNoSmuT_aux(ti, ti->root, ni);
}

Info getInfoSmuT(SmuTreap t, Node n){
    if(!n){
        fprintf(stderr, "(getInfoSmuT) Erro: parametros invalidos");
        return NULL;
    }
    node_internal *ni = (node_internal*)n;
    return ni->info;
}

Info getBoundingBoxSmuT(SmuTreap t, Node n, double *x, double *y, double *w, double *h){
    if(!n){
        fprintf(stderr, "(getBoundingBoxSmuT) Erro: parametro invalido");
        return NULL;
    }

    node_internal *ni = (node_internal*)n;
    if(ni->BBsubTree.h < 0 || ni->BBsubTree.w < 0){
        fprintf(stderr, "(getBoundingBoxSmuT) Erro: Bounding Box invalido (h ou w < 1)");
        return NULL;
    }

    *x = ni->BBsubTree.x;
    *y = ni->BBsubTree.y;
    *w = ni->BBsubTree.w;
    *h = ni->BBsubTree.h;

    return ni->info;
}

//aux
void getNodesDentroRegiaoSmuT_aux(SmuTreap t, node_internal *root, double epsilon, Lista L, double rx1, double ry1, double rx2, double ry2){
    if (!root){
        return;
    }

    double root_x_min;
    double root_y_min;
    double root_w;
    double root_h;
    getBoundingBoxSmuT(t, root, &root_x_min, &root_y_min, &root_w, &root_h);

    double root_x_max = root_x_min + root_w;
    double root_y_max = root_y_min + root_h;
    
    if (!retangulos_interceptam(root_x_min, root_y_min, root_x_max, root_y_max, rx1, ry1, rx2, ry2)){
        return;
    }

    if(ponto_interno_retangulo(root->x, root->y, rx1, ry1, rx2, ry2, epsilon)){
        insereNaLista(L, (Item)root);
    }

    if (root->right) getNodesDentroRegiaoSmuT_aux(t, root->right, epsilon, L, rx1, ry1, rx2, ry2);
    if (root->left) getNodesDentroRegiaoSmuT_aux(t, root->left, epsilon, L, rx1, ry1, rx2, ry2);

    return;
}

bool getNodesDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2, Lista L){
    if(!t || !L){
        fprintf(stderr, "(getNodesDentroRegiaoSmuT) Erro: parametros invalidos.");
        return false;
    }

    SmuTreap_internal *t_i = (SmuTreap_internal*)t;
    if(!(t_i->root)){
        fprintf(stderr, "(getNodesDentroRegiaoSmuT) Erro: arvore vazia.");
    }

    Lista tempL = criaLista();
    getNodesDentroRegiaoSmuT_aux(t, t_i->root, t_i->epsilon, tempL, x1, y1, x2, y2);

    if (listaEstaVazia(tempL)) {
        destroiLista(tempL, NULL);
        return false;
    }

    while (!listaEstaVazia(tempL)){
        node_internal* n = (node_internal*)removePrimeiroDaLista(tempL);
        if (!n) continue;

        n->hitCountCounter++;

        if (n->hitCountCounter >= t_i->hitCountConfig) {
            double old_x = n->x;
            double old_y = n->y;
            n->hitCountCounter = 0;

            promoteNodeSmuT(t, (Node)n, t_i->promoRateConfig);

            Node n_new = recoverNodePostPromotion(t, old_x, old_y); // Usa a função auxiliar
            if (n_new) {
                insereNaLista(L, n_new);
            }
        } else {
            insereNaLista(L, (Item)n);
        }
    }
    destroiLista(tempL, NULL);
    return !listaEstaVazia(L);
}

//aux
void getInfosDentroRegiaoSmuT_aux(SmuTreap t, node_internal *root, double rx1, double ry1, double rx2, double ry2, FdentroDeRegiao f, Lista L){
    if(!root || !f) return; // Verifica validade

    double root_x_min;
    double root_y_min;
    double root_w;
    double root_h;
    getBoundingBoxSmuT(t, root, &root_x_min, &root_y_min, &root_w, &root_h);

    double root_x_max = root_x_min + root_w;
    double root_y_max = root_y_min + root_h;

    if (!retangulos_interceptam(root_x_min, root_y_min, root_x_max, root_y_max, rx1, ry1, rx2, ry2)) { // Poda quando se deve, para otimizar a busca
        return;
    }
    if(f(t, (Node)root, root->info, rx1, ry1, rx2, ry2)) insereNaLista(L, (Item)root);

    // Se nao e' no' folha, anda na arvore.
    if (root->right) getInfosDentroRegiaoSmuT_aux(t, root->right, rx1, ry1, rx2, ry2, f, L);
    if (root->left) getInfosDentroRegiaoSmuT_aux(t, root->left, rx1, ry1, rx2, ry2, f, L);
    return; // Volta um no'.
}

bool getInfosDentroRegiaoSmuT(SmuTreap t, double x1, double y1, double x2, double y2, FdentroDeRegiao f, Lista L){
    if(!t || !f || !L){
        fprintf(stderr, "(getInfosDentroRegiaoSmuT) Erro: parametros invalidos");
        return false;
    }

    SmuTreap_internal *t_i = (SmuTreap_internal*)t;
    if(!(t_i->root)){
        fprintf(stderr, "(getNodesDentroRegiaoSmuT) Erro: arvore vazia.");
    }

    Lista tempL = criaLista();
    getInfosDentroRegiaoSmuT_aux(t, t_i->root, x1, y1, x2, y2, f, tempL);

    if (listaEstaVazia(tempL)) {
        destroiLista(tempL, NULL);
        return false;
    }

    while(!listaEstaVazia(tempL)){
        node_internal* n = (node_internal*)removePrimeiroDaLista(tempL);
        if (!n) continue;

        n->hitCountCounter++;

        if (n->hitCountCounter >= t_i->hitCountConfig) {
            double old_x = n->x;
            double old_y = n->y;
            n->hitCountCounter = 0;

            promoteNodeSmuT(t, (Node)n, t_i->promoRateConfig);

            Node n_new = recoverNodePostPromotion(t, old_x, old_y); // Usa a função auxiliar
            if (n_new) {
                insereNaLista(L, n_new);
            }
        } else {
            insereNaLista(L, (Item)n);
        }
    }
    destroiLista(tempL, NULL);
    return !listaEstaVazia(L);
}

//aux
void getInfosAtingidoPontoSmuT_aux(SmuTreap_internal *t, node_internal *root, double x, double y, FpontoInternoAInfo f, Lista L){
    if (!root || !f) return;

    // Informacoes do BB que sera usado na busca. Atualiza a cada chamada recursiva.
    double bb_rx_min = root->BBsubTree.x;
    double bb_ry_min = root->BBsubTree.y;
    double bb_rw = root->BBsubTree.w;
    double bb_rh = root->BBsubTree.h;
    double bb_rx_max = bb_rx_min + bb_rw;
    double bb_ry_max = bb_ry_min + bb_rh;

    //Se o ponto (px, py) não está dentro do BBsubTree (que contém âncoras),
    // e' certo de que ele nao esta' dentro de alguma Info da subárvore.
    if(!ponto_interno_retangulo(x, y, bb_rx_min, bb_ry_min, bb_rx_max, bb_ry_max, t->epsilon)){ // Se P nao e' interno 'a BB do no' atual, volta um passo.
        return;
    }

    if(f((SmuTreap)t, (Node)root, root->info, x, y)){
        insereNaLista(L, (Item)root);
    }

    if(root->right) getInfosAtingidoPontoSmuT_aux(t, root->right, x, y, f, L);
    if(root->right) getInfosAtingidoPontoSmuT_aux(t, root->left, x, y, f, L);
    return;
}

bool getInfosAtingidoPontoSmuT(SmuTreap t, double x, double y, FpontoInternoAInfo f, Lista L){
    if(!t || !f || !L){
        fprintf(stderr, "(getInfosAtingidoPontoSmuT) Erro: parametros invalidos");
        return false;
    }

    SmuTreap_internal *t_i = (SmuTreap_internal*)t;
    if(!(t_i->root)){
        fprintf(stderr, "(getInfosAtingidoPontoSmuT) Erro: arvore vazia.");
    }

    Lista tempL = criaLista();
    getInfosAtingidoPontoSmuT_aux(t_i, t_i->root, x, y, f, tempL);

    if (listaEstaVazia(tempL)) {
        destroiLista(tempL, NULL);
        return false;
    }

    while (!listaEstaVazia(tempL)) {
        node_internal* n = (node_internal*)removePrimeiroDaLista(tempL);
        if (!n) continue;

        n->hitCountCounter++;

        if (n->hitCountCounter >= t_i->hitCountConfig) {
            double old_x = n->x;
            double old_y = n->y;
            n->hitCountCounter = 0;

            promoteNodeSmuT(t, (Node)n, t_i->promoRateConfig);

            Node n_new = recoverNodePostPromotion(t, old_x, old_y); // Usa a função auxiliar
            if (n_new) {
                insereNaLista(L, n_new);
            }
        } else {
            insereNaLista(L, (Item)n);
        }
    }

    destroiLista(tempL, NULL);
    return !listaEstaVazia(L);
}

//aux
void visitaProfundidadeSmuT_aux(SmuTreap t, node_internal *root, FvisitaNo f, void* aux){ // Preorder
    if(!root || !f) return;

    f(t, root, root->info, root->x, root->y, aux);       // Visita raiz.
    visitaProfundidadeSmuT_aux(t, root->left, f, aux);   // Visita subarvore esquerda.
    visitaProfundidadeSmuT_aux(t, root->right, f, aux);  // Visita subarvore direita.
}

void visitaProfundidadeSmuT(SmuTreap t, FvisitaNo f, void *aux){
    if (!t || !f){
        fprintf(stderr, "(visitaProfundidadeSmuT) Erro: parametros invalidos");
        return;
    }
    SmuTreap_internal *t_i = (SmuTreap_internal*)t;
    visitaProfundidadeSmuT_aux(t, t_i->root, f, aux);
}

void visitaLarguraSmuT(SmuTreap t, FvisitaNo f, void *aux){
    if (!t || !f) {
        fprintf(stderr, "(visitaLarguraSmuT) Erro: Parametros invalidos (arvore ou funcao de visita nula).\n");
        return;
    }

    SmuTreap_internal *treap_i = (SmuTreap_internal *)t;
    if (treap_i->root == NULL) {
        // Arvore vazia, nada a visitar.
        return;
    }

    Lista fila = criaLista();
    if (!fila) {
        fprintf(stderr, "(visitaLarguraSmuT) Erro: Nao foi possivel criar a fila para o percurso.\n");
        return;
    }

    // Adiciona a raiz à fila
    insereNaLista(fila, (Item)treap_i->root);

    while (!listaEstaVazia(fila)) {
        node_internal *no_atual = (node_internal *)removePrimeiroDaLista(fila);

        if (no_atual) {
            // Visita o nó atual
            f(t, (Node)no_atual, no_atual->info, no_atual->x, no_atual->y, aux);

            // Adiciona os filhos à fila, se existirem
            if (no_atual->left) {
                insereNaLista(fila, (Item)no_atual->left);
            }
            if (no_atual->right) {
                insereNaLista(fila, (Item)no_atual->right);
            }
        }
    }

    // Libera a memória usada pela estrutura da lista (não os nós da árvore)
    destroiLista(fila, NULL);
}

//aux
node_internal *procuraNoSmuT_aux(SmuTreap t, node_internal *root, FsearchNo f, void *aux, bool *flag_encontrado){
    if(root == NULL || !f || !flag_encontrado) return NULL;

    // Verifica o no atual
    if(f(t, (Node)root, root->info, root->x, root->y, aux)){
        *flag_encontrado = true;
        return root;
    }

    // Busca na subarvore direita
    node_internal *encontrado_dir = procuraNoSmuT_aux(t, root->right, f, aux, flag_encontrado);
    if(*flag_encontrado){
        return encontrado_dir;
    } else{
        // Busca na subarvore esquerda
        node_internal *encontrado_esq = procuraNoSmuT_aux(t, root->left, f, aux, flag_encontrado);
        if(*flag_encontrado) return encontrado_esq;
    }

    return NULL;

}

Node procuraNoSmuT(SmuTreap t, FsearchNo f, void *aux){
    if (!t || !f) return NULL;
    SmuTreap_internal *t_i = (SmuTreap_internal*)t;
    bool encontrado_flag = false;

    node_internal *encontrado = procuraNoSmuT_aux(t, t_i->root, f, aux, &encontrado_flag); 
    if(!encontrado){
        fprintf(stderr, "(procuraNoSmuT) Erro: no' nao encontrado.");
        return NULL;
    } else return (Node) encontrado;
}

/**
 * @brief Função auxiliar para escrever a representação DOT de um nó.
 */
void print_dot_node(node_internal *node, FILE *fp) {
    if (node == NULL) return;

    // Nó atual
    // Usaremos o endereço do nó como identificador único no DOT para evitar colisões com valores de âncora.
    fprintf(fp, "  node_%p [label=\"(%.2f, %.2f)\\nPrio: %d\\nBBsub[%.1f,%.1f,%.1f,%.1f]\"];\n",
            (void*)node, node->x, node->y, node->priority,
            node->BBsubTree.x, node->BBsubTree.y, node->BBsubTree.w, node->BBsubTree.h);

    // Conexões com filhos
    if (node->left) {
        fprintf(fp, "  node_%p -> node_%p [label=\"L\"];\n", (void*)node, (void*)node->left);
        print_dot_node(node->left, fp);
    } else {
         fprintf(fp, "  left_null_%p [shape=point, style=invis];\n", (void*)node);
         fprintf(fp, "  node_%p -> left_null_%p [style=invis];\n", (void*)node, (void*)node);
    }

    if (node->right) {
        fprintf(fp, "  node_%p -> node_%p [label=\"R\"];\n", (void*)node, (void*)node->right);
        print_dot_node(node->right, fp);
    } else {
        fprintf(fp, "  right_null_%p [shape=point, style=invis];\n", (void*)node);
        fprintf(fp, "  node_%p -> right_null_%p [style=invis];\n", (void*)node, (void*)node);
    }
}

bool printDotSmuTreap(SmuTreap t, char *fn){
    if (!t || !fn) return false;
    SmuTreap_internal *treap = (SmuTreap_internal*)t;

    FILE *fp = fopen(fn, "w");
    if (!fp) {
        perror("Erro ao abrir arquivo DOT para escrita");
        return false;
    }

    fprintf(fp, "digraph SmuTreap {\n");
    fprintf(fp, "  node [fontname=\"Helvetica\", shape=record];\n"); // Configurações globais de nó

    if (treap->root == NULL) {
        fprintf(fp, "  empty [label=\"Arvore Vazia\"];\n");
    } else {
        print_dot_node(treap->root, fp);
    }

    fprintf(fp, "}\n");
    fclose(fp);
    return true;
}

//aux
void killSmuTreap_aux(node_internal *no) {
    if (no == NULL) {
        return;
    }

    killSmuTreap_aux(no->left);   // Libera subárvore esquerda
    killSmuTreap_aux(no->right);  // Libera subárvore direita

    // Libera a informação associada ao nó, e o nó
    killNode(no);
}

void killSmuTreap(SmuTreap t){
    if (t == NULL) {
        return;
    }

    SmuTreap_internal *treap_i = (SmuTreap_internal *)t;

    // Chama a função auxiliar recursiva para liberar todos os nós
    killSmuTreap_aux(treap_i->root);

    // Libera a estrutura da SmuTreap
    free(treap_i);
}