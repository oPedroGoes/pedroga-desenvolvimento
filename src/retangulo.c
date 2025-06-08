#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "retangulo.h"

typedef struct{
    int id;
    double x, y;
    double w, h;
    char *corb, *corp;
    double strokeWidth;

}rectangle;

RECTANGLE create_rectangle(int id, double x, double y, double w, double h, char* cb, char* cp){
    rectangle* r=(rectangle*)malloc(sizeof(rectangle));
    if(r==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }

    r->id=id;
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

    r->strokeWidth = 1.0;


    return r;
}

int get_idR(RECTANGLE r){
    rectangle *r1=((rectangle*)r);
    return r1->id;
}

double get_XR(RECTANGLE r){
    rectangle *r1=((rectangle*)r);
    return r1->x;
}

double get_YR(RECTANGLE r){
    rectangle *r1=((rectangle*)r);
    return r1->y;
}

double get_wR(RECTANGLE r){
    rectangle *r1=((rectangle*)r);
    return r1->w;
}

double get_hR(RECTANGLE r){
    rectangle *r1=((rectangle*)r);
    return r1->h;
}

char* get_cbR(RECTANGLE r){
    rectangle *r1=((rectangle*)r);
    return r1->corb;
}

char* get_cpR(RECTANGLE r){
    rectangle *r1=((rectangle*)r);
    return r1->corp;
}

double get_strkWT(RECTANGLE r) {
    if(!r){
        fprintf(stderr, "(set_strkWC) Erro: parametros invalidos.");
        return -1;
    }
    rectangle* r1 = (rectangle*)r;
    return r1->strokeWidth;
}

void set_strkWT(RECTANGLE r, double sw) {
    if(!r || sw < 0){
        fprintf(stderr, "(set_strkWC) Erro: parametros invalidos.");
        return;
    }
    rectangle* r1 = (rectangle*)r;
    if (r1) r1->strokeWidth = sw;
}

double get_areaR(RECTANGLE r){
    rectangle *r1=((rectangle*)r);
    return r1->w*r1->h;
}

void set_xR(RECTANGLE r, double x){
    rectangle *r1=((rectangle*)r);
    r1->x=x;
}

void set_yR(RECTANGLE r, double y){
    rectangle *r1=((rectangle*)r);
    r1->y=y;
}

void set_cbR(RECTANGLE r, char* cb){
    rectangle *r1=((rectangle*)r);

    free(r1->corb);

    r1->corb=(char*)malloc(sizeof(char)*(strlen(cb)+1));
    if(r1->corb==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(r1->corb, cb);
}

void set_idR(RECTANGLE r, int id){
    rectangle *r1=(rectangle*)r;
    r1->id=id;
}

void set_cpR(RECTANGLE r, char* cp){
    rectangle *r1=((rectangle*)r);

    free(r1->corp);
    
    r1->corp=(char*)malloc(sizeof(char)*(strlen(cp)+1));
    if(r1->corp==NULL){
        printf("Erro na alocação de memória!\n");
        exit(1);
    }
    strcpy(r1->corp, cp);
}

void kill_rectangle(RECTANGLE r){
    rectangle *r1=((rectangle*)r);
    free(r1->corb);
    free(r1->corp);
    free(r1);
}