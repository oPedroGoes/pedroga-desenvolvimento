#ifndef GEOMETRIA_H
#define GEOMETRIA_H

#include "boundingBox.h"

/**
 * @file geometria.h
 * @brief Funções utilitárias para operações geométricas com retângulos e pontos.
 *
 * Este módulo fornece funções para verificar relações espaciais entre retângulos e pontos,
 * como inclusão, interseção e pertinência. Essas operações são fundamentais para algoritmos
 * de busca espacial, seleção de regiões e manipulação de formas geométricas em estruturas
 * como árvores e listas.
 */

/**
 * @brief Verifica se um retângulo está totalmente contido dentro de outro retângulo.
 *
 * Os retângulos são definidos por dois pontos opostos (Dx1, Dy1)-(Dx2, Dy2) para o retângulo interno,
 * e (Fx1, Fy1)-(Fx2, Fy2) para o retângulo externo.
 *
 * @param Dx1, Dy1 Coordenadas do primeiro vértice do retângulo interno.
 * @param Dx2, Dy2 Coordenadas do vértice oposto do retângulo interno.
 * @param Fx1, Fy1 Coordenadas do primeiro vértice do retângulo externo.
 * @param Fx2, Fy2 Coordenadas do vértice oposto do retângulo externo.
 * @return true se o retângulo interno está totalmente contido no externo, false caso contrário.
 */
bool retanguloInternoRetangulo(double Dx1, double Dy1, double Dx2, double Dy2,
                               double Fx1, double Fy1, double Fx2, double Fy2);

/**
 * @brief Verifica se dois retângulos se interceptam (sobrepõem).
 *
 * Cada retângulo é definido por dois pontos opostos.
 *
 * @param R1x1, R1y1 Coordenadas do primeiro vértice do retângulo 1.
 * @param R1x2, R1y2 Coordenadas do vértice oposto do retângulo 1.
 * @param R2x1, R2y1 Coordenadas do primeiro vértice do retângulo 2.
 * @param R2x2, R2y2 Coordenadas do vértice oposto do retângulo 2.
 * @return true se os retângulos se interceptam, false caso contrário.
 */
bool retangulos_interceptam(double R1x1, double R1y1, double R1x2, double R1y2,
                            double R2x1, double R2y1, double R2x2, double R2y2);

/**
 * @brief Verifica se um ponto está dentro de um retângulo, considerando uma margem de erro epsilon.
 *
 * O retângulo é definido por dois pontos opostos (rx1, ry1)-(rx2, ry2).
 *
 * @param px, py Coordenadas do ponto a ser testado.
 * @param rx1, ry1 Coordenadas do primeiro vértice do retângulo.
 * @param rx2, ry2 Coordenadas do vértice oposto do retângulo.
 * @param epsilon Margem de erro para a verificação da inclusão do ponto.
 * @return true se o ponto está dentro do retângulo, false caso contrário.
 */
bool ponto_interno_retangulo(double px, double py, double rx1, double ry1, double rx2, double ry2, double epsilon);

#endif