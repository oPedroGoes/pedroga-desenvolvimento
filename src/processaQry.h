#ifndef processaQry_h
#define processaQry_h

#include "SmuTreap.h"
#include "Lista.h"

SmuTreap processa_qry(SmuTreap t, const char *pathQry, const char *pathSaida, const char *nomeQry, Lista *array_anotacoes, 
                        Lista lista_anotacoes_svg, int *idMax, int epsilon);

#endif // PROCESSA_QRY_H