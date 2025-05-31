#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef int nodeKeyType;
typedef int nodePriorityType;

typedef struct node{
    nodeKeyType key;
    nodePriorityType priority;
    int depth;
    struct node* left;
    struct node* right;
}node;

typedef struct{
    int height;
    node* root;
}tree;

tree* initialize_tree(){
    tree *t1 = (tree*) malloc(sizeof(tree));

    // Verifica o retorno do malloc
    if (t1 == NULL){
        printf("(initialize_tree) Erro: falha ao alocar memória para t1\n");
        exit(1);
    }

    t1->height = 0;
    t1->root = NULL;

    return t1;
}

node* new_node(nodeKeyType key, nodePriorityType priority){
    node* newNode = (node*) malloc(sizeof(node));

    // Verifica o retorno do malloc
    if (newNode == NULL){
        printf("(new_node) Erro: falha ao alocar memória para o nó\n");
        return NULL;
    }

    newNode->depth = 0;
    newNode->key = key;
    newNode->priority = priority;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

/* Busca a partir de um no' raiz.
* @return NULL para erro; no' encontrado para sucesso.
*/
node* search_aux(node* root, nodeKeyType key){
    // no' com tal chave nao existe na arvore
    if (root == NULL){
        printf("(search) Erro: no nao encontrado");
        return NULL;
    }
    
    // Se encontrou no' com tal chave, retorna no'
    if (key == root->key){
        return root;
    }

    // Mecanismo padrão de busca binaria
    if (key < root->key)
        return search_aux(root->left, key);
    else
        return search_aux(root->right, key);
}

/* Busca a partir de uma arvore.
* @return NULL para erro; no' encontrado para sucesso.
*/
node* search(tree* t1, nodeKeyType key){
    if (t1 == NULL){
        printf("(search) Erro: ponteiro para arvore aponta NULL.");
        exit(1);
    }

    if (t1->root == NULL){
        printf("(search) Erro: arvore vazia.");
        return NULL;
    }

    return search_aux(t1->root, key);
}

void print_tree(node* root, int space, int increment) {
    if (root == NULL) {
        return;
    }

    // Aumenta a distância entre níveis
    space += increment;

    // Processo o filho da direita primeiro
    print_tree(root->right, space, increment);

    // Imprime o nó atual após o espaço apropriado
    printf("\n");
    for (int i = increment; i < space; i++) {
        printf(" ");
    }
    printf("%d (Pri: %d)\n", root->key, root->priority);

    // Processo o filho da esquerda
    print_tree(root->left, space, increment);
}

void print_tree_wrapper(tree* t1) {
    if (t1 == NULL || t1->root == NULL) {
        printf("Árvore vazia ou inexistente.\n");
        return;
    }
    print_tree(t1->root, 0, 5);
}


/* "Eu" viro filho direito do meu filho esquerdo, 
*   e os flhos direitos dele viram meus filhos esquerdos.
*
* @returns sucesso - No' do ANTIGO filho direito \n
*          erro - NULL
*/
node* rotateToRight (node** rootRef){
    /* Ao passar como parâmetro um ponteiro simples *root, criamos uma variável de
    * nome root que apenas recebe o valor que a variável root da função original
    * armazena. Ou seja, nesse caso, temos apenas uma cópia, e não temos acesso
    * à root da função original. Isso implicaria em termos que atualizar a root
    * original na própria função que chamou a rotação.
    *  Quando utilizamos ponteiros duplos, passamos como parâmetro o endereço
    * da root original, e, portanto, temos acesso a ela e podemos modificar
    * o que quisermos.
    * 
    * Define-se aqui root como a raiz que queremos rotacionar, sendo que
    *  rootRef é como um acesso ao ponteiro *root da função que chamou a rotação.
    */
    node *root = *rootRef;
    
    /* Guarda o endereco de memoria do filho esquerdo
    *  para o retornar.
    */
    if (root->left == NULL){
        printf("(rotateToRight) Erro: funcao tenta acessar NULL [root->left]\n");
        exit(1);
    }

    node *leftSon = root->left;                                                     

    // "filhos direitos dele viram meus filhos esquerdos"
    root->left = root->left->right;

    // "'Eu' viro filho direito do meu filho esquerdo"
    leftSon->right = root;

    /* Faz o root da função que chamou a rotação  
    * apontar para leftSon, sendo então atualizado
    * na proxima volta da recursão.
    */
    *rootRef = leftSon;

    /* Retorna o ANTIGO filho esquerdo para que o pai
    *  de root seja ligado ao ANTIGO filho esquerdo na
    *  proxima chamada recursiva de insert
    */
    return *rootRef; // na real, acho que tanto faz o que retorna. Poderia retornar void.
}

/* "Eu" viro filho esquerdo do meu filho direito, 
*   e os flhos esquerdos dele viram meus filhos direitos.
* 
* @returns sucesso - No' do ANTIGO filho direito \n
*          erro - NULL
*/
node* rotateToLeft (node** rootRef){
    // Cf. o comentario na primeira linha de rotateToRight.
    node *root = *rootRef;

    /* Guarda o endereco de memoria do filho direito
    *  para o retornar.
    */

    if (root->right == NULL){
        printf("(rotateToLeft) Erro: funcao tenta acessar NULL [root->right]\n");
        return NULL;
    }

    node *rightSon = root->right;

    // "filhos esquerdos dele viram meus filhos direitos"
    root->right = root->right->left;

    // "'Eu' viro filho esquerdo do meu filho direito"
    rightSon->left = root;

    // Cf. o comentario nesse mesmo caso em rotateToLeft.
    *rootRef = rightSon;
    
    /* Retorna o ANTIGO filho direito para que o pai
    *  de root seja ligado ao ANTIGO filho direito na
    *  proxima chamada recursiva de insert
    */
    return *rootRef;
}


/* @note  Parcialmente certa. Do modo que está, quando acontecem as rotacões,
* @note   o pai do "eu" nunca é atualizado para apontar para o filho correto. Ou eu
* @note   faco root apontar para o filho correto, ou eu tiro return root e coloco
* @note   return filhoCorreto na funcão de rotacionar.
* 
* @return Sucesso - no' raiz da arvore *
*         Erro - NULL
*/
node* insert_aux(node *root, nodeKeyType key, nodePriorityType priority){
    if (root == NULL){
        root = new_node(key, priority);

        if (root == NULL){
            printf("(insert_aux) Erro: falha na alocacão de memória para novo no'");
            exit(1);
        }

        return root;
    }

    if (key == root->key){
        printf("(insert_aux) Erro: nó já existente.\n");
        return NULL;
    }

    if (key < root->key){
        root->left = insert_aux(root->left, key, priority);

        if (root->left->priority > root->priority){
            rotateToRight(&root);

            if (root == NULL){
                printf("(insert_aux) Erro: falha em rotacionar nós[rotateToLeft].");
                exit(1);
            }
        }
    }
    else{
        root->right = insert_aux(root->right, key, priority);

        if (root->right->priority > root->priority){
            rotateToLeft(&root);

            if (root == NULL){
                printf("(insert_aux) Erro: falha em rotacionar nós[rotateToRight].");
                exit(1);
            }
        }
    }

    return root;
}

node* insert (tree *t1, nodeKeyType key, nodePriorityType priotiry){
    t1->root = insert_aux(t1->root, key, priotiry);
    return t1->root;
}

/*node* remove_aux(node* root, nodeKeyType key){
    if (root->left->priority > root->right->priority){
       
    }
}

node* remove (tree* t1, nodeKeyType key){
    if (t1 == NULL){
        printf("(remove) Erro: ponteiro para arvore aponta NULL. \n");
        exit(1);
    }

    if (t1->root == NULL){
        printf("(remove) Erro: arvore vazia. \n");
        return NULL;
    }

    return remove_aux(search(t1, key), key);
}*/


int main()
{
    tree* t1 = initialize_tree();

    insert(t1, 70, 7);
    printf("\n\nEstrutura da Treap:\n");
    print_tree_wrapper(t1);

    insert(t1, 30, 8);
    printf("\n\nEstrutura da Treap:\n");
    print_tree_wrapper(t1);

    insert(t1, 60, 4);
    printf("\n\nEstrutura da Treap:\n");
    print_tree_wrapper(t1);

    insert(t1, 50, 9);
    printf("\n\nEstrutura da Treap:\n");
    print_tree_wrapper(t1);

    insert(t1, 80, 6);
    printf("\n\nEstrutura da Treap:\n");
    print_tree_wrapper(t1);

    insert(t1, 10, 5);
    printf("\n\nEstrutura da Treap:\n");
    print_tree_wrapper(t1);

    insert(t1, 40, 3);
    printf("\n\nEstrutura da Treap:\n");
    print_tree_wrapper(t1);

    return 0;
}