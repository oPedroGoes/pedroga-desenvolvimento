#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include "lancadores.h"
#include "linha.h"
#include "circulo.h"
#include "texto.h"
#include "retangulo.h"
//#include "arvorebin.h"
//#include "formas.h"

int ma(float x1, float x2, float y1, float y2){
    if(x1<x2){
        return 1;
    } else if(x2<x1){
        return 2;
    } else{
        if(y1<=y2){
            return 3;
        } else{
            return 4;
        }
    }
}

/*AbdTree*/ void leitura_geo(FILE **ssvg1, FILE *arqgeo, int *instru, int * formcriadas, int *id) {
    char* str = (char*)malloc(500 * sizeof(char));
    char* tipo = (char*)malloc(3 * sizeof(char));
    char* fam = NULL;
    char* wei = NULL;
    char* size = NULL;
    int contadorforms=1;
    //AbdTree a1;
    //FORMASGEO f1;
    while (fgets(str, 500, arqgeo)) {
        strncpy(tipo, str, 3);
        tipo[2] = '\0';
        if (strcmp(tipo, "ts") == 0) {
            free(fam);
            free(wei);
            free(size);
            fam = (char*)malloc(15 * sizeof(char));
            size = (char*)malloc(15 * sizeof(char));
            wei = (char*)malloc(15 * sizeof(char));
            sscanf(str + 2, "%s %s %s", fam, wei, size);
            printf("\n\n Ts lido!\nlinha lida: %s %s %s %s", tipo, fam, wei, size);
        }

        if (strcmp(tipo, "c ") == 0) {
            int i;
            float x, y, r;
            char* cb = (char*)malloc(15 * sizeof(char));
            char* cp = (char*)malloc(15 * sizeof(char));
            sscanf(str + 2, "%d %f %f %f %s %s", &i, &x, &y, &r, cb, cp);
            *id=i;
            CIRCLE c = create_circle(i, x, y, r, cb, cp);
            printf("\n\n Circulo criado!\nlinha lida: %s %d %f %f %f %s %s", tipo, i, x, y, r, cb, cp);
            if(contadorforms==1){

/////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                /*a1=createAbd(x, y, 0.0001);                
                *xcent=x;
                *ycent=y;
                f1=setforma(c, 1);      // Talvez, esse não seja específico do edu.
                insertAbd(a1, x, y, f1);*/
/////////////////////////////////////////////////////////////////////////////////////////
                contadorforms++;
            }else{

/*////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                f1=setforma(c, 1);      // Talvez, esse não seja específico do edu.
                insertAbd(a1, x, y, f1);
////////////////////////////////////////////////////////////////////////////////////////*/
            }
            free(cb);
            free(cp);
            *formcriadas=*formcriadas+1;
        }

        if (strcmp(tipo, "r ") == 0) {
            int i;
            float x, y, h, w;
            char* cb = (char*)malloc(15 * sizeof(char));
            char* cp = (char*)malloc(15 * sizeof(char));
            sscanf(str + 2, "%d %f %f %f %f %s %s", &i, &x, &y, &w, &h, cb, cp);
            *id=i;
            RECTANGLE r = create_rectangle(i, x, y, w, h, cb, cp);
            printf("\n\n Retangulo criado!\nlinha lida: %s %d %f %f %f %f %s %s", tipo, i, x, y, w, h, cb, cp);
            if(contadorforms==1){
/*////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                *xcent=acharbaricentroRetX(x, w);
                *ycent=acharbaricentroRetY(y, h);
                a1=createAbd(acharbaricentroRetX(x, w), acharbaricentroRetY(y, h), 0.0001);
                f1=setforma(r, 2);
                insertAbd(a1, x, y, f1);
////////////////////////////////////////////////////////////////////////////////////////*/                
                contadorforms++;
            }else{
/*////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                f1=setforma(r, 2);
                insertAbd(a1, x, y, f1);
////////////////////////////////////////////////////////////////////////////////////////*/
            }
            free(cb);
            free(cp);
            *formcriadas=*formcriadas+1;
        }

        if (strcmp(tipo, "l ") == 0) {
            int i;
            float x1, y1, x2, y2;
            char* c = (char*)malloc(15 * sizeof(char));
            sscanf(str + 2, "%d %f %f %f %f %s", &i, &x1, &y1, &x2, &y2, c);
            *id=i;
            LINHA l = cria_linha(i, x1, y1, x2, y2, c);
            printf("\n\n Linha criada!\nlinha lida: %s %d %f %f %f %f %s", tipo, i, x1, y1, x2, y2, c);
            int maior=ma(x1, x2, y1, y2);
            if(maior==1 || maior ==3){
                if(contadorforms==1){
/*////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                *xcent=acharbaricentroLinX(x1, x2);
                *ycent=acharbaricentroLinY(y1, y2);
                a1=createAbd(acharbaricentroLinX(x1, x2), acharbaricentroLinY(y1, y2), 0.0001);
                f1=setforma(l, 4);
                insertAbd(a1, x1, y1, f1);
////////////////////////////////////////////////////////////////////////////////////////*/                    
                    contadorforms++;
                }else{
/*////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                f1=setforma(l, 4);
                insertAbd(a1, x1, y1, f1);
////////////////////////////////////////////////////////////////////////////////////////*/                    
                }
            } else{
                if(contadorforms==1){
/*////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                *xcent=acharbaricentroLinX(x1, x2);
                *ycent=acharbaricentroLinY(y1, y2);
                a1=createAbd(acharbaricentroLinX(x1, x2), acharbaricentroLinY(y1, y2), 0.0001);
                f1=setforma(l, 4);
                insertAbd(a1, x2, y2, f1);
////////////////////////////////////////////////////////////////////////////////////////*/                    
                    contadorforms++;
                }else{
/*////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                f1=setforma(l, 4);
                insertAbd(a1, x2, y2, f1);
////////////////////////////////////////////////////////////////////////////////////////*/                    
                }
            }
            free(c);
            *formcriadas=*formcriadas+1;
        }

        if (strcmp(tipo, "t ") == 0) {
            int i;
            float x, y;
            char* cb = (char*)malloc(15 * sizeof(char));
            char* cp = (char*)malloc(15 * sizeof(char));
            char a;
            char* txt = (char*)malloc(300 * sizeof(char));
            sscanf(str + 2, "%d %f %f %s %s %c %[^\n]", &i, &x, &y, cb, cp, &a, txt);
            *id=i;
            TEXTO t = cria_texto(i, x, y, cb, cp, a, txt, fam, wei, size);
            printf("\n\n Texto criado!\nlinha lida: %s %d %f %f %s %s %s %s", tipo, i, x, y, cb, cp, &a, txt);
            if(contadorforms==1){
/*////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                *xcent=acharbaricentroLinX(x, get_eixoX2T(t));
                *ycent=acharbaricentroLinY(y, y);
                    a1=createAbd(acharbaricentroLinX(x, get_eixoX2T(t)), acharbaricentroLinY(y, y), 0.0001);
                    f1=setforma(t, 3);
                    insertAbd(a1, x, y, f1);
////////////////////////////////////////////////////////////////////////////////////////*/                
                contadorforms++;
            }else{
/*////////////////////////////////////////////////////////////////////////////////////////
                //COMANDOS ESPECIFICOS DO EDU
                f1=setforma(t, 3);
                insertAbd(a1, x, y, f1);
////////////////////////////////////////////////////////////////////////////////////////*/                
            }
            free(cb);
            free(cp);
            free(txt);
            *formcriadas=*formcriadas+1;
        }
        *instru=*instru+1;
    }
    //percursoSimetrico(a1, &printasvg, ssvg1);
    free(str);
    free(tipo);
    free(fam);
    free(size);
    free(wei);
    //return a1;
}

/*AbdTree*/ void processa_geo(const char* pathgeo, const char* dirsaida, const char* nomegeo, int *instru, 
                     int *formcriadas, int *id) {

    FILE* arqgeo = fopen(pathgeo, "r");
    printf("Diretório do arquivo geo: %s\n", pathgeo);
    if (arqgeo==NULL) {
        fprintf(stderr, "Erro na abertura do arquivo GEO: %s\n", pathgeo);
        fclose(arqgeo);
        exit(1);
    }

    char saidasvg1[512];
        saidasvg1[0]='\0';
        strcat(saidasvg1, dirsaida);
        strcat(saidasvg1, "/");
        strcat(saidasvg1, nomegeo);
        strcat(saidasvg1, ".svg");
        printf("Diretório do arquivo svg1: %s\n", saidasvg1);
    FILE* ssvg1 = fopen(saidasvg1, "w");
    if (ssvg1==NULL) {
        fprintf(stderr, "Erro na criação do arquivo SVG: %s\n", saidasvg1);
        fclose(arqgeo);
        fclose(ssvg1);
        exit(1);
    }

    fprintf(ssvg1, "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(ssvg1, "<svg width=\"2000\" height=\"2000\" xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">\n");

    /*AbdTree*/ leitura_geo(&ssvg1, arqgeo, instru, formcriadas, id);

    fprintf(ssvg1, "</svg>\n");
    fclose(arqgeo);
    fclose(ssvg1);
    //return a1;
}