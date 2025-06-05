#ifndef comandosQry_h
#define comandosQry_h

#include <stdio.h>
#include <stdlib.h>

#include "SmuTreap.h"
#include "Lista.h"

typedef void* CONTEXTO;

void handle_selr(CONTEXTO ctxt, int n_id_selecao, double sel_x, double sel_y, double sel_w, double sel_h);


#endif