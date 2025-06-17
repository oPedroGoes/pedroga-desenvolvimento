#ifndef FORMAS_H
#define FORMAS_H

#include "SmuTreap.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"

/**
 * @file formas.h
 * @brief TAD utilitário para manipulação genérica de formas geométricas.
 *
 * Este módulo fornece funções para acessar e modificar propriedades de formas
 * geométricas (círculo, retângulo, linha, texto) de maneira genérica, usando
 * ponteiros void e um descritor de tipo. Permite operações como obtenção de ID,
 * área, âncoras, cores, largura da borda e destruição da forma, facilitando o
 * uso polimórfico em estruturas como árvores e listas.
 */

/**
 * @enum TIPO_FORMA
 * @brief Enumeração dos tipos de formas suportados.
 */
typedef enum{
    TIPO_DESCONHECIDO = 0, 
    TIPO_CIRCULO = 1, 
    TIPO_RETANGULO = 2, 
    TIPO_TEXTO = 3, 
    TIPO_LINHA = 4
} TIPO_FORMA;

/**
 * @brief Duplica uma string em uma nova área de memória.
 * @param s String de entrada.
 * @return Ponteiro para a nova string (deve ser liberada com free), ou NULL em caso de erro.
 */
char* my_strdup(const char* s);

/**
 * @brief Obtém o identificador da forma, independentemente do tipo.
 * @param forma Ponteiro para a estrutura da forma.
 * @param tipo Descritor do tipo da forma.
 * @return Identificador da forma, ou -1 em caso de erro.
 */
int get_idF(Info forma, DescritorTipoInfo tipo);

/**
 * @brief Retorna o nome do tipo da forma como string.
 * @param tipo Descritor do tipo da forma.
 * @return String constante com o nome do tipo ("circulo", "retangulo", etc).
 */
const char* get_NameStrF(DescritorTipoInfo tipo);

/**
 * @brief Calcula a área da forma, de acordo com seu tipo.
 * @param forma Ponteiro para a estrutura da forma.
 * @param tipo Descritor do tipo da forma.
 * @return Área da forma, ou 0.0 para tipos desconhecidos.
 */
double get_areaF(Info forma, DescritorTipoInfo tipo);

/**
 * @brief Obtém as coordenadas da(s) âncora(s) de uma forma geométrica.
 *
 * Para círculos, retângulos e textos, retorna a âncora principal (x1, y1).
 * Para linhas, retorna também o ponto final (x2, y2).
 *
 * @param forma Ponteiro para a estrutura da forma.
 * @param tipo Descritor do tipo da forma.
 * @param x1 Ponteiro para armazenar a coordenada X da primeira âncora.
 * @param y1 Ponteiro para armazenar a coordenada Y da primeira âncora.
 * @param x2 Ponteiro para armazenar a coordenada X da segunda âncora (usado apenas para linhas).
 * @param y2 Ponteiro para armazenar a coordenada Y da segunda âncora (usado apenas para linhas).
 * @return Número de âncoras preenchidas (1 para formas comuns, 2 para linhas, 0 para erro).
 */
int get_anchorF(Info forma, DescritorTipoInfo tipo, double *x1, double *y1, double *x2, double *y2);

/**
 * @brief Função de callback para verificar se uma forma está totalmente contida em uma região retangular.
 * @param t Árvore SmuTreap.
 * @param n_node Nó da árvore.
 * @param forma Ponteiro para a estrutura da forma.
 * @param reg_x1, reg_y1, reg_x2, reg_y2 Coordenadas do retângulo de referência.
 * @return true se a forma está totalmente contida, false caso contrário.
 */
bool formaTotalmenteContidaCallback(SmuTreap t, Node n_node, Info forma, double reg_x1, double reg_y1, double reg_x2, double reg_y2);

/**
 * @brief Obtém as cores da forma (borda e preenchimento), de forma genérica.
 *
 * Para linhas, retorna apenas a cor da linha em new_corb e new_corp será NULL.
 * Para outras formas, retorna as duas cores.
 *
 * @param forma Ponteiro para a estrutura da forma.
 * @param tipo Descritor do tipo da forma.
 * @param new_corb Referência para ponteiro de string da cor da borda (alocada dinamicamente).
 * @param new_corp Referência para ponteiro de string da cor de preenchimento (alocada dinamicamente).
 * @return 2 se retornou duas cores, 1 se apenas uma, 0 em caso de erro.
 */
int get_corF(Info forma, DescritorTipoInfo tipo, char** new_corb, char** new_corp);

/**
 * @brief Define as cores da forma (borda e preenchimento), de forma genérica.
 *
 * Para linhas, apenas a cor da linha é alterada.
 *
 * @param forma Ponteiro para a estrutura da forma.
 * @param tipo Descritor do tipo da forma.
 * @param new_corb Nova cor da borda.
 * @param new_corp Nova cor de preenchimento.
 */
void set_corF(Info forma, DescritorTipoInfo tipo, const char* new_corb, const char* new_corp);

/**
 * @brief Define a largura da borda da forma, de forma genérica.
 * @param forma Ponteiro para a estrutura da forma.
 * @param tipo Descritor do tipo da forma.
 * @param strokeWidth Nova largura da borda.
 */
void set_strkWF(Info forma, DescritorTipoInfo tipo, double strokeWidth);

/**
 * @brief Libera a memória ocupada pela forma, de acordo com seu tipo.
 * @param forma Ponteiro para a estrutura da forma.
 * @param tipo Descritor do tipo da forma.
 */
void killF(Info forma, DescritorTipoInfo tipo);

#endif