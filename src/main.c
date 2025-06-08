 #include <stdio.h>
 #include <stdlib.h> // Para malloc, free, strtol, strtof, strtod, exit, EXIT_SUCCESS, EXIT_FAILURE
 #include <assert.h>
 #include <string.h> // Para strcmp, strlen, strcpy, strcspn
 #include <errno.h>  // Para errno, ERANGE
 #include <limits.h> // Para INT_MAX, INT_MIN, LONG_MAX, LONG_MIN
 #include <math.h>   // Para HUGE_VALF
 #include <time.h> // Para srand()
 
 #include "processaGeo.h"
 #include "processaQry.h"
 #include "leituraTerminal.h"
 #include "Lista.h"
 #include "SmuTreap.h"

#define EPSILON_CONFIG 0.001 //AJUSTAVEL.

 /* Falta decidir o que fazer com cada um dos full names.
 */

/* FREE EM:
* * todo diretório derivado de pathDir
* todo diretório derivado de pathArq
* prioMax, hc, promoRate (se alocados)
*/

 int main(int argc, char *argv[]){
  char *dirEntrada = NULL, *dirSaida = NULL, *nomeGeo = NULL, *fullNomeGeo = NULL,
       *nomeQry = NULL, *fullNomeQry = NULL; char *fullNomeTxt = NULL;

  srand(time(NULL));
  int *prioMax = NULL, *hc = NULL;
  double *promoRate = NULL;

  int i = 1;
  while (i < argc){
    if (strcmp(argv[i], "-e") == 0){
        printf("lendo parametro -e...\n");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -e.\n"); 
          goto frees_and_exit;
        }
        trataPath(&dirEntrada, argv[i]);
        if(dirEntrada == NULL){ 
          fprintf(stderr, "\nFalha ao processar -e.\n"); 
          goto frees_and_exit;
        }
    }
    else if (strcmp(argv[i], "-f") == 0){
        printf("lendo parametro -f...\n");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -f.\n");
          goto frees_and_exit;
        }
        trataNomeArquivo(&nomeGeo, argv[i]);
        if(nomeGeo == NULL) {
          fprintf(stderr, "\nFalha ao processar -f.\n");
          goto frees_and_exit;
        }
    } else if (strcmp(argv[i], "-q") == 0){
        printf("lendo parametro -q...\n");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -q.\n");
          goto frees_and_exit;
        }
        trataNomeArquivo(&nomeQry, argv[i]);
        if(nomeQry == NULL) {
          fprintf(stderr, "\nFalha ao processar -q.\n");
          goto frees_and_exit;
        }
    } else if (strcmp(argv[i], "-o") == 0){
        printf("lendo parametro -o...\n");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -o.\n");
          goto frees_and_exit;
        }
        trataPath(&dirSaida, argv[i]);
        if(dirSaida == NULL) {
          fprintf(stderr, "\nFalha ao processar -o.\n");
          goto frees_and_exit;
        }
    } else if (strcmp(argv[i], "-p") == 0){ // prioMax (int)
        printf("lendo parametro -p (prioMax)...\n");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -p.\n");
          goto frees_and_exit;
        }
        trataParamNumericoInt(&prioMax, argv[i]);
        if(prioMax == NULL) {
          fprintf(stderr, "\nFalha ao processar -p (prioMax).\n");
          /* Pode decidir sair ou continuar com prioMax=NULL */
        }
      } else if (strcmp(argv[i], "-hc") == 0){ // hc (int)
        printf("lendo parametro -hc...\n");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -hc.\n");
          goto frees_and_exit;
        }
        trataParamNumericoInt(&hc, argv[i]);
        if(hc == NULL) {
          fprintf(stderr, "\nFalha ao processar -hc.\n");
        }
      } else if (strcmp(argv[i], "-pr") == 0){ // promoRate (float)
        printf("lendo parametro -pr (promoRate)...\n");
        i++;
        if (i >= argc){
          printf("\nERRO: falta parametro para -pr.\n");
          goto frees_and_exit;
        }
        trataParamNumericoDouble(&promoRate, argv[i]);
        if(promoRate == NULL) {
          fprintf(stderr, "\nFalha ao processar -pr (promoRate).\n");
        }
      } else {
        printf("\nParametro desconhecido: %s\n", argv[i]);
        // goto frees_and_exit; 
      }
    i++;
  }
  //while.

  completaNomeArquivo(dirEntrada, nomeGeo, &fullNomeGeo);
  completaNomeArquivo(dirEntrada, nomeQry, &fullNomeQry);
  trataArqTxt(dirSaida, nomeGeo, nomeQry, &fullNomeTxt);

  printf("\n--- Valores Processados ---\n");
  printf("dirEntrada = %s\n", dirEntrada ? dirEntrada : "(null ou falha)");
  printf("dirSaida   = %s\n", dirSaida   ? dirSaida   : "(null ou falha)");
  printf("nomeGeo    = %s\n", nomeGeo    ? nomeGeo    : "(null ou falha)");
  printf("nomeQry    = %s\n", nomeQry    ? nomeQry    : "(null ou falha)");
  printf("fullNomeQry    = %s\n", fullNomeQry    ? fullNomeQry    : "(null ou falha)");
  printf("fullNomeGeo    = %s\n", fullNomeGeo   ? fullNomeGeo    : "(null ou falha)");
  printf("fullNomeTxt    = %s\n", fullNomeTxt   ? fullNomeTxt    : "(null ou falha)");
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
    printf("promoRate = %f\n\n\n\n\n\n", *promoRate);
  } else {
    printf("promoRate = (nao definido ou falha)\n");
  }

  int idMax = 0;
  SmuTreap t = processa_geo(fullNomeGeo, dirSaida, nomeGeo, &idMax, prioMax, hc, promoRate, EPSILON_CONFIG);

  Lista array_selecoes[100];
  for (int i = 0; i < 100; i++){
    array_selecoes[i] = NULL;
  }

  Lista lista_svg_qry = criaLista();
  processa_qry(t, fullNomeQry, dirSaida, nomeQry, &array_selecoes[0], lista_svg_qry, &idMax, EPSILON_CONFIG);

frees_and_exit: // Rótulo para liberar memória antes de sair em caso de erro
  // Liberar memória
  if(dirEntrada) free(dirEntrada);
  if(nomeGeo) free(nomeGeo);
  if(fullNomeGeo) free(fullNomeGeo);
  if(dirSaida) free(dirSaida);
  if(nomeQry) free(nomeQry);
  if(fullNomeQry) free(fullNomeQry);

  if(prioMax) free(prioMax);
  if(hc) free(hc);
  if(promoRate) free(promoRate);

  return 0;
 }