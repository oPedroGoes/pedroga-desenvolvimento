#ifndef PROCESSASVG_H
#define PROCESSASVG_H

#include "SmuTreap.h"
#include "Lista.h"

/**
 * @file processaSvg.h
 * @brief Funções para geração de elementos SVG a partir das formas armazenadas na SmuTreap.
 *
 * Este módulo fornece funções para escrever as formas geométricas e anotações SVG em arquivos,
 * permitindo a visualização gráfica do estado da árvore e das operações realizadas.
 */

/**
 * @brief Escreve uma forma geométrica no arquivo SVG, no formato apropriado.
 * @param t Árvore SmuTreap.
 * @param n Nó da árvore.
 * @param i Ponteiro para a forma.
 * @param x_ancora_no Coordenada x da âncora do nó.
 * @param y_ancora_no Coordenada y da âncora do nó.
 * @param aux_file_ptr Ponteiro para o arquivo SVG aberto para escrita.
 */
void escreverFormaSvg(SmuTreap t, Node n, Info i, double x_ancora_no, double y_ancora_no, void *aux_file_ptr);

/**
 * @brief Escreve uma anotação SVG (string) no arquivo SVG.
 * @param anotacao String SVG a ser escrita.
 * @param aux Ponteiro para o arquivo SVG aberto para escrita.
 */
void visitaListaSvg(Item anotacao, void *aux);

#endif