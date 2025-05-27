#include <stdio.h>
 #include <stdlib.h> // Para malloc, free, strtol, strtof, strtod, exit, EXIT_SUCCESS, EXIT_FAILURE
 #include <assert.h>
 #include <string.h> // Para strcmp, strlen, strcpy, strcspn
 #include <errno.h>  // Para errno, ERANGE
 #include <limits.h> // Para INT_MAX, INT_MIN, LONG_MAX, LONG_MIN
 #include <math.h>   // Para HUGE_VALF

 /* Acabei de corrigir a logica para printar, guardar e modificar os parametros passados por linha. FALTAM OS PARAMETROS DE ARVORE. FALTA ATUALIZAR O FULL NAME ARQ.
 */

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
  errno = 0; // Resetar errno
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

  *num_ptr_addr = (float*) malloc(sizeof(float)); // Aloca espaço para UM float
  if (*num_ptr_addr == NULL) {
    fprintf(stderr, "(trataParamNumericoFloat) Erro: falha ao alocar memoria.\n");
    return;
  }
  **num_ptr_addr = val;
 }

 int main(int argc, char *argv[]){
  char *dirEntrada = NULL, *nomeGeo = NULL, *fullNomeGeo = NULL, *dirSaida = NULL,
       *nomeQry = NULL, *fullNomeQry = NULL, *fullNameArq = NULL; // Inicializado fullNameArq
  int *prioMax = NULL, *hc = NULL;
  float *promoRate = NULL;

  FILE *f = NULL; // Se for usar, lembre de fclose(f)
  
  /* MOSTRA OS PARAMETROS */
  printf("Numero de argumentos: %d\n", argc);
  for(int k= 0; k< argc; k++){ // Usei k para não conflitar com i abaixo
    printf("argv[%d] = %s\n",k,argv[k]);
  }
  printf("\n");

  int i = 1;
  while (i < argc){
    if (strcmp(argv[i], "-e") == 0){
        printf("\n lendo parametro -e... ");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -e."); 
          goto frees_and_exit;
        }
        trataPath(&dirEntrada, argv[i]);
        if(dirEntrada == NULL){ 
          fprintf(stderr, "\nFalha ao processar -e.\n"); 
          goto frees_and_exit;
        }
    }
    else if (strcmp(argv[i], "-f") == 0){
        printf("\n lendo parametro -f... ");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -f.");
          goto frees_and_exit;
        }
        trataNomeArquivo(&nomeGeo, argv[i]);
        if(nomeGeo == NULL) {
          fprintf(stderr, "\nFalha ao processar -f.\n");
          goto frees_and_exit;
        }
    } else if (strcmp(argv[i], "-q") == 0){
        printf("\n lendo parametro -q... ");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -q.");
          goto frees_and_exit;
        }
        trataNomeArquivo(&nomeQry, argv[i]);
        if(nomeQry == NULL) {
          fprintf(stderr, "\nFalha ao processar -q.\n");
          goto frees_and_exit;
        }
    } else if (strcmp(argv[i], "-o") == 0){
        printf("\n lendo parametro -o... ");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -o.");
          goto frees_and_exit;
        }
        trataPath(&dirSaida, argv[i]);
        if(dirSaida == NULL) {
          fprintf(stderr, "\nFalha ao processar -o.\n");
          goto frees_and_exit;
        }
    } else if (strcmp(argv[i], "-p") == 0){ // prioMax (int)
        printf("\n lendo parametro -p (prioMax)... ");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -p.");
          goto frees_and_exit;
        }
        trataParamNumericoInt(&prioMax, argv[i]);
        if(prioMax == NULL) {
          fprintf(stderr, "\nFalha ao processar -p (prioMax).\n");
          /* Pode decidir sair ou continuar com prioMax=NULL */
        }
      } else if (strcmp(argv[i], "-hc") == 0){ // hc (int)
        printf("\n lendo parametro -hc... ");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -hc.");
          goto frees_and_exit;
        }
        trataParamNumericoInt(&hc, argv[i]);
        if(hc == NULL) {
          fprintf(stderr, "\nFalha ao processar -hc.\n");
        }
      } else if (strcmp(argv[i], "-pr") == 0){ // promoRate (float)
        printf("\n lendo parametro -pr (promoRate)... ");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -pr.");
          goto frees_and_exit;
        }
        trataParamNumericoFloat(&promoRate, argv[i]);
        if(promoRate == NULL) {
          fprintf(stderr, "\nFalha ao processar -pr (promoRate).\n");
        }
      } else {
        printf("\nParametro desconhecido: %s\n", argv[i]);
        // Você pode querer sair aqui se um parâmetro desconhecido for um erro fatal
        // goto frees_and_exit; 
      }
    i++;
  } 
  //while.

  printf("\n--- Valores Processados ---\n");
  printf("dirEntrada = %s\n", dirEntrada ? dirEntrada : "(null ou falha)");
  printf("dirSaida   = %s\n", dirSaida   ? dirSaida   : "(null ou falha)");
  printf("nomeGeo    = %s\n", nomeGeo    ? nomeGeo    : "(null ou falha)");
  printf("nomeQry    = %s\n", nomeQry    ? nomeQry    : "(null ou falha)");

  if (prioMax != NULL) {
    printf("prioMax = %d\n", *prioMax);
  } else {
    printf("prioMax = (nao definido ou falha)\n");
  }
  if (hc != NULL) {
    printf("hc = %d\n", *hc);
  } else {
    printf("hc = (nao definido ou falha)\n");
  }
  if (promoRate != NULL) {
    printf("promoRate = %f\n", *promoRate);
  } else {
    printf("promoRate = (nao definido ou falha)\n");
  }

frees_and_exit: // Rótulo para liberar memória antes de sair em caso de erro
  // Liberar memória
  if(dirEntrada) free(dirEntrada);
  if(nomeGeo) free(nomeGeo);
  // if(fullNomeGeo) free(fullNomeGeo); // Se você alocar memória para eles
  if(dirSaida) free(dirSaida);
  if(nomeQry) free(nomeQry);
  // if(fullNomeQry) free(fullNomeQry);
  // if(fullNameArq) free(fullNameArq);

  if(prioMax) free(prioMax);
  if(hc) free(hc);
  if(promoRate) free(promoRate);

  return 0;
 }