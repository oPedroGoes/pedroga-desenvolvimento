#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "SmuTreap.h"
typedef void* BOUNDING_BOX; 

/**
 * @file boundingBox.h
 * @brief Funções utilitárias para cálculo de bounding boxes (BB) de formas geométricas.
 *
 * Este módulo fornece funções para calcular o retângulo delimitador (bounding box)
 * de diferentes tipos de formas geométricas (círculo, retângulo, texto, linha).
 * O bounding box é utilizado para otimizações em estruturas de dados espaciais,
 * como a SmuTreap, facilitando operações de busca, inserção e remoção.
 *
 * Todas as funções recebem um ponteiro genérico para a forma (Info) e retornam,
 * por referência, as coordenadas do canto superior esquerdo (x, y), largura (w)
 * e altura (h) do bounding box.
 */

 /**
 * @brief Calcula o bounding box de um círculo.
 * @param i Ponteiro para a estrutura do círculo.
 * @param bbA_x Ponteiro para receber a coordenada x do BB.
 * @param bbA_y Ponteiro para receber a coordenada y do BB.
 * @param bbA_w Ponteiro para receber a largura do BB.
 * @param bbA_h Ponteiro para receber a altura do BB.
 */
void calcBB_c(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);

/**
 * @brief Calcula o bounding box de um retângulo.
 * @param i Ponteiro para a estrutura do retângulo.
 * @param bbA_x Ponteiro para receber a coordenada x do BB.
 * @param bbA_y Ponteiro para receber a coordenada y do BB.
 * @param bbA_w Ponteiro para receber a largura do BB.
 * @param bbA_h Ponteiro para receber a altura do BB.
 */
void calcBB_r(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);

/**
 * @brief Calcula o bounding box de um texto.
 * @param i Ponteiro para a estrutura do texto.
 * @param bbA_x Ponteiro para receber a coordenada x do BB.
 * @param bbA_y Ponteiro para receber a coordenada y do BB.
 * @param bbA_w Ponteiro para receber a largura do BB.
 * @param bbA_h Ponteiro para receber a altura do BB.
 */
void calcBB_t(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);

/**
 * @brief Calcula o bounding box de uma linha.
 * @param i Ponteiro para a estrutura da linha.
 * @param bbA_x Ponteiro para receber a coordenada x do BB.
 * @param bbA_y Ponteiro para receber a coordenada y do BB.
 * @param bbA_w Ponteiro para receber a largura do BB.
 * @param bbA_h Ponteiro para receber a altura do BB.
 */
void calcBB_l(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);

/**
 * @brief Calcula o bounding box individual de uma forma, de acordo com seu tipo.
 * @param d Descritor do tipo da forma (ex: TIPO_CIRCULO, TIPO_RETANGULO, etc).
 * @param i Ponteiro para a estrutura da forma.
 * @param bbA_x Ponteiro para receber a coordenada x do BB.
 * @param bbA_y Ponteiro para receber a coordenada y do BB.
 * @param bbA_w Ponteiro para receber a largura do BB.
 * @param bbA_h Ponteiro para receber a altura do BB.
 */
void fCalcBB_individual(DescritorTipoInfo d, Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);



#endif