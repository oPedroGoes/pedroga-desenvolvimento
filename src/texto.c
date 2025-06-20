#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "texto.h"

typedef struct{
    char *fFamily, *fWeight, *fSize;
}tipotxt;

typedef struct{
    int id;
    double x, y;
    char *corb, *corp;
    double strokeWidth;
    char a; //ancora do texto (inicio, meio e fim)
    char *texto;
    tipotxt ttxt;
}texto;

TEXTO cria_texto(int id, double x, double y, char* cb, char* cp, char a, char* txt, char* ff, char* fw, char* fs){
    texto* t=(texto*)malloc(sizeof(texto));
    if(t==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    t->id=id;
    t->x=x;
    t->y=y;
    t->corb =(char*)malloc(sizeof(char)*(strlen(cb)+1));
    if(t->corb==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(t->corb, cb);
    t->corp =(char*)malloc(sizeof(char)*(strlen(cp)+1));
    if(t->corp==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(t->corp, cp);

    t->strokeWidth = 1.0;
    t->a=a;
    
    t->texto =(char*)malloc(sizeof(char)*(strlen(txt)+1));
    if(t->texto==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(t->texto, txt);

    t->ttxt.fFamily =(char*)malloc(sizeof(char)*(strlen(ff)+1));
    if(t->ttxt.fFamily==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(t->ttxt.fFamily, ff);
    
    t->ttxt.fSize =(char*)malloc(sizeof(char)*(strlen(fs)+1));
    if(t->ttxt.fSize==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(t->ttxt.fSize, fs);
    t->ttxt.fWeight =(char*)malloc(sizeof(char)*(strlen(fw)+1));
    if(t->ttxt.fWeight==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(t->ttxt.fWeight, fw);
    return t;
}

int get_idT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->id;
}

double get_XT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->x;
}

double get_YT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->y;
}

char* get_cbT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->corb;
}

char* get_cpT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->corp;
}

double get_strkWT(TEXTO t) {
    if(!t){
        fprintf(stderr, "(set_strkWC) Erro: parametros invalidos.");
        return -1;
    }
    texto* t1 = (texto*)t;
    return t1->strokeWidth;
}

void set_strkWT(TEXTO t, double sw) {
    if(!t || sw < 0){
        fprintf(stderr, "(set_strkWC) Erro: parametros invalidos.");
        return;
    }
    texto* t1 = (texto*)t;
    if (t1) t1->strokeWidth = sw;
}

char* get_ancoraT(TEXTO t){
    texto *t1=((texto*)t);
    if(t1->a=='i'){
        return "start";
    } else if(t1->a=='m'){
        return "middle";
    } else{
        return "end";
    }
}

char* get_txtT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->texto;
}

char* get_ffT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->ttxt.fFamily;
}

char* get_fsT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->ttxt.fSize;
}

char* get_fwT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->ttxt.fWeight;
}

int tamanho_dotextoT(TEXTO t){
    texto *t1=((texto*)t);
    return strlen(t1->texto);
}

int comprimento_da_linhaT(TEXTO t){
    texto *t1=((texto*)t);
    return 12*tamanho_dotextoT(t1);
}

double get_eixoX1T(TEXTO t){
    texto *t1=((texto*)t);
    if(t1->a=='i'){
        return t1->x;
    } else if(t1->a=='m'){
        return (t1->x-comprimento_da_linhaT(t1)/2);
    } else{
        return t1->x-comprimento_da_linhaT(t1);
    }
}

double get_eixoX2T(TEXTO t){
    texto *t1=((texto*)t);
    if(t1->a=='i'){
        return t1->x+comprimento_da_linhaT(t1);
    } else if(t1->a=='m'){
        return (t1->x+comprimento_da_linhaT(t1)/2);
    } else{
        return t1->x;
    }
}

void set_idT(TEXTO t, int id){
    texto *t1=(texto*)t;
    t1->id=id;
}

void set_xT(TEXTO t, double x){
    texto *t1=((texto*)t);
    t1->x=x;
}

void set_yT(TEXTO t, double y){
    texto *t1=((texto*)t);
    t1->y=y;
}

void set_cbT(TEXTO t, char* cb){
    texto *t1=((texto*)t);
    free(t1->corb);
    t1->corb=(char*)malloc(sizeof(char)*(strlen(cb)+1));
    if(t1->corb==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(t1->corb, cb);
}

void set_cpT(TEXTO t, char* cp){
    texto *t1=((texto*)t);
    free(t1->corp);
    t1->corp=(char*)malloc(sizeof(char)*(strlen(cp)+1));
    if(t1->corp==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(t1->corp, cp);
}

double get_areaT(TEXTO t){
    return 12*tamanho_dotextoT(t);
}

char get_charancoraT(TEXTO t){
    texto *t1=((texto*)t);
    return t1->a;
}

void kill_texto(TEXTO t){
    texto *t1 = (texto*)t;
    free(t1->corb);
    free(t1->corp);

    free(t1->ttxt.fFamily);
    free(t1->ttxt.fSize);
    free(t1->ttxt.fWeight);

    free(t1);
}