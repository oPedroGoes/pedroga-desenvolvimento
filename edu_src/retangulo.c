#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"

typedef struct{
    int i;
    float x, y;
    float w, h;
    char *corb, *corp;
}retangulo;

RETANGULO cria_retangulo(int i, float x, float y, float w, float h, char* cb, char* cp){
    retangulo* r=(retangulo*)malloc(sizeof(retangulo));
    if(r==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    r->i=i;
    r->x=x;
    r->y=y;
    r->w=w;
    r->h=h;
    r->corb =(char*)malloc(sizeof(char)*(strlen(cb)+1));
    if(r->corb==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(r->corb, cb);
    r->corp =(char*)malloc(sizeof(char)*(strlen(cp)+1));
    if(r->corp==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(r->corp, cp);
    return r;
}

int get_idR(RETANGULO r){
    retangulo *r1=((retangulo*)r);
    return r1->i;
}

float get_XR(RETANGULO r){
    retangulo *r1=((retangulo*)r);
    return r1->x;
}

float get_YR(RETANGULO r){
    retangulo *r1=((retangulo*)r);
    return r1->y;
}

float get_wR(RETANGULO r){
    retangulo *r1=((retangulo*)r);
    return r1->w;
}

float get_hR(RETANGULO r){
    retangulo *r1=((retangulo*)r);
    return r1->h;
}

char* get_cbR(RETANGULO r){
    retangulo *r1=((retangulo*)r);
    return r1->corb;
}

char* get_cpR(RETANGULO r){
    retangulo *r1=((retangulo*)r);
    return r1->corp;
}

float get_areaR(RETANGULO r){
    retangulo *r1=((retangulo*)r);
    return r1->w*r1->h;
}

void set_xR(RETANGULO r, float x){
    retangulo *r1=((retangulo*)r);
    r1->x=x;
}

void set_yR(RETANGULO r, float y){
    retangulo *r1=((retangulo*)r);
    r1->y=y;
}

void set_cbR(RETANGULO r, char* cb){
    retangulo *r1=((retangulo*)r);
    free(r1->corb);
    r1->corb=(char*)malloc(sizeof(char)*(strlen(cb)+1));
    if(r1->corb==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(r1->corb, cb);
}

void set_idR(RETANGULO r, int i){
    retangulo *r1=(retangulo*)r;
    r1->i=i;
}

void set_cpR(RETANGULO r, char* cp){
    retangulo *r1=((retangulo*)r);
    free(r1->corp);
    r1->corp=(char*)malloc(sizeof(char)*(strlen(cp)+1));
    if(r1->corp==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(r1->corp, cp);
}

void kill_retangulo(RETANGULO r){
    retangulo *r1=((retangulo*)r);
    free(r1->corb);
    free(r1->corp);
    free(r1);
}