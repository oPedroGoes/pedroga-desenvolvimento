#ifndef comandosQry_h
#define comandosQry_h

#include <stdio.h>
#include <stdlib.h>

#include "SmuTreap.h"
#include "Lista.h"

void handle_selr(SmuTreap tree, FILE* pathTxtOut, int n_id_selecao, double sel_x, double sel_y, double sel_w, double sel_h, Lista* array_selecoes, Lista lista_anotacoes_svg);


#endif