#include "leituraTerminal.h"

#include <stdio.h>
#include <stdlib.h> // Para malloc, free, strtol, strtof, strtod, exit, EXIT_SUCCESS, EXIT_FAILURE
#include <assert.h>
#include <string.h> // Para strcmp, strlen, strcpy, strcspn
#include <errno.h>  // Para errno, ERANGE
#include <limits.h> // Para INT_MAX, INT_MIN, LONG_MAX, LONG_MIN
#include <math.h>   // Para HUGE_VALF

void trataPath(char **pathDir, char* arg){ // arg é argv[i]
  if(!arg || !pathDir){
    fprintf(stderr, "(trataPath) Erro: parametros invalidos.\n");
    return;
  }
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
  if(!arg || !pathArq){
    fprintf(stderr, "(trataNomeArquivo) Erro: parametros invalidos.\n");
    return;
  }
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
  if(!num_ptr_addr || !param_str){
    fprintf(stderr, "(trataParamNumericoInt) Erro: parametros invalidos.\n");
    return;
  }
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
  if(!num_ptr_addr || !param_str){
    fprintf(stderr, "(trataParamNumericoFloat) Erro: parametros invalidos.\n");
    return;
  }
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

void trataParamNumericoDouble(double **num_ptr_addr, const char* param_str){
  if(!num_ptr_addr || !param_str){
    fprintf(stderr, "(trataParamNumericoDouble) Erro: parametros invalidos.\n");
    return;
  }
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

void completaNomeGeo(char *pathDir, char *nomeBaseArq, char **fullNomeArq){
  if(!nomeBaseArq || !fullNomeArq){
    fprintf(stderr, "(completaNomeArquivo) Erro: parametros invalidos. \n");
    return;
  }

  // Caso nao seja especificado o BED, tomamos o diretorio atual.
  const char *absolut = ".";
  if(!pathDir){
    pathDir = (char*)malloc(sizeof(char)*2);  // "." + '\0'.
    strcpy(pathDir, absolut);
  }

  int fullNomeLen = strlen(pathDir) + 1 + strlen(nomeBaseArq) + 4;  // pathDir + "/" + nomeBase + ".geo"
  
  *fullNomeArq = (char*)malloc(sizeof(char)*(fullNomeLen+1));
  if(fullNomeArq == NULL){
    fprintf(stderr, "(trataPath) Erro: falha ao alocar memoria para '%s'\n", nomeBaseArq);
  // Deixa *fullNomeArq como NULL para ser checado em main
  return ;
  }
  
  // Concatena as strings corretamente.
  sprintf(*fullNomeArq, "%s/%s.geo", pathDir, nomeBaseArq);
}

void completaNomeQry(char *pathDir, char *nomeBaseGeo, char *nomeBaseQry, char **fullNomeArq){
  if(!pathDir || !nomeBaseGeo || !nomeBaseQry || !fullNomeArq){
    fprintf(stderr, "(completaNomeArquivo) Erro: parametros invalidos. \n");
    return;
  }

  // pathDir + "/" + nomeBaseGeo + "/" + nomeBaseQry + ".qry"
  int fullNomeLen = strlen(pathDir) + 1 + strlen(nomeBaseGeo) + 1 + strlen(nomeBaseQry) + 4;

  *fullNomeArq = (char*)malloc(sizeof(char)*(fullNomeLen+1));
  if(fullNomeArq == NULL){
    fprintf(stderr, "(trataPath) Erro: falha ao alocar memoria para '%s'\n", nomeBaseQry);
  // Deixa *fullNomeArq como NULL para ser checado em main
  return ;
  }
  
  // Concatena as strings corretamente.
  sprintf(*fullNomeArq, "%s/%s/%s.qry", pathDir, nomeBaseGeo, nomeBaseQry);
}

int removeExtensaoArq(const char* src, char** dest) {
    if (src == NULL || dest == NULL) {
        return -1;
    }
    // Inicializa o ponteiro de destino como NULL por segurança.
    *dest = NULL;

    size_t len = strlen(src);
    int codigo_retorno = -1;
    size_t len_sem_extensao = 0;

    // Verifica qual extensão está presente, se houver
    if (len >= 4 && strcmp(src + len - 4, ".geo") == 0) {
        codigo_retorno = 0; // .geo
        len_sem_extensao = len - 4;
    } else if (len >= 4 && strcmp(src + len - 4, ".qry") == 0) {
        codigo_retorno = 1; // .qry
        len_sem_extensao = len - 4;
    }

    // Se uma extensão válida foi encontrada, cria a nova string
    if (codigo_retorno != -1) {
        // Aloca memória para a string base + 1 para o caractere nulo ('\0')
        *dest = malloc(len_sem_extensao + 1);
        if (*dest == NULL) {
            // Falha na alocação de memória é um erro crítico
            fprintf(stderr, "ERRO: Falha ao alocar memoria em remove_extensao_e_copia.\n");
            return -1;
        }

        // Copia apenas a parte do nome do arquivo, sem a extensão
        strncpy(*dest, src, len_sem_extensao);
        // Garante que a nova string é terminada corretamente
        (*dest)[len_sem_extensao] = '\0';
    }

    // Retorna o código do resultado
    return codigo_retorno;
}

void trataArqDot(const char *pathDir, const char *nomeBaseGeo, const char *nomeBaseQry, char **arqDot) {
    if (!pathDir || !nomeBaseGeo || !nomeBaseQry || !arqDot) return;

    // pathDir + "/" + nomeBaseGeo + "-" + nomeBaseQry + ".dot"
    int len = strlen(pathDir) + 1 + strlen(nomeBaseGeo) + 1 + strlen(nomeBaseQry) + 4;
    *arqDot = (char*) malloc(len + 1);
    if (*arqDot) {
        sprintf(*arqDot, "%s/%s-%s.dot", pathDir, nomeBaseGeo, nomeBaseQry);
    }
}
