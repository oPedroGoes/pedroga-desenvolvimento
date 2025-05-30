#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "circulo.h"

typedef struct{
    int id;
    double x, y;
    double r;
    char *corb, *corp;
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


    return c;
}

int get_idC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->id;
}

double get_XC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->x;
}

double get_YC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->y;
}

double get_rC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->r;
}

char* get_cbC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->corb;
}

char* get_cpC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->corp;
}

double get_areaC(CIRCLE c){
    circle *c1=((circle*)c);
    return c1->r*c1->r*3.1415;
}

void set_xC(CIRCLE c, double x){
    circle *c1=((circle*)c);
    c1->x=x;
}

void set_yC(CIRCLE c, double y){
    circle *c1=((circle*)c);
    c1->y=y;
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

void set_idC(CIRCLE c, int id){
    circle *c1=((circle*)c);
    c1->id=id;
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

void kill_circ(CIRCLE c){
    circle *c1=((circle*)c);
    free(c1->corb);
    free(c1->corp);
    free(c1);
}