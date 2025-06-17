#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circulo.h"

typedef struct{
    int id;
    double x, y;
    double r;
    char *corb, *corp;
    double strokeWidth;
}circle;

CIRCLE create_circle(int id, double x, double y, double r, char* cb, char* cp){
    circle* c=(circle*)malloc(sizeof(circle));
    if(c == NULL){printf("(create_circle) Erro: falha ao alocar memoria para c."); exit(1);}

    c->id=id;
    c->x=x;
    c->y=y;
    c->r=r;

    c->corb =(char*)malloc(sizeof(char)*(strlen(cb)+1));
    if(c->corb == NULL){printf("(create_circle) Erro: falha ao alocar memoria para corb."); exit(1);}
    strcpy(c->corb, cb);


    c->corp = (char*)malloc(sizeof(char)*(strlen(cp)+1));
    if(c->corp == NULL){printf("(create_circle) Erro: falha ao alocar memoria para corp."); exit(1);}
    strcpy(c->corp, cp);

    c->strokeWidth = 1.0;

    return c;
}

int get_idC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->id;
}

void set_idC(CIRCLE c, int id){
    circle *c1=((circle*)c);
    c1->id=id;
}

double get_XC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->x;
}

void set_xC(CIRCLE c, double x){
    circle *c1=((circle*)c);
    c1->x=x;
}

double get_YC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->y;
}

void set_yC(CIRCLE c, double y){
    circle *c1=((circle*)c);
    c1->y=y;
}

char* get_cbC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->corb;
}

void set_cbC(CIRCLE c, char* cb){
    circle *c1=((circle*)c);
    free(c1->corb);
    c1->corb=(char*)malloc(sizeof(char)*(strlen(cb)+1));
    if(c1->corb==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(c1->corb, cb);
}

char* get_cpC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->corp;
}

void set_cpC(CIRCLE c, char* cp){
    circle *c1=((circle*)c);
    free(c1->corp);
    c1->corp=(char*)malloc(sizeof(char)*(strlen(cp)+1));
    if(c1->corp==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(c1->corp, cp);
}

double get_strkWC(CIRCLE c) {
    if(!c){
        fprintf(stderr, "(set_strkWC) Erro: parametros invalidos.");
        return -1;
    }
    circle* c1 = (circle*)c;
    return c1->strokeWidth;
}

void set_strkWC(CIRCLE c, double sw) {
    if(!c || sw < 0){
        fprintf(stderr, "(set_strkWC) Erro: parametros invalidos.");
        return;
    }
    circle* c1 = (circle*)c;
    if (c1) c1->strokeWidth = sw;
}

double get_areaC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->r*c1->r*3.1415;
}

double get_rC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->r;
}

void kill_circ(CIRCLE c){
    circle *c1=((circle*)c);
    free(c1->corb);
    free(c1->corp);
    free(c1);
}