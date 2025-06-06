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

bool retangulos_interceptam(double R1x1, double R1y1, double R1x2, double R1y2, double R2x1, double R2y1, double R2x2, double R2y2){
    if(R1x1 < 0 || R1y1 < 0 || R1x2 < 0 || R1y2 < 0 || R2x1 < 0 || R2y1 < 0 || R2x2 < 0 || R2y2 < 0){
        fprintf(stderr, "(retangulos_interceptam) Erro: parametros invalidos");
        return false;
    }

    double x_R1_min = fmin(R1x1, R1x2);
    double x_R1_max = fmax(R1x1, R1x2);
    double y_R1_min = fmin(R1y1, R1y2);
    double y_R1_max = fmax(R1y1, R1y2);

    double x_R2_min = fmin(R2x1, R2x2);
    double x_R2_max = fmax(R2x1, R2x2);
    double y_R2_min = fmin(R2y1, R2y2);
    double y_R2_max = fmax(R2y1, R2y2);

    if(x_R1_max < x_R2_min || x_R1_min > x_R2_max || // Nao intercepta em x
       y_R1_max < y_R2_min || y_R1_min > y_R2_max){ // Nao intercepta em y
        return false;
    }

    return true;
}

bool ponto_interno_retangulo(double px, double py, double rx1, double ry1, double rx2, double ry2, double epsilon){
    if (rx1 < 0 || ry1 < 0 || rx2 < 0 || ry2 < 0){
        fprintf(stderr, "(ponto_interno_retangulo) Erro: Tentativa de verificar ponto em retângulo com dimensões inválidas");
        return false;
    }

    double rx_min = fmin(rx1, rx2);
    double rx_max = fmax(rx1, rx2);
    double ry_min = fmin(ry1, ry2);
    double ry_max = fmax(ry1, ry2);
    

    bool px_esta_dentro = (px >= rx_min - epsilon) && (px <= rx_max + epsilon);
    bool py_esta_dentro = (py >= ry_min - epsilon) && (py <= ry_max + epsilon);

    return px_esta_dentro && py_esta_dentro;
}