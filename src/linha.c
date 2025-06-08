#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include "linha.h"

typedef struct{
    int id;
    double x1, y1, x2, y2;
    char *cor;
    double strokeWidth;
}linha;

LINHA cria_linha(int id, double x1, double y1, double x2, double y2, char* c){
    linha* l=(linha*)malloc(sizeof(linha));
    if(l==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    l->id=id;
    l->x1=x1;
    l->y1=y1;
    l->x2=x2;
    l->y2=y2;
    l->cor =(char*)malloc(sizeof(char)*(strlen(c)+1));
    if(l->cor==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(l->cor, c);
    l->strokeWidth = 1.0;
    return l;
}

int get_idL(LINHA l){
    linha *l1=((linha*)l);
    return l1->id;
}

double get_X1L(LINHA l){
    linha *l1=((linha*)l);
    return l1->x1;
}

double get_Y1L(LINHA l){
    linha *l1=((linha*)l);
    return l1->y1;
}

double get_X2L(LINHA l){
    linha *l1=((linha*)l);
    return l1->x2;
}

double get_Y2L(LINHA l){
    linha *l1=((linha*)l);
    return l1->y2;
}

char* get_cL(LINHA l){
    linha *l1=((linha*)l);
    return l1->cor;
}

double get_strkWL(LINHA l){
    if(!l){
        fprintf(stderr, "(get_strkWL) Erro: parametro invalido");
        return -1;
    }

    linha *l1 = (linha*)l;
    return l1->strokeWidth;
}

void set_strkWL(LINHA l, double sw){
    if(!l || sw < 0){
        fprintf(stderr, "(ser_strkWL) Erro: parametro invalido");
        return ;
    }
    linha *l1 = (linha*)l;
    l1->strokeWidth = sw;
}

double get_areaL(LINHA l){
    linha *l1=((linha*)l);
    double x=l1->x2-l1->x1;
    double y=l1->y2-l1->y1;
    return 1.5*sqrt(pow(x,2)+pow(y,2));
}

void set_x1L(LINHA l, double x){
    linha *l1=((linha*)l);
    l1->x1=x;
}

void set_y1L(LINHA l, double y){
    linha *l1=((linha*)l);
    l1->y1=y;
}

void set_x2L(LINHA l, double x){
    linha *l1=((linha*)l);
    l1->x2=x;
}

void set_y2L(LINHA l, double y){
    linha *l1=((linha*)l);
    l1->y2=y;
}

void set_cL(LINHA l, char* c){
    linha *l1=((linha*)l);
    free(l1->cor);
    l1->cor=(char*)malloc(sizeof(char)*(strlen(c)+1));
    if(l1->cor==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(l1->cor, c);
}

void set_idL(LINHA l, int id){
    linha *l1=(linha*)l;
    l1->id=id;
}

void kill_linha(LINHA l){
    linha *l1=((linha*)l);
    free(l1->cor);
    free(l1);
}