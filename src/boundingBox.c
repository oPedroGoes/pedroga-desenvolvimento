#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "boundingBox.h"
#include "SmuTreap.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"
#include "formas.h"


void calcBB_c(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    if (i == NULL) { 
        printf("(calcBB_c) Erro: ponteiro para estrutura circulo aponta NULL. Definindo a BB como 0.\n");
        *bbA_x=0; *bbA_y=0; *bbA_w=0; *bbA_h=0; 
        return;
    }
    double xc = get_XC((CIRCLE)i);
    double yc = get_YC((CIRCLE)i);
    double rc = get_rC((CIRCLE)i);

    *bbA_x = xc - rc;
    *bbA_y = yc + rc;
    *bbA_w = 2*rc;
    *bbA_h = 2*rc;
}

void calcBB_r(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    if (i == NULL) { 
        printf("(calcBB_r) Erro: ponteiro para estrutura RETANGULO aponta NULL. Definindo a BB como 0.\n");
        *bbA_x=0; *bbA_y=0; *bbA_w=0; *bbA_h=0; 
        return;
    }
    *bbA_x = get_XR((RECTANGLE)i);
    *bbA_y = get_YR((RECTANGLE)i);
    *bbA_w = get_wR((RECTANGLE)i);
    *bbA_h = get_hR((RECTANGLE)i);
}

void calcBB_t(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    if (i == NULL) { 
        printf("(calcBB_t) Erro: ponteiro para estrutura circulo aponta NULL. Definindo a BB como 0.\n");
        *bbA_x=0; *bbA_y=0; *bbA_w=0; *bbA_h=0; 
        return;
    }

    TEXTO texto_info = (TEXTO)i; // Cast para o tipo TEXTO do seu TAD

    double tx_ancora = get_XT(texto_info);     
    double ty_ancora = get_YT(texto_info);    
    char tipo_ancora = get_charancoraT(texto_info);
    char* str_texto = get_txtT(texto_info);
    char* str_fSize = get_fsT(texto_info);

    // Converter tamanho da fonte (string) para double.
    double Fsize = str_fSize ? atof(str_fSize) : 10.0;
    if (Fsize <= 0) Fsize = 10.0;

    int num_chars = str_texto ? strlen(str_texto) : 0;
    if (num_chars == 0) num_chars = 1;
    

    // Estima qual é a largura de cada char, dado que a fonte pode variar.
    double K_LARGURA_CHAR_APROX = 0.6;
    double larguraReal_char = K_LARGURA_CHAR_APROX * Fsize;

    double calc_bb_h = Fsize; // Altura do BB é igual ao tamanho da fonte
    double calc_bb_w = num_chars * larguraReal_char;

    if (num_chars == 0) { // Texto vazio
        calc_bb_w =  Fsize * K_LARGURA_CHAR_APROX; // Cria o BB de um char.
    }

    // Ajuste horizontal baseado na âncora (i, m, f)
    if (tipo_ancora == 'i') {        // Início
        *bbA_x = tx_ancora;
    } else if (tipo_ancora == 'm') { // Meio
        *bbA_x = tx_ancora - (calc_bb_w / 2.0);
    } else {                         // Fim ('f')
        *bbA_x = tx_ancora - calc_bb_w;
    }
    
    // Ajuste vertical. O 'y' da âncora é o centro, então o topo do BB está
    // meia altura para cima.
    *bbA_y = ty_ancora - (calc_bb_h / 2.0);

    // Atribui largura e altura calculadas
    *bbA_w = calc_bb_w;
    *bbA_h = calc_bb_h;
}

void calcBB_l(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    double x1 = get_X1L((LINHA)i);
    double y1 = get_Y1L((LINHA)i);
    double x2 = get_X2L((LINHA)i);
    double y2 = get_Y2L((LINHA)i);

    *bbA_x = fmin(x1, x2);
    *bbA_y = fmin(y1, y2);
    *bbA_w = fabs(x1 - x2);
    *bbA_h = fabs(y1 - y2);
}

void fCalcBB_individual(DescritorTipoInfo tp, Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    switch (tp)
    {
    case TIPO_CIRCULO:
        calcBB_c(i, bbA_x, bbA_y, bbA_w, bbA_h);
        break;

    case TIPO_RETANGULO:
        calcBB_r(i, bbA_x, bbA_y, bbA_w, bbA_h);
        break;

    case TIPO_TEXTO:
        calcBB_t(i, bbA_x, bbA_y, bbA_w, bbA_h);
        break;
    
    case TIPO_LINHA:
        calcBB_l(i, bbA_x, bbA_y, bbA_w, bbA_h);
        break;

    default:
        fprintf(stderr, "(fCalcBB_individual) Erro: tipo de forma invalido.");
        return;
    }
}