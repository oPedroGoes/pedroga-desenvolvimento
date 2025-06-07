#include <stdio.h>
#include <stdlib.h>
#include "Lista.h"

typedef struct NoLista {
    Item item;            // Ponteiro para dado generico armazenado
    struct NoLista *prox;
} NoLista;

typedef struct {
    NoLista *primeiro;
    NoLista *ultimo;
    int tamanho;
}Lista_internal;

Lista criaLista() {
    Lista_internal *l = (Lista_internal*)malloc(sizeof(Lista_internal));
    if (!l){
        perror("Erro ao alocar memoria para a lista\n");
        return NULL;
    }
    l->primeiro = NULL;
    l->ultimo = NULL;
    l->tamanho = 0;
    return l;
}

void destroiLista(Lista l, void (*liberaItemNaoNulo)(Item item)) {
    if (!l){
        perror("(destroiLista) Erro: parametro para lista invalido.\n");
        return;
    }
    Lista_internal *li = (Lista_internal*)l;

    NoLista *atual = li->primeiro;
    NoLista *proximoNo;

    while (atual != NULL) {
        proximoNo = atual->prox;
        if (liberaItemNaoNulo != NULL && atual->item != NULL) {
            liberaItemNaoNulo(atual->item);
        }
        free(atual);
        atual = proximoNo;
    }
    free(l);
}

bool insereNaLista(Lista l, Item item) {
    printf("DEBUG    (insereNaLista) Lista l = %p\nDEBUG    (insereNaLista) Item item = %p\n", l, item);
    if (!l || !item){
        perror("\n(insereNaLista) Erro: parametros invalidos\n");
        return false; // Lista inválida
    }

    Lista_internal *li = (Lista_internal*)l;

    NoLista *novoNo = (NoLista *)malloc(sizeof(NoLista));
    if (novoNo == NULL) {
        perror("\n(insereNaLista) Erro ao alocar memoria para novo no da lista\n");
        return false;
    }
    novoNo->item = item;
    printf("DEBUG    (insereNaLista) novoNo->item = %p\n", novoNo->item);
    novoNo->prox = NULL;

    if (li->primeiro == NULL) { // Lista estava vazia
        li->primeiro = novoNo;
        li->ultimo = novoNo;
    } else { // Lista não estava vazia
        li->ultimo->prox = novoNo;
        li->ultimo = novoNo;
    }
    li->tamanho++;
    return true;
}

Item removePrimeiroDaLista(Lista l) {
    if (!l){
        perror("\n(removePrimeiroDaLista) Erro: parametros invalidos\n");
        return NULL; // Lista inválida
    }
    Lista_internal *li = (Lista_internal*)l;

    if (li->primeiro == NULL){
        perror("\n(removePrimeiroDaLista) Erro: lista vazia\n");
        return NULL;
    }

    NoLista *noRemovido = li->primeiro;
    Item itemRemovido = noRemovido->item;

    li->primeiro = noRemovido->prox;
    if (li->primeiro == NULL) { // A lista ficou vazia
        li->ultimo = NULL;
    }

    free(noRemovido);
    li->tamanho--;
    return itemRemovido;
}

bool listaEstaVazia(Lista l) {
    if (l == NULL) {
        perror("\n(listaEstaVazia) Erro: lista invalida.\n");
        return true; // Considera NULL como vazia para evitar erros
    }

    Lista_internal *li = (Lista_internal*)l;

    return li->tamanho == 0;
}

int getTamanhoLista(Lista l) {
    if (l == NULL) {
        perror("\n(getTamanhoLista) Erro: lista invalida.\n");
        return 0; // Lista inválida
    }
    Lista_internal *li = (Lista_internal*)l;
    return li->tamanho;
}

void percorreLista(Lista l, void (*visita)(Item item, void *aux), void *aux) {
    if (l == NULL || visita == NULL){
        perror("\n(percorreLista) Erro: parametros invalidos\n");
        return;
    }

    Lista_internal *li = (Lista_internal*)l;
    printf("\nDEBUG    (percorreLista) Lista_internal li = %p\n", li);

    if (listaEstaVazia(l)) printf("Lista esta vazia\n");

    NoLista *atual = li->primeiro;
    while (atual != NULL) {
        visita(atual->item, aux);
        atual = atual->prox;
    }
}