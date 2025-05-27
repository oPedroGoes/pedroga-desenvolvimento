#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circulo.h"

typedef struct{
    int i;
    float x, y;
    float r;
    char *corb, *corp;
}circulo;

CIRCULO cria_circulo(int i, float x, float y, float r, char* cb, char* cp){
    circulo* c=(circulo*)malloc(sizeof(circulo));
    if(c==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    c->i=i;
    c->x=x;
    c->y=y;
    c->r=r;
    c->corb =(char*)malloc(sizeof(char)*(strlen(cb)+1));
    if(c->corb==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(c->corb, cb);
    c->corp =(char*)malloc(sizeof(char)*(strlen(cp)+1));
    if(c->corp==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(c->corp, cp);
    return c;
}

int get_idC(CIRCULO c){
    circulo *c1=((circulo*)c);
    return c1->i;
}

float get_XC(CIRCULO c){
    circulo *c1=((circulo*)c);
    return c1->x;
}

float get_YC(CIRCULO c){
    circulo *c1=((circulo*)c);
    return c1->y;
}

float get_rC(CIRCULO c){
    circulo *c1=((circulo*)c);
    return c1->r;
}

char* get_cbC(CIRCULO c){
    circulo *c1=((circulo*)c);
    return c1->corb;
}

char* get_cpC(CIRCULO c){
    circulo *c1=((circulo*)c);
    return c1->corp;
}

float get_areaC(CIRCULO c){
    circulo *c1=((circulo*)c);
    return c1->r*c1->r*3.1415;
}

void set_xC(CIRCULO c, float x){
    circulo *c1=((circulo*)c);
    c1->x=x;
}

void set_yC(CIRCULO c, float y){
    circulo *c1=((circulo*)c);
    c1->y=y;
}

void set_cbC(CIRCULO c, char* cb){
    circulo *c1=((circulo*)c);
    free(c1->corb);
    c1->corb=(char*)malloc(sizeof(char)*(strlen(cb)+1));
    if(c1->corb==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(c1->corb, cb);
}

void set_idC(CIRCULO c, int i){
    circulo *c1=((circulo*)c);
    c1->i=i;
}

void set_cpC(CIRCULO c, char* cp){
    circulo *c1=((circulo*)c);
    free(c1->corp);
    c1->corp=(char*)malloc(sizeof(char)*(strlen(cp)+1));
    if(c1->corp==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(c1->corp, cp);
}

void kill_circ(CIRCULO c){
    circulo *c1=((circulo*)c);
    free(c1->corb);
    free(c1->corp);
    free(c1);
}