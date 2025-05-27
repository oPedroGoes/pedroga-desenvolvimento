 #include <stdio.h>
 #include <stdlib.h>
 #include <assert.h>
 #include <string.h>


 #define PATH_LEN 250
 #define FILE_NAME_LEN 100
 #define MSG_LEN 1000




 /* Acabei de corrigir a logica para printar, guardar e modificar os parametros passados por linha. FALTAM OS PARAMETROS DE ARVORE. FALTA ATUALIZAR O FULL NAME ARQ.
 */


 void trataPath(char **pathDir, char* arg){
  int argLen = strlen(arg);

  *pathDir = (char*) malloc(sizeof(char)*(argLen));
  if (*pathDir == NULL){
    printf("(trataPath) ERRO: falha ao alocar memoria");
    return ;
  }

  if (argLen > 0 && arg[argLen - 1] == '/') {
    arg[argLen - 1] = '\0'; // Modifica arg (argv[i])
}

  strcpy(*pathDir,arg);
 }


 void trataNomeArquivo(char **pathArq, char* arg){
  int argLen = strlen(arg);

  *pathArq = (char*) malloc(sizeof(char)*(argLen+1));
  if (*pathArq == NULL){
    printf("(trataNomeArquivo) ERRO: falha ao alocar memoria.");
    return ;
  }

  printf("nome: %s", arg);

  if(arg[argLen-1] == '/'){
    arg[argLen-1] = '\0';
  }

  printf("nome: %s", arg);
  strcpy(*pathArq, arg);
 }

void trataParamNumerico(int **num, char* param){
  int paramLen = strlen(param);
}

 int main(int argc, char *argv[]){
  char *dirEntrada = NULL, *nomeGeo = NULL, *fullNomeGeo = NULL, *dirSaida = NULL,
  *nomeQry = NULL, *fullNomeQry = NULL, *fullNameArq;
  int *prioMax = NULL, *hc = NULL, *promoRate = NULL;




  FILE *f;
  /* MOSTRA OS PARAMETROS */
  for(int i= 0; i< argc; i++){
    printf("argv[%d] = %s\n",i,argv[i]);
  }

  int i = 1;
  while (i < argc){
    if (strcmp(argv[i], "-e") == 0){
        printf("\n lendo parametro -e... ");
        i++;

        if (i >= argc){
            printf("\nERRO: falta parametro.");
            assert(i < argc);
        }

        trataPath(&dirEntrada, argv[i]);
    }
    else if (strcmp(argv[i], "-f") == 0){
        printf("\n lendo parametro -f... ");
        i++;

        if (i >= argc){
            printf("\nERRO: falta parametro.");
            assert(i < argc);
        }

        trataNomeArquivo(&nomeGeo, argv[i]);
    } else if (strcmp(argv[i], "-q") == 0){
        printf("\n lendo parametro -q... ");
        i++;

        if (i >= argc){
            printf("\nERRO: falta parametro.");
            assert(i < argc);
        }

        trataNomeArquivo(&nomeQry, argv[i]);
    } else if (strcmp(argv[i], "-o") == 0){
        printf("\n lendo parametro -o... ");
        i++;

        if (i >= argc){
            printf("\nERRO: falta parametro.");
            assert(i < argc);
        }

        trataPath(&dirSaida, argv[i]);
    } else if (strcmp(argv[i], "-p") == 0){
        printf("\n lendo parametro -p... ");
        i++;

        if (i >= argc){
            printf("\nERRO: falta parametro.");
            assert(i < argc);
        }

        trataPath(&dirSaida, argv[i]);
      }
    i++;
  }
  //while.


  printf("\ndirEntrada = %s\n", dirEntrada);
  printf("\ndirSaida = %s\n", dirSaida);
  printf("\nnomeGeo = %s\n", nomeGeo);
  printf("\nnomeQry = %s\n", nomeQry);
  printf("\ndirEntrada = %s\n", dirEntrada);

return 0;
 }
