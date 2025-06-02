#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

//#include "boundingBox.h"
#include "SmuTreap.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"

//c: 1, r: 2, t: 3, l: 4
typedef void* BB; //ADICIONAR NO .H!!!!!!!


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
    double Fsize = 0;
    if (str_fSize != NULL) {
        Fsize = atof(str_fSize); // atof converte string para double
    }
    if (Fsize <= 0) {
        Fsize = 10.0; // Um tamanho de fonte padrão pequeno se a conversão falhar ou for inválida
        // printf("Aviso (calcBB_t): Tamanho da fonte inválido ou zero, usando default %f.\n", Fsize);
    }

    int num_chars = 0;
    if (str_texto != NULL) {
        num_chars = strlen(str_texto);
    }

    // Estima qual é a largura de cada char, dado que a fonte pode variar.
    double K_LARGURA_CHAR_APROX = 0.6;
    double larguraReal_char = K_LARGURA_CHAR_APROX * Fsize;

    double calc_bb_h = Fsize; // Altura do BB é igual ao tamanho da fonte
    double calc_bb_w = num_chars * larguraReal_char;

    if (num_chars == 0) { // Texto vazio
        calc_bb_w =  Fsize * K_LARGURA_CHAR_APROX; // Cria o BB de um char.
    }

    // Calcular âncora (canto inferior esquerdo) do BB,
    // assumindo que ty_ancora é a coordenada Y do pe' da linha de texto.
    if (tipo_ancora == 'i') { // Âncora no início (canto superior esquerdo do texto)
        *bbA_x = tx_ancora;
        *bbA_y = ty_ancora;
    } else if (tipo_ancora == 'm') { // Âncora no meio (horizontalmente, no topo do texto)
        *bbA_x = tx_ancora - (calc_bb_w / 2.0);
        *bbA_y = ty_ancora;
    } else if (tipo_ancora == 'f') { // Âncora no fim (canto superior direito do texto)
        *bbA_x = tx_ancora - calc_bb_w;
        *bbA_y = ty_ancora;
    } else {
        // Caso padrão ou erro no tipo de âncora: assumir início
        *bbA_x = tx_ancora;
        *bbA_y = ty_ancora;
        printf("Aviso (calcBB_t): Tipo de âncora de texto desconhecido '%c', usando 'início' como padrão.\n", tipo_ancora);
    }

    *bbA_w = calc_bb_w;
    *bbA_h = calc_bb_h;
}

double maxD(double x, double y){
    return (x > y) ? x : y;
}
double minD(double x, double y){
    return (x < y) ? x : y;
}
void calcBB_l(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    double x1 = get_X1L((LINHA)i);
    double y1 = get_Y1L((LINHA)i);
    double x2 = get_X2L((LINHA)i);
    double y2 = get_Y2L((LINHA)i);

    *bbA_x = minD(x1, x2);
    *bbA_y = maxD(y1, y2);
    *bbA_w = fabs(x1 - x2);
    *bbA_h = fabs(y1 - y2);
}

void fCalcBB_individual(DescritorTipoInfo tp, Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    switch (tp)
    {
    case 1:
        calcBB_c(i, bbA_x, bbA_y, bbA_w, bbA_h);
        break;

    case 2:
        calcBB_r(i, bbA_x, bbA_y, bbA_w, bbA_h);
        break;

    case 3:
        calcBB_t(i, bbA_x, bbA_y, bbA_w, bbA_h);
        break;
    
    default:
        calcBB_l(i, bbA_x, bbA_y, bbA_w, bbA_h);
        break;
    }
}

/*void uniaoBB(BB bb_dest, BB bb1, BB bb2){
    // VERIFICACAO VALIDADE DE AMBOS OS BB.
    if (!bb1 || (bb1->w < 0 || bb1->h < 0)) { // bb1 inválido ou não inicializado
        if (bb2 && bb2->w >= 0 && bb2->h >= 0) {
            *bb_res = *bb2;
        } else {
            bb_res->x = 0; bb_res->y = 0; bb_res->w = -1; bb_res->h = -1; // inválido
        }
        return;
    }
    if (!bb2 || (bb2->w < 0 || bb2->h < 0)) { // bb2 inválido ou não inicializado
         *bb_res = *bb1; // bb1 é válido (verificado acima)
        return;
    }
}*/

//TALVEZ HAVERA UMA
//void *fCalcBB_agrupada(){}