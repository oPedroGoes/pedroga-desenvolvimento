#ifndef RETANGULO_H
#define RETANGULO_H

/**
 * @file retangulo.h
 * @brief Interface do TAD RETANGULO para manipulação de retângulos geométricos.
 *
 * Este módulo define o tipo abstrato de dados RETANGULO e fornece funções para criar,
 * acessar e modificar propriedades de retângulos, como coordenadas, dimensões,
 * cores e largura da borda. Também permite calcular a área do retângulo e liberar
 * a memória alocada.
 */

typedef void* RECTANGLE;

/**
 * @brief Cria um novo retângulo.
 * @param id Identificador do retângulo.
 * @param x Coordenada x da ancora do retangulo. (canto superior esquerdo na tela)
 * @param y Coordenada y da ancora do retangulo.
 * @param w Largura do retângulo.
 * @param h Altura do retângulo.
 * @param cb Cor da borda (string).
 * @param cp Cor de preenchimento (string).
 * @return Ponteiro para o retângulo criado.
 */
RECTANGLE create_rectangle(int id, double x, double y, double w, double h, char* cb, char* cp);

/**
 * @brief Obtém o identificador do retângulo.
 * @param r Retângulo.
 * @return Identificador.
 */
int get_idR(RECTANGLE r);

/**
 * @brief Define o identificador do retângulo.
 * @param r Retângulo.
 * @param id Novo identificador.
 */
void set_idR(RECTANGLE r, int id);

/**
 * @brief Obtém a coordenada x da ancora do retangulo.
 * @param r Retângulo.
 * @return Coordenada x.
 */
double get_XR(RECTANGLE r);

/**
 * @brief Define a coordenada x da ancora do retangulo.
 * @param r Retângulo.
 * @param x Nova coordenada x.
 */
void set_xR(RECTANGLE r, double x);

/**
 * @brief Obtém a coordenada y da ancora do retangulo.
 * @param r Retângulo.
 * @return Coordenada y.
 */
double get_YR(RECTANGLE r);

/**
 * @brief Define a coordenada y da ancora do retangulo.
 * @param r Retângulo.
 * @param y Nova coordenada y.
 */
void set_yR(RECTANGLE r, double y);

/**
 * @brief Obtém a cor da borda do retângulo.
 * @param r Retângulo.
 * @return String com a cor da borda.
 */
char* get_cbR(RECTANGLE r);

/**
 * @brief Define a cor da borda do retângulo.
 * @param r Retângulo.
 * @param cb Nova cor da borda (string).
 */
void set_cbR(RECTANGLE r, char* cb);

/**
 * @brief Obtém a cor de preenchimento do retângulo.
 * @param r Retângulo.
 * @return String com a cor de preenchimento.
 */
char* get_cpR(RECTANGLE r);

/**
 * @brief Define a cor de preenchimento do retângulo.
 * @param r Retângulo.
 * @param cp Nova cor de preenchimento (string).
 */
void set_cpR(RECTANGLE r, char* cp);

/**
 * @brief Obtém a largura da borda do retângulo.
 * @param r Retângulo.
 * @return Largura da borda.
 */
double get_strkWR(RECTANGLE r);

/**
 * @brief Define a largura da borda do retângulo.
 * @param r Retângulo.
 * @param sw Nova largura da borda.
 */
void set_strkWR(RECTANGLE r, double sw);

/**
 * @brief Obtém a largura do retângulo.
 * @param r Retângulo.
 * @return Largura.
 */
double get_wR(RECTANGLE r);

/**
 * @brief Obtém a altura do retângulo.
 * @param r Retângulo.
 * @return Altura.
 */
double get_hR(RECTANGLE r);

/**
 * @brief Calcula a área do retângulo.
 * @param r Retângulo.
 * @return Área.
 */
double get_areaR(RECTANGLE r);

/**
 * @brief Libera a memória ocupada pelo retângulo.
 * @param r Retângulo.
 */
void kill_rectangle(RECTANGLE r);

#endif