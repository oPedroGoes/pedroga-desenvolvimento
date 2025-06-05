#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#include "geometria.h"

bool retanguloInternoRetangulo(double Dx1, double Dy1, double Dx2, double Dy2, double Fx1, double Fy1, double Fx2, double Fy2){
    if(Dx1 < 0 || Dy1 < 0 || Dx2 < 0 || Dy2 < 0 || Fx1 < 0 || Fy1 < 0 || Fx2 < 0 || Fy2 < 0){
        fprintf(stderr, "(retanguloInternoRetangulo) Erro: parametros invalidos");
        return false;
    }

    double x_dentro_min = fmin(Dx1, Dx2);
    double x_dentro_max = fmax(Dx1, Dx2);
    double y_dentro_min = fmin(Dy1, Dy2);
    double y_dentro_max = fmax(Dy1, Dy2);

    double x_fora_min = fmin(Fx1, Fx2);
    double x_fora_max = fmax(Fx1, Fx2);
    double y_fora_min = fmin(Fy1, Fy2);
    double y_fora_max = fmax(Fy1, Fy2);

    if (x_dentro_min >= x_fora_min && x_dentro_max <= x_fora_max && // X e' valido
        y_dentro_min >= y_fora_min && y_dentro_max <= y_fora_max){  // y e' valido
            return true;
    }

    return false;
}
