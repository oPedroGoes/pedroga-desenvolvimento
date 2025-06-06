#ifndef geometria_h
#define geometria_h

#include "boundingBox.h"

bool retanguloInternoRetangulo(double Dx1, double Dy1, double Dx2, double Dy2, double Fx1, double Fy1, double Fx2, double Fy2);
bool retangulos_interceptam(double R1x1, double R1y1, double R1x2, double R1y2, double R2x1, double R2y1, double R2x2, double R2y2);
bool ponto_interno_retangulo(double px, double py, double rx1, double ry1, double rx2, double ry2, double epsilon);

#endif