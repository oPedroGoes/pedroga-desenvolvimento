#ifndef LEITURATERMINAL_H
#define LEITURATERMINAL_H

/**
 * Processa um argumento de string que representa um caminho de diretório.
 * Aloca memória para o caminho processado e remove uma barra '/' no final do argumento original 'arg' (argv[i]), se existir.
 * 
 *  @param pathDir (dest) referencia para o ponteiro que se deseja armazenar o path.
 *  @param arg (src) conteudo que sera tratado. Derivado de um parametro do terminal.
 * 
 *  @return O ponteiro '*pathDir' apontará para a nova string alocada ou para NULL em caso de falha na alocação.
 */
void trataPath(char **pathDir, char* arg);

/**
 * Processa um argumento de string que representa um nome de arquivo (possivelmente com caminho).
 * Aloca memória para o nome processado e remove uma barra '/' no final do argumento original 'arg' (argv[i]), se existir.
 * O ponteiro '*pathArq' apontará para a nova string alocada ou para NULL em caso de falha na alocação.
 */
void trataNomeArquivo(char **pathArq, char* arg);

/**
 * Converte uma string de parâmetro ('param_str') em um valor inteiro.
 * Aloca memória para o inteiro convertido e faz '*num_ptr_addr' apontar para ele.
 * Em caso de erro de conversão, formato inválido, valor fora do range de int, ou falha de alocação, '*num_ptr_addr' é definido como NULL e uma mensagem de erro é impressa em stderr.
 * 
 * @param num_ptr_addr (dest) referencia do lugar onde se armazenara' o parametro tratado
 * @param param_str (src) conteudo que sera tratado.
 */
void trataParamNumericoInt(int **num_ptr_addr, const char *param_str);

/**
 * Converte uma string de parâmetro ('param_str') em um valor float.
 * Aloca memória para o float convertido e faz '*num_ptr_addr' apontar para ele.
 * Em caso de erro de conversão, formato inválido, valor fora do range de float, ou falha de alocação, '*num_ptr_addr' é definido como NULL e uma mensagem de erro é impressa em stderr.
 * 
 * @param num_ptr_addr (dest) referencia do lugar onde se armazenara' o parametro tratado
 * @param param_str (src) conteudo que sera tratado.
 */
void trataParamNumericoFloat(float **num_ptr_addr, const char* param_str);

/**
 * Converte uma string de parâmetro ('param_str') em um valor double.
 * Aloca memória para o double convertido e faz '*num_ptr_addr' apontar para ele.
 * Em caso de erro de conversão, formato inválido, valor fora do range de double, ou falha de alocação, '*num_ptr_addr' é definido como NULL e uma mensagem de erro é impressa em stderr.
 * 
 * @param num_ptr_addr (dest) referencia do lugar onde se armazenara' o parametro tratado
 * @param param_str (src) conteudo que sera tratado.
 */
void trataParamNumericoDouble(double **num_ptr_addr, const char* param_str);

/**
*  @brief Completa o caminho que se deve percorrer para abrir ou criar um arquivo geo.
* 
*  @param pathDir (src) diretorio base que se trabalha
*  @param nomeArq (src) nome do arquivo que deseja tratar
*  @param fullNomeArq (dest) referencia para o destino do path
* 
*/
void completaNomeGeo(char *pathDir, char *nomeBaseArq, char **fullNomeArq);

/**
*  @brief Completa o caminho que se deve percorrer para abrir ou criar um arquivo geo.
* 
*  @param pathDir (src) diretorio base que se trabalha
*  @param nomeBaseGeo (src) nome base do arquivo geo do qual se deriva o qry
*  @param nomeBaseQry (src) nome do arquivo que deseja tratar
*  @param fullNomeArq (dest) referencia para o destino do path
* 
*/
void completaNomeQry(char *pathDir, char *nomeBaseGeo, char *nomeBaseQry, char **fullNomeArq);

  /**
 * @brief Cria uma nova string sem a extensão .geo ou .qry.
 * A função não modifica a string de entrada. A memória para a nova
 * string de destino é alocada dinamicamente e deve ser libertada
 * pelo chamador usando free().
 * 
 *   @param src A string fonte original (não será modificada).
 *   @param dest Um ponteiro para um char*, onde o endereço da nova string alocada será armazenado.
 *   @return int Retorna 0 se a extensão .geo foi encontrada,
 *           1 se a extensão .qry foi encontrada,
 *          -1 se nenhuma extensão foi encontrada ou em caso de erro.
 */
int removeExtensaoArq(const char *src, char **dest);

/**
 * @brief Constrói o nome completo para o arquivo .dot.
 * @param pathDir Diretório de saída.
 * @param nomeBaseGeo Nome base do arquivo .geo.
 * @param arqDot Ponteiro para a string que armazenará o caminho completo.
 */
void trataArqDot(char *pathDir, char *nomeBaseGeo, char *nomeBaseQry, char **arqDot);

#endif