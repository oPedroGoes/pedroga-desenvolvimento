 #include <stdio.h>
 #include <stdlib.h> // Para malloc, free, strtol, strtof, strtod, exit, EXIT_SUCCESS, EXIT_FAILURE
 #include <assert.h>
 #include <string.h> // Para strcmp, strlen, strcpy, strcspn
 #include <errno.h>  // Para errno, ERANGE
 #include <limits.h> // Para INT_MAX, INT_MIN, LONG_MAX, LONG_MIN
 #include <math.h>   // Para HUGE_VALF

 #include "leituraTerminal.h"

 /* FALTA ATUALIZAR OS FULL NAMES. Falta decidir o que fazer com cada um dos full names.
 */

/* FREE EM:
* * todo diretório derivado de pathDir
* todo diretório derivado de pathArq
* prioMax, hc, promoRate (se alocados)
*/

 int main(int argc, char *argv[]){
  char *dirEntrada = NULL, *nomeGeo = NULL, *fullNomeGeo = NULL, *dirSaida = NULL,
       *nomeQry = NULL, *fullNomeQry = NULL, *fullNameArq = NULL; // Inicializado fullNameArq
  int *prioMax = NULL, *hc = NULL;
  double *promoRate = NULL;

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
        trataParamNumericoDouble(&promoRate, argv[i]);
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