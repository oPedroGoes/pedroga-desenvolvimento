#ifndef leituraTerminal_h
#define leituraTerminal_h

void trataPath(char **pathDir, char* arg);

void trataNomeArquivo(char **pathArq, char* arg);

void trataParamNumericoInt(int **num_ptr_addr, const char *param_str);

void trataParamNumericoFloat(float **num_ptr_addr, const char* param_str);

#endif