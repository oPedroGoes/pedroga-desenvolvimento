#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "SmuTreap.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"

//c: 1, r: 2, t: 3, l: 4

void calcBB_c(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    double xc = get_XC(i);
    double yc = get_YC(i);
    double rc = get_rC(i);

    *bbA_x = xc - rc;
    *bbA_y = yc + rc;
    *bbA_w = 2*rc;
    *bbA_h = 2*rc;
}

void calcBB_r(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    *bbA_x = get_XR(i);
    *bbA_y = get_YR(i);
    *bbA_w = get_wR(i);
    *bbA_h = get_hR(i);
}

void calcBB_t(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    
}

double maxD(double x, double y){
    return (x > y) ? x : y;
}
double minD(double x, double y){
    return (x < y) ? x : y;
}
void calcBB_l(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    double x1 = get_X1L(i);
    double y1 = get_Y1L(i);
    double x2 = get_X2L(i);
    double y2 = get_Y2L(i);

    *bbA_x = minD(x1, x2);
    *bbA_y = maxD(y1, y2);
    *bbA_w = abs(x1 - x2);
    *bbA_h = abs(y1 - y2);
}

void *fCalcBB(DescritorTipoInfo tp, Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h){
    if (tp == 1){
        //calcBB_c()
    }
}