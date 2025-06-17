#ifndef TEXTO_H
#define TEXTO_H

/**
 * @file texto.h
 * @brief Interface do TAD TEXTO para manipulação de textos no plano 2D.
 *
 * Este módulo define o tipo abstrato de dados TEXTO e fornece funções para criar,
 * acessar e modificar textos, incluindo atributos como posição, cores, âncora,
 * conteúdo textual e propriedades de fonte. Também permite calcular a área do texto
 * e liberar a memória alocada.
 */

typedef void* TEXTO;

/**
 * @brief Cria um novo texto.
 * @param id Identificador do texto.
 * @param x Coordenada x da ancora do texto.
 * @param y Coordenada y da ancora do texto.
 * @param cb Cor da borda (string).
 * @param cp Cor de preenchimento (string).
 * @param a Caractere indicando a âncora do texto ('i', 'm' ou 'f').
 * @param txt Conteúdo textual.
 * @param ff Família da fonte (string).
 * @param fw Peso da fonte (string).
 * @param fs Tamanho da fonte (string).
 * @return Ponteiro para o texto criado.
 */
TEXTO cria_texto(int id, double x, double y, char* cb, char* cp, char a, char* txt, char* ff, char* fw, char* fs);

/**
 * @brief Obtém o identificador do texto.
 * @param t Estrutura Texto.
 * @return Identificador.
 */
int get_idT(TEXTO t);

/**
 * @brief Obtém a coordenada x do texto.
 * @param t Estrutura Texto.
 * @return Coordenada x.
 */
double get_XT(TEXTO t);

/**
 * @brief Obtém a coordenada y do texto.
 * @param t Estrutura Texto.
 * @return Coordenada y.
 */
double get_YT(TEXTO t);

/**
 * @brief Obtém a cor da borda do texto.
 * @param t Estrutura Texto.
 * @return String com a cor da borda.
 */
char* get_cbT(TEXTO t);

/**
 * @brief Obtém a cor de preenchimento do texto.
 * @param t Estrutura Texto.
 * @return String com a cor de preenchimento.
 */
char* get_cpT(TEXTO t);

/**
 * @brief Obtém a âncora do texto para uso em SVG ("start", "middle" ou "end").
 * @param t Estrutura Texto.
 * @return String indicando a âncora.
 */
char* get_ancoraT(TEXTO t);

/**
 * @brief Obtém o caractere da âncora do texto ('i', 'm' ou 'f').
 * @param t Estrutura Texto.
 * @return Caractere da âncora.
 */
char get_charancoraT(TEXTO t);

/**
 * @brief Obtém o conteúdo textual.
 * @param t Estrutura Texto.
 * @return String com o conteúdo textual.
 */
char* get_txtT(TEXTO t);

/**
 * @brief Obtém a família da fonte.
 * @param t Estrutura Texto.
 * @return String com a família da fonte.
 */
char* get_ffT(TEXTO t);

/**
 * @brief Obtém o tamanho da fonte.
 * @param t Estrutura Texto.
 * @return String com o tamanho da fonte.
 */
char* get_fsT(TEXTO t);

/**
 * @brief Obtém o peso da fonte.
 * @param t Estrutura Texto.
 * @return String com o peso da fonte.
 */
char* get_fwT(TEXTO t);

/**
 * @brief Obtém a largura da borda do texto.
 * @param t Estrutura Texto.
 * @return Largura da borda.
 */
double get_strkWT(TEXTO t);

/**
 * @brief Define a largura da borda do texto.
 * @param t Estrutura Texto.
 * @param sw Nova largura da borda.
 */
void set_strkWT(TEXTO t, double sw);

/**
 * @brief Obtém a área do texto (valor simbólico, relacionado ao comprimento do texto).
 * @param t Estrutura Texto.
 * @return Área.
 */
double get_areaT(TEXTO t);

/**
 * @brief Obtém o número de caracteres do texto.
 * @param t Estrutura Texto.
 * @return Número de caracteres.
 */
int tamanho_dotextoT(TEXTO t);

/**
 * @brief Define o identificador do texto.
 * @param t Estrutura Texto.
 * @param id Novo identificador.
 */
void set_idT(TEXTO t, int id);

/**
 * @brief Define a coordenada x do texto.
 * @param t Estrutura Texto.
 * @param x Nova coordenada x.
 */
void set_xT(TEXTO t, double x);

/**
 * @brief Define a coordenada y do texto.
 * @param t Estrutura Texto.
 * @param y Nova coordenada y.
 */
void set_yT(TEXTO t, double y);

/**
 * @brief Define a cor da borda do texto.
 * @param t Estrutura Texto.
 * @param cb Nova cor da borda.
 */
void set_cbT(TEXTO t, char* cb);

/**
 * @brief Define a cor de preenchimento do texto.
 * @param t Estrutura Texto.
 * @param cp Nova cor de preenchimento.
 */
void set_cpT(TEXTO t, char* cp);

/**
 * @brief Libera a memória ocupada pelo texto.
 * @param t Estrutura Texto.
 */
void kill_texto(TEXTO t);
#endif