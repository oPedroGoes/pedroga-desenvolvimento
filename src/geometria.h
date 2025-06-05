#ifndef geometria_h
#define geometria_h

#include "boundingBox.h"

bool retanguloInternoRetangulo(double Dx1, double Dy1, double Dx2, double Dy2, double Fx1, double Fy1, double Fx2, double Fy2);
bool retangulos_interceptam(const BB *bb1, const BB *bb2);

#endif