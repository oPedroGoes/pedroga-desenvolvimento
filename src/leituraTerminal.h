#ifndef leituraTerminal_h
#define leituraTerminal_h

/*
 * Processa um argumento de string que representa um caminho de diretório.
 * Aloca memória para o caminho processado e remove uma barra '/' no final do argumento original 'arg' (argv[i]), se existir.
 * O ponteiro '*pathDir' apontará para a nova string alocada ou para NULL em caso de falha na alocação.
 */
void trataPath(char **pathDir, char* arg);

/*
 * Processa um argumento de string que representa um nome de arquivo (possivelmente com caminho).
 * Aloca memória para o nome processado e remove uma barra '/' no final do argumento original 'arg' (argv[i]), se existir.
 * O ponteiro '*pathArq' apontará para a nova string alocada ou para NULL em caso de falha na alocação.
 */
void trataNomeArquivo(char **pathArq, char* arg);

/*
 * Converte uma string de parâmetro ('param_str') em um valor inteiro.
 * Aloca memória para o inteiro convertido e faz '*num_ptr_addr' apontar para ele.
 * Em caso de erro de conversão, formato inválido, valor fora do range de int, ou falha de alocação, '*num_ptr_addr' é definido como NULL e uma mensagem de erro é impressa em stderr.
 */
void trataParamNumericoInt(int **num_ptr_addr, const char *param_str);

/*
 * Converte uma string de parâmetro ('param_str') em um valor float.
 * Aloca memória para o float convertido e faz '*num_ptr_addr' apontar para ele.
 * Em caso de erro de conversão, formato inválido, valor fora do range de float, ou falha de alocação, '*num_ptr_addr' é definido como NULL e uma mensagem de erro é impressa em stderr.
 */
void trataParamNumericoFloat(float **num_ptr_addr, const char* param_str);

#endif