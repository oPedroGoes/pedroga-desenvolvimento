#ifndef CIRCULO_H
#define CIRCULO_H

/**
 * @file circulo.h
 * @brief Funções utilitárias para manipulação de círculos.
 *
 * Este módulo fornece funções para criar, acessar e modificar propriedades de círculos,
 * como coordenadas, raio, cor da borda e cor de preenchimento.
 */

typedef void* CIRCLE;

/**
 * @brief Cria um círculo com as propriedades especificadas.
 * @param id Identificador do círculo.
 * @param x Coordenada x do centro do círculo.
 * @param y Coordenada y do centro do círculo.
 * @param r Raio do círculo.
 * @param cb Cor da borda do círculo (string).
 * @param cp Cor de preenchimento do círculo (string).
 * @return Retorna um ponteiro para o círculo criado.
 */
CIRCLE create_circle(int id, double x, double y, double r, char *cb, char *cp);

/**
 * @brief Obtém o identificador do círculo.
 * @param c Círculo.
 * @return Identificador.
 */
int get_idC(CIRCLE c);

/**
 * @brief Define o identificador do círculo.
 * @param c Círculo.
 * @param id Novo identificador.
 */
void set_idC(CIRCLE c, int id);

/**
 * @brief Obtém a coordenada x do centro.
 * @param c Círculo.
 * @return Coordenada x.
 */
double get_XC(CIRCLE c);

/**
 * @brief Define a coordenada x do centro.
 * @param c Círculo.
 * @param x Nova coordenada x.
 */
void set_xC(CIRCLE c, double x);

/**
 * @brief Obtém a coordenada y do centro.
 * @param c Círculo.
 * @return Coordenada y.
 */
double get_YC(CIRCLE c);

/**
 * @brief Define a coordenada y do centro.
 * @param c Círculo.
 * @param y Nova coordenada y.
 */
void set_yC(CIRCLE c, double y);

/**
 * @brief Obtém a cor da borda.
 * @param c Círculo.
 * @return String com a cor da borda.
 */
char* get_cbC(CIRCLE c);

/**
 * @brief Define a cor da borda do círculo.
 * @param c Círculo.
 * @param cb Nova cor da borda (string).
 */
void set_cbC(CIRCLE c, char* cb);

/**
 * @brief Obtém a cor de preenchimento do círculo.
 * @param c Círculo.
 * @return String com a cor de preenchimento.
 */
char* get_cpC(CIRCLE c);

/**
 * @brief Define a cor de preenchimento do círculo.
 * @param c Círculo.
 * @param cp Nova cor de preenchimento (string).
 */
void set_cpC(CIRCLE c, char* cp);

/**
 * @brief Obtém a largura da borda.
 * @param c Círculo.
 * @return Largura da borda.
 */
double get_strkWC(CIRCLE c);

/**
 * @brief Define a largura da borda.
 * @param c Círculo.
 * @param sw Nova largura da borda.
 */
void set_strkWC(CIRCLE c, double sw);

/**
 * @brief Obtém a área do círculo.
 * @param c Círculo.
 * @return Área do círculo.
 */
double get_areaC(CIRCLE c);

/**
 * @brief Obtém o raio do círculo.
 * @param c Círculo.
 * @return Raio do círculo.
 */
double get_rC(CIRCLE c);

/**
 * @brief Libera a memória ocupada por um círculo.
 * @param c Círculo.
 */
void kill_circ(CIRCLE c);

#endif