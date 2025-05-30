#include "leituraTerminal.h"

#include <stdio.h>
#include <stdlib.h> // Para malloc, free, strtol, strtof, strtod, exit, EXIT_SUCCESS, EXIT_FAILURE
#include <assert.h>
#include <string.h> // Para strcmp, strlen, strcpy, strcspn
#include <errno.h>  // Para errno, ERANGE
#include <limits.h> // Para INT_MAX, INT_MIN, LONG_MAX, LONG_MIN
#include <math.h>   // Para HUGE_VALF

/* FREE EM:
* * todo diretório derivado de pathDir
* todo diretório derivado de pathArq
* prioMax, hc, promoRate (se alocados)
*/

void trataPath(char **pathDir, char* arg){ // arg é argv[i]
  int argLen = strlen(arg);

  *pathDir = (char*) malloc(sizeof(char)*(argLen + 1)); // +1 para o '\0'
  if (*pathDir == NULL){
    fprintf(stderr, "(trataPath) ERRO: falha ao alocar memoria para '%s'\n", arg);
    // Deixa *pathDir como NULL para ser checado em main
    return ;
  }

  // Modifica arg (argv[i]) se terminar com '/'
  if (argLen > 0 && arg[argLen - 1] == '/') {
    arg[argLen - 1] = '\0'; 
  }

  strcpy(*pathDir,arg); // Copia o arg (potencialmente modificado)
 }

 void trataNomeArquivo(char **pathArq, char* arg){ // arg é argv[i]
  int argLen = strlen(arg);

  *pathArq = (char*) malloc(sizeof(char)*(argLen+1)); // +1 para '\0'
  if (*pathArq == NULL){
    fprintf(stderr, "(trataNomeArquivo) ERRO: falha ao alocar memoria para '%s'.\n", arg);
    // Deixa *pathArq como NULL para ser checado em main
    return ;
  }

  // Modifica arg (argv[i]) se terminar com '/'
  if(argLen > 0 && arg[argLen-1] == '/'){ 
    arg[argLen-1] = '\0';
  }

  strcpy(*pathArq, arg); // Copia o arg (potencialmente modificado)
 }

void trataParamNumericoInt(int **num_ptr_addr, const char *param_str){ // param_str apenas leitura
  *num_ptr_addr = NULL; // Falha/não processado por padrão

  if (param_str == NULL) {
    fprintf(stderr, "Erro (trataParamNumericoInt): Parametro string fornecido e nulo.\n");
    // Deixa *param_str como NULL para ser checado em main
    return;
  }

  char *strEnd;
  errno = 0; // Resetar errno antes de chamar strtol
  long val = strtol(param_str, &strEnd, 10);

  if (strEnd == param_str) {
    fprintf(stderr, "(trataParamNumericoInt) Erro: Parametro '%s' nao contem digitos validos.\n", param_str);
    return;
  }
  if (*strEnd != '\0') {
    fprintf(stderr, "(trataParamNumericoInt) Erro: Caracteres invalidos ('%s') apos o numero em '%s'.\n", strEnd, param_str);
    return;
  }
  if ((errno == ERANGE && (val == LONG_MAX || val == LONG_MIN)) || (val > INT_MAX || val < INT_MIN)) {
    fprintf(stderr, "(trataParamNumericoInt) Erro: Valor '%s' fora do range para um inteiro.\n", param_str);
    return;
  }

  *num_ptr_addr = (int*) malloc(sizeof(int)); // Aloca espaço para um int
  if (*num_ptr_addr == NULL) {
    fprintf(stderr, "(trataParamNumericoInt) Erro: falha ao alocar memoria.\n");
    return;
  }
  **num_ptr_addr = (int)val; 
 }

void trataParamNumericoFloat(float **num_ptr_addr, const char* param_str){ // param_str apenas leitura
  *num_ptr_addr = NULL; // Falha/não processado por padrão

  if (param_str == NULL) {
    fprintf(stderr, "Erro (trataParamNumericoFloat): Parametro string fornecido e nulo.\n");
    // Deixa *param_str como NULL para ser checado em main
    return;
  }

  char *strEnd;
  errno = 0; // Resetar errno antes de chamar strtof
  float val = strtof(param_str, &strEnd); // Usar strtof para float

  if (strEnd == param_str) {
    fprintf(stderr, "(trataParamNumericoFloat) Erro : Parametro '%s' nao contem digitos validos.\n", param_str);
    return;
  }
  if (*strEnd != '\0') {
    fprintf(stderr, "(trataParamNumericoFloat) Erro: Caracteres invalidos ('%s') apos o numero em '%s'.\n", strEnd, param_str);
    return;
  }
  if (errno == ERANGE && (val == HUGE_VALF || val == -HUGE_VALF || val == 0.0f)) {
    fprintf(stderr, "(trataParamNumericoFloat) Erro: Valor '%s' fora do range para float ou underflow para zero.\n", param_str);
    return;
  }

  *num_ptr_addr = (float*) malloc(sizeof(float)); // Aloca espaço para um float
  if (*num_ptr_addr == NULL) {
    fprintf(stderr, "(trataParamNumericoFloat) Erro: falha ao alocar memoria.\n");
    return;
  }
  **num_ptr_addr = val;
 }

void trataParamNumericoDouble(double **num_ptr_addr, const char* param_str) {
    *num_ptr_addr = NULL; // Falha/não processado por padrão

    if (param_str == NULL) {
        fprintf(stderr, "Erro (trataParamNumericoDouble): Parametro string fornecido e nulo.\n");
        // Deixa *param_str como NULL para ser checado em main
        return;
    }

    char *strEnd;
    errno = 0; // Resetar errno antes de chamar strtod
    double val = strtod(param_str, &strEnd);

    // Verificar se a conversão foi bem-sucedida
    if (strEnd == param_str) {
        fprintf(stderr, "(trataParamNumericoDouble) Erro: Parametro '%s' nao contem digitos validos.\n", param_str);
        return;
    }

    if (*strEnd != '\0') {
        fprintf(stderr, "(trataParamNumericoDouble) Erro: Caracteres invalidos ('%s') apos o numero em '%s'.\n", strEnd, param_str);
        return;
    }

    if (errno == ERANGE && (val == HUGE_VAL || val == -HUGE_VAL || val == 0.0)) {
        fprintf(stderr, "(trataParamNumericoDouble) Erro: Valor '%s' fora do range para double ou underflow para zero.\n", param_str);
        return;
    }

    *num_ptr_addr = (double*) malloc(sizeof(double)); // Aloca espaço para um double
    if (*num_ptr_addr == NULL) {
        fprintf(stderr, "(trataParamNumericoDouble) ERRO: falha ao alocar memoria para o double.\n");
        return;
    }
    **num_ptr_addr = val;
}

void completaNomeArquivo(char *pathDir, char *nomeArq, char **fullNomeArq){
    int fullNomeLen = strlen(pathDir) + 1 + strlen(nomeArq);
    
    *fullNomeArq = (char*)malloc(sizeof(char)*(fullNomeLen+1));
    if(fullNomeArq == NULL){
      fprintf(stderr, "(trataPath) Erro: falha ao alocar memoria para '%s'\n", nomeArq);
    // Deixa *fullNomeArq como NULL para ser checado em main
    return ;
    }
    
    strcpy(*fullNomeArq, pathDir);
    strcat(*fullNomeArq, "/");
    strcat(*fullNomeArq, nomeArq);
  }

void trataArqTxt(char *pathOut, char *nomeGeo, char *nomeQry, char **arqTxt){
    int lenGeo = strlen(nomeGeo);
    int lenQry = strlen(nomeQry);
    
    char buff1[lenGeo];
    char buff2[lenQry];

    strcpy(buff1, nomeGeo);
    strcpy(buff2, nomeQry);
    
    buff1[strlen(buff1)-4] = '\0';
    buff2[strlen(buff2)-4] = '\0';

    int lenArqTxt = strlen(pathOut) + 1 + strlen(buff1) + 1 + strlen(buff2) + 4;

    *arqTxt = (char*)malloc(sizeof(char)*(lenArqTxt + 1));
    if (arqTxt == NULL){
      fprintf(stderr, "(trataArqTxt) ERRO: falha ao alocar memoria para '%s'\n", *arqTxt);
    // Deixa *pathDir como NULL para ser checado em main
    return ;
    }

    strcpy(*arqTxt, pathOut); 
    strcat(*arqTxt, "/");
    strcat(*arqTxt, buff1);
    strcat(*arqTxt, "-");
    strcat(*arqTxt, buff2);
    strcat(*arqTxt, ".txt");
}