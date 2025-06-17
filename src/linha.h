#ifndef LINHA_H
#define LINHA_H

/**
 * @file linha.h
 * @brief Interface para o TAD LINHA, que representa segmentos de reta.
 *
 * Fornece funções para criar, acessar e modificar propriedades de linhas,
 * como coordenadas dos pontos inicial e final, cor e largura.
 */

typedef void* LINHA;

/**
 * @brief Cria uma nova linha.
 * @param id Identificador da linha.
 * @param x1 Coordenada x do ponto inicial.
 * @param y1 Coordenada y do ponto inicial.
 * @param x2 Coordenada x do ponto final.
 * @param y2 Coordenada y do ponto final.
 * @param c Cor da linha.
 * @return Ponteiro para a linha criada.
 */
LINHA cria_linha(int id, double x1, double y1, double x2, double y2, char* c);

/**
 * @brief Obtém o identificador da linha.
 * @param l Linha.
 * @return Identificador.
 */
int get_idL(LINHA l);

/**
 * @brief Define o identificador da linha.
 * @param l Linha.
 * @param id Novo identificador.
 */
void set_idL(LINHA l, int id);

/**
 * @brief Obtém a coordenada x do ponto inicial.
 * @param l Linha.
 * @return Coordenada x inicial.
 */
double get_X1L(LINHA l);

/**
 * @brief Define a coordenada x do ponto inicial.
 * @param l Linha.
 * @param x Nova coordenada x inicial.
 */
void set_x1L(LINHA l, double x);

/**
 * @brief Obtém a coordenada y do ponto inicial.
 * @param l Linha.
 * @return Coordenada y inicial.
 */
double get_Y1L(LINHA l);

/**
 * @brief Define a coordenada y do ponto inicial.
 * @param l Linha.
 * @param y Nova coordenada y inicial.
 */
void set_y1L(LINHA l, double y);

/**
 * @brief Obtém a coordenada x do ponto final.
 * @param l Linha.
 * @return Coordenada x final.
 */
double get_X2L(LINHA l);

/**
 * @brief Define a coordenada x do ponto final.
 * @param l Linha.
 * @param x Nova coordenada x final.
 */
void set_x2L(LINHA l, double x);

/**
 * @brief Obtém a coordenada y do ponto final.
 * @param l Linha.
 * @return Coordenada y final.
 */
double get_Y2L(LINHA l);

/**
 * @brief Define a coordenada y do ponto final.
 * @param l Linha.
 * @param y Nova coordenada y final.
 */
void set_y2L(LINHA l, double y);

/**
 * @brief Obtém a cor da linha.
 * @param l Linha.
 * @return String com a cor.
 */
char* get_cL(LINHA l);

/**
 * @brief Define a cor da linha.
 * @param l Linha.
 * @param c Nova cor (string).
 */
void set_cL(LINHA l, char* c);

/**
 * @brief Obtém a largura da linha.
 * @param l Linha.
 * @return Largura da linha.
 */
double get_strkWL(LINHA l);

/**
 * @brief Define a largura da linha.
 * @param l Linha.
 * @param sw Nova largura.
 */
void set_strkWL(LINHA l, double sw);

/**
 * @brief Obtém o comprimento da linha.
 * @param l Linha.
 * @return Comprimento.
 */
double get_comprimentoL(LINHA l);

/**
 * @brief Obtém a "área" associada à linha (valor simbólico).
 * @param l Linha.
 * @return Área.
 */
double get_areaL(LINHA l);

/**
 * @brief Libera a memória da linha.
 * @param l Linha.
 */
void kill_linha(LINHA l);

#endif