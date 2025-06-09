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
    if(!forma || !tipo || !x1 || !y1){
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

// Ideia de fazer um get_cor para todas as formas. Caso linha retorna 1 e passa corL para cb.
int get_corF(Info forma, DescritorTipoInfo tipo, char* new_corb, char* new_corp){
    if(!forma || !tipo || !new_corb || !new_corp){
        fprintf(stderr, "(get_corF) Erro: parametros invalidos.\n");
        return;
    }

    switch (tipo)
    {
    case TIPO_CIRCULO:{
        CIRCLE c = (CIRCLE)forma;
        new_corb = strdup(get_cbC(c));
        new_corp = strdup(get_cpC(c));
        return 2;
    }
    case TIPO_RETANGULO:{
        RECTANGLE r = (RECTANGLE)forma;
        new_corb = strdup(get_cbR(r));
        new_corp = strdup(get_cpR(r));
        return 2;
    }
    case TIPO_TEXTO:{
        TEXTO t = (TEXTO)forma;
        new_corb = strdup(get_cbT(t));
        new_corp = strdup(get_cpT(t));
        return 2;
    }

    case TIPO_LINHA:{
        LINHA l = (LINHA)forma;
        new_corb = strdup(get_cL(l));
        new_corp = NULL;
        return 1;
    }
    
    default:
        fprintf(stderr, "(get_corF) Erro: tipo de forma desconhecido.\n");
        return;
    }

    if(!new_corb || !new_corp){
        fprintf(stderr, "(get_corF) Erro: falha ao alocar memoria para cores.\n");
        return NULL;
    }

    return;
}

// Ideia de fazer um set_cor para todas as formas. recebe duas cores, e define corL como corB_new.
void set_corF(Info forma, DescritorTipoInfo tipo, const char* new_corb, const char* new_corp){
    if(!forma || !tipo || !new_corb || !new_corp){
        fprintf(stderr, "(set_corF) Erro: parametros invalidos.\n");
        return;
    }

    switch (tipo)
    {
    case TIPO_CIRCULO:{
        CIRCLE c = (CIRCLE)forma;
        set_cbC(c, (char*)new_corb);
        set_cpC(c, (char*)new_corp);
        break;
    }
    case TIPO_RETANGULO:{
        RECTANGLE r = (RECTANGLE)forma;
        set_cbR(r, (char*)new_corb);
        set_cpR(r, (char*)new_corp);
        break;
    }
    case TIPO_TEXTO:{
        TEXTO t = (TEXTO)forma;
        set_cbT(t, (char*)new_corb);
        set_cpT(t, (char*)new_corp);
        break;
    }

    case TIPO_LINHA:{
        LINHA l = (LINHA)forma;
        set_cL(l, (char*)new_corb);
        break;
    }
    
    default:
        fprintf(stderr, "(set_corF) Erro: tipo de forma desconhecido.\n");
        return;
    }

    return;
}

void set_strkWF(Info forma, DescritorTipoInfo tipo, double strokeWidth){
    if(!forma || !tipo || strokeWidth < 0){
        fprintf(stderr, "(set_strkWF) Erro: parametros invalidos.\n");
        return;
    }

    switch (tipo)
    {
    case TIPO_CIRCULO:{
        CIRCLE c = (CIRCLE)forma;
        set_strkWC(c, strokeWidth);
        break;
    }

    case TIPO_RETANGULO:{
        RECTANGLE r = (RECTANGLE)forma;
        set_strkWR(r, strokeWidth);
        break;
    }

    case TIPO_TEXTO:{
        TEXTO t = (TEXTO)forma;
        set_strkWT(t, strokeWidth);
        break;
    }

    case TIPO_LINHA:{
        LINHA l = (LINHA)forma;
        set_strkWL(l, strokeWidth);
        break;
    }
    
    default:
        fprintf(stderr, "(set_strkWF) Erro: tipo de forma desconhecido.\n");
        return;
    }
}

void killF(Info forma, DescritorTipoInfo tipo){
    if(!forma || !tipo){
        fprintf(stderr, "(killF) Erro: parametros invalidos.\n");
        return;
    }

    switch (tipo)
    {
    case TIPO_CIRCULO:
        kill_circ((CIRCLE)forma);
        break;

    case TIPO_RETANGULO:
        kill_rectangle((RECTANGLE)forma);
        break;

    case TIPO_TEXTO:
        kill_texto((TEXTO)forma);
        break;

    case TIPO_LINHA:
        kill_linha((LINHA)forma);
        break;
    
    default:
        fprintf(stderr, "(killF) Erro: tipo de informacao desconhecido.\n");
        break;
    }
    return;
}







