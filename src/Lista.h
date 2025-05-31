#ifndef Lista_h
#define Lista_h


/**
Uma lista e´ uma colecao ordenada de itens (ou seja, um elemento possui um 
antecessor e um sucessor)  em que seus elementos podem
ser acessados atraves de sua posicao (tipo Posic). 
Uma lista sem nenhum elemento (comprimento 0) e' denominada lista vazia.

Uma instancia do tipo Posic indica a posicao de um item dentro da lista. 
Este tipo possui um valor invalido que indica nao se referir a nenhum item. 
Tal valor invalido e' denotado por NIL.

Este modulo prove diversos iteradores: 1 deles genérico e 3 comuns a linguagens
de programacao funcionais (high-order functions).
*/

typedef void *Lista;
typedef void *Posic;
typedef void *Item;

/*
 * Retorna -1, se item1 < item2; 1, se item1 > item2; 0, caso contrario.
 */
typedef int (*compara)(Item, Item, float, float);

#define NIL NULL;
#define CAPAC_ILIMITADA -1;

/** Retorna uma lista vazia. A lista pode definir um limite maximo de 
elementos armazenados (capacidade). Caso capacidade < 0, o tamanho da lista é 
ilimitado */
Lista createLst();


/**  Insere o item info no final da lista L. O comprimento da
lista e' acrescido de 1 elemento. 
Retorna um indicador para o elemento acrescentado; ou NIL, se a lista estiver
cheia */
void insertLst(Lista L, Item info);

Item getInfo(Lista L, int p);

void ordena(Lista L, compara c, float xcent, float ycent);

/****
 ****
 **** ACRESCENTAR OUTRAS ROTINAS
 ****
 ****/

/** Libera toda memoria alocada pela lista. */
void killLst(Lista L);


int get_tamanholista(Lista L);

Item removePrimeiroElemento(Lista l);

void removeElemento(Lista l, Item i);

/**
 ** High-order functions
 **/

typedef void *Clausura;
typedef Item (*Apply)(Item item, Clausura c);
typedef int (*Check)(Item item, Clausura c);
typedef void (*ApplyClosure)(Item item, Clausura c);

/** Cria uma nova lista. Aplica a funcao f a cada item de L
    e insere o resultado na nova lista.
 */
Lista map(Lista L, Apply f, Clausura c);

/** 
   Cria uma nova lista contendo os itens de L para os quais a
   invocacao da funcao f retornar verdeira.
 */
Lista filter(Lista L, Check f, Clausura c);

/**
   Aplica a funcao f a cada elemento de L, possivelmente, atualizando
   o conteudo de c.
 */
void fold(Lista L, ApplyClosure f, Clausura c);

#endif