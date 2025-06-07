#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"
#include "boundingBox.h"
#include "geometria.h"


int get_idF(Info forma, DescritorTipoInfo tipo){
    if(!forma){
        fprintf(stderr, "(get_idF) Erro: parametro invalido.\n");
        return -1;
    }

   switch(tipo) {
        case TIPO_CIRCULO: return get_idC((CIRCLE)forma);
        case TIPO_RETANGULO: return get_idR((RECTANGLE)forma);
        case TIPO_LINHA: return get_idL((LINHA)forma);
        case TIPO_TEXTO: return get_idT((TEXTO)forma);
        default: return TIPO_DESCONHECIDO;
    }
}

const char* get_NameStrF(DescritorTipoInfo tipo){
    switch(tipo) {
        case TIPO_CIRCULO: return "circulo";
        case TIPO_RETANGULO: return "retangulo";
        case TIPO_LINHA: return "linha";
        case TIPO_TEXTO: return "texto";
        default: return "Desconhecido";
    }
}

int get_anchorF(Info forma, DescritorTipoInfo tipo, double *x1, double *y1, double *x2, double *y2){
    if(!forma || !tipo || !x1 || !y1 || !x2 || !y2){
        fprintf(stderr, "(get_anchorF) Erro: parametros invalidos.\n");
        return 0;
    }

    switch(tipo){
        case TIPO_CIRCULO:
            *x1 = get_XC((CIRCLE) forma);
            *y1 = get_YC((CIRCLE) forma);
            return 1;
        
        case TIPO_RETANGULO:
            *x1 = get_XR((RECTANGLE) forma);
            *y1 = get_YR((RECTANGLE) forma);
            return 1;

        case TIPO_TEXTO:
            *x1 = get_XT((TEXTO) forma);
            *y1 = get_YT((TEXTO) forma);
            return 1;

        case TIPO_LINHA:
            *x1 = get_X1L((LINHA) forma);
            *y1 = get_Y1L((LINHA) forma);
            *x2 = get_X2L((LINHA) forma);
            *y2 = get_Y2L((LINHA) forma);
            return 2;
        
        default:
            fprintf(stderr, "(forma_get_ancoras) Erro: tipo de forma desconhecido.\n");
            return 0;
    }
}

// Função de callback para getInfosDentroRegiaoSmuT: verifica se uma forma está totalmente contida na região
bool formaTotalmenteContidaCallback(SmuTreap t, Node n_node, Info forma_info, double reg_x1, double reg_y1, double reg_x2, double reg_y2){
    printf("\n\n Entrando em formaTotalmenteContidaCallback...\n");
    DescritorTipoInfo tipo_forma = getTypeInfoSmuT(t, n_node);

    if (tipo_forma == TIPO_CIRCULO || tipo_forma == TIPO_RETANGULO || tipo_forma == TIPO_TEXTO || tipo_forma == TIPO_LINHA ){
    double fx;
    double fy;
    double fw;
    double fh;
    fCalcBB_individual(tipo_forma, forma_info, &fx, &fy, &fw, &fh);

    double fxMax = fx + fw;
    double fyMax = fy + fh;

        if(retanguloInternoRetangulo(fx, fy, fxMax, fyMax, reg_x1, reg_y1, reg_x2, reg_y2)){
            const char *nomeForma_DEBUG = get_NameStrF(tipo_forma);
            printf("DEBUG (formaTotalmenteContidaCallback) Forma contida = %s\n", nomeForma_DEBUG);
        }

    return (retanguloInternoRetangulo(fx, fy, fxMax, fyMax, reg_x1, reg_y1, reg_x2, reg_y2));
    } else return false; // Tipo desconhecido.
}







