#ifndef PROCESSASVG_H
#define PROCESSASVG_H

#include "SmuTreap.h"
#include "Lista.h"

void escreverFormaSvg(SmuTreap t, Node n, Info i, double x_ancora_no, double y_ancora_no, void *aux_file_ptr);

void visitaListaSvg(Item anotacao, void *aux);

#endif