#ifndef PROCESSAQRY_H
#define PROCESSAQRY_H

#include "SmuTreap.h"
#include "Lista.h"

SmuTreap processa_qry(SmuTreap t, const char *pathQry, const char *pathSaida, const char *nomeBaseGeo, const char *nomeBaseQry, Lista *array_anotacoes, 
                        Lista lista_anotacoes_svg, int *idMax, double epsilon);

#endif // PROCESSA_QRY_H