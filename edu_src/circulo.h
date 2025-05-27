#ifndef circulo_h
#define circulo_h
 

/*

O arquivo linha.h define a interface para manipulação de uma estrutura de dados que representa uma linha em um plano bidimensional. Este header fornece funções para criar, acessar, modificar e liberar recursos relacionados a uma linha, seguindo os princípios de encapsulamento e modularidade.

A estrutura da linha é composta por um identificador único (ID), coordenadas dos pontos inicial 
 uma cor associada à linha. O header utiliza um tipo opaco (LINHA), que é um ponteiro para void, para esconder os detalhes de implementação da estrutura interna da linha. Isso garante que o usuário da biblioteca não precise conhecer os detalhes internos da estrutura, promovendo um uso mais seguro e modular.
As funções fornecidas permitem:

Criar uma nova linha com parâmetros específicos, incluindo coordenadas, identificador e cor.

Acessar atributos como ID, coordenadas dos pontos, cor e uma medida de "área" (comprimento da linha multiplicado por 1.5).

Modificar atributos como coordenadas dos pontos, cor e identificador.

Liberar a memória alocada para a linha quando ela não for mais necessária, garantindo o gerenciamento adequado de recursos.

Este header é útil em aplicações que envolvem geometria básica, gráficos 2D ou sistemas de desenho, fornecendo uma abstração simples e eficiente para a manipulação de linhas.

*/


// Definição do tipo CIRCULO como um ponteiro para void
typedef void* CIRCULO;

/**
 * @brief Cria uma estrutura de círculo com os parâmetros fornecidos.
 * 
 * @param i ID do círculo.
 * @param x Coordenada x do centro do círculo.
 * @param y Coordenada y do centro do círculo.
 * @param r Raio do círculo.
 * @param cb Cor da borda do círculo.
 * @param cp Cor de preenchimento do círculo.
 * @return CIRCULO Retorna um ponteiro para a estrutura do círculo criada.
 */
CIRCULO cria_circulo(int i, float x, float y, float r, char* cb, char* cp);

/**
 * @brief Obtém o ID do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @return int Retorna o ID do círculo.
 */
int get_idC(CIRCULO c);

void set_idC(CIRCULO c, int i);

/**
 * @brief Obtém a coordenada x do centro do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @return float Retorna a coordenada x do centro do círculo.
 */
float get_XC(CIRCULO c);

/**
 * @brief Obtém a coordenada y do centro do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @return float Retorna a coordenada y do centro do círculo.
 */
float get_YC(CIRCULO c);

/**
 * @brief Obtém o raio do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @return float Retorna o raio do círculo.
 */
float get_rC(CIRCULO c);

/**
 * @brief Obtém a cor da borda do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @return char* Retorna a cor da borda do círculo.
 */
char* get_cbC(CIRCULO c);

/**
 * @brief Obtém a cor de preenchimento do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @return char* Retorna a cor de preenchimento do círculo.
 */
char* get_cpC(CIRCULO c);

/**
 * @brief Calcula e retorna a área do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @return float Retorna a área do círculo.
 */
float get_areaC(CIRCULO c);

/**
 * @brief Define a coordenada x do centro do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @param x Nova coordenada x do centro do círculo.
 */
void set_xC(CIRCULO c, float x);

/**
 * @brief Define a coordenada y do centro do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @param y Nova coordenada y do centro do círculo.
 */
void set_yC(CIRCULO c, float y);

/**
 * @brief Define a cor da borda do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @param cb Nova cor da borda do círculo.
 */
void set_cbC(CIRCULO c, char* cb);

/**
 * @brief Define a cor de preenchimento do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 * @param cp Nova cor de preenchimento do círculo.
 */
void set_cpC(CIRCULO c, char* cp);

/**
 * @brief Libera a memória alocada para a estrutura do círculo.
 * 
 * @param c Ponteiro para a estrutura do círculo.
 */
void kill_circ(CIRCULO c);

#endif