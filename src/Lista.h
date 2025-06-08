#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h> // Para usar o tipo bool

/**
 * @file Lista.h
 * @brief Interface para um Tipo Abstrato de Dados Lista Genérica.
 *
 * A lista é implementada como uma lista simplesmente encadeada e pode
 * armazenar ponteiros para qualquer tipo de dado (void*).
 */


typedef void *Lista;
typedef void *Item;

/**
 * @brief Cria uma nova lista vazia.
 *
 * Aloca memória para a estrutura da lista e a inicializa.
 *
 * @return Ponteiro para a nova lista criada. Retorna NULL em caso de falha
 * na alocação de memória.
 */
Lista criaLista();

/**
 * @brief Destroi uma lista, liberando toda a memória alocada.
 *
 * Percorre todos os elementos da lista, liberando os nós da lista.
 * Se uma função 'liberaItemNaoNulo' for fornecida, ela é chamada para cada
 * item armazenado na lista antes de liberar o nó que o contém. Isso é útil
 * para liberar a memória dos dados apontados pelos itens, caso a lista
 * seja proprietária desses dados.
 *
 * @param l Ponteiro para a lista a ser destruída. Se NULL, nada acontece.
 * @param liberaItemNaoNulo Ponteiro para uma função que sabe como liberar a memória
 * de um item armazenado na lista. Se os itens não
 * precisam ser liberados pela lista (por exemplo, são
 * gerenciados externamente ou são tipos primitivos
 * armazenados diretamente de alguma forma),
 * este parâmetro pode ser NULL.
 */
void destroiLista(Lista l, void (*liberaItemNaoNulo)(Item item));

/**
 * @brief Insere um novo item no final da lista.
 *
 * Aloca um novo nó para armazenar o item e o adiciona ao final da lista.
 *
 * @param l Ponteiro para a lista onde o item será inserido. Não deve ser NULL.
 * @param item Ponteiro para o item a ser adicionado à lista.
 * @return true se a inserção for bem-sucedida, false em caso de falha
 * (por exemplo, falha na alocação de memória para o novo nó).
 */
bool insereNaLista(Lista l, Item item);

Item getPriomeiroNoLista(Lista l);

/**
 * @brief Remove e retorna o primeiro item da lista.
 *
 * O nó que continha o item é liberado. O item em si (o dado apontado)
 * não é liberado por esta função; o chamador é responsável por isso, se necessário.
 *
 * @param l Ponteiro para a lista. Não deve ser NULL.
 * @return Ponteiro para o primeiro item da lista. Retorna NULL se a lista
 * estiver vazia.
 */
Item removePrimeiroDaLista(Lista l);

/**
 * @brief Verifica se a lista está vazia.
 *
 * @param l Ponteiro para a lista. Não deve ser NULL.
 * @return true se a lista não contiver nenhum item, false caso contrário.
 */
bool listaEstaVazia(Lista l);

/**
 * @brief Retorna o número de itens atualmente na lista.
 *
 * @param l Ponteiro para a lista. Não deve ser NULL.
 * @return O número de itens na lista.
 */
int getTamanhoLista(Lista l);

/**
 * @brief Percorre a lista e aplica uma função a cada item.
 *
 * Esta função itera sobre cada item da lista, do primeiro ao último,
 * e invoca a função 'visita' para cada um deles.
 *
 * @param l Ponteiro para a lista a ser percorrida. Não deve ser NULL.
 * @param visita Ponteiro para uma função que será chamada para cada item.
 * A função 'visita' recebe o item e um dado auxiliar 'aux'.
 * @param aux Ponteiro para um dado auxiliar que será passado para cada
 * chamada da função 'visita'. Pode ser NULL se não for necessário.
 */
void percorreLista(Lista l, void (*visita)(Item item, void *aux), void *aux);

#endif // LISTA_H