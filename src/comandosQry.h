#ifndef COMANDOSQRY_H
#define COMANDOSQRY_H

#include <stdio.h>
#include <stdlib.h>

#include "SmuTreap.h"
#include "Lista.h"

typedef void* CONTEXTO;

CONTEXTO iniciaContext(FILE *arqTxt, SmuTreap t, Lista lista_anotacoes_svg, Lista *array_selecoes, int *idMax, FCalculaBoundingBox fCalcBB, double epsilon);
void killAnotacaoCallback(void *anotacao);

void handle_selr(CONTEXTO ctxt, int n_id_selecao, double sel_x, double sel_y, double sel_w, double sel_h);
void handle_seli(CONTEXTO ctxt, int n_id_selecao, double sel_x, double sel_y);
void handle_transp(CONTEXTO ctxt, int id, double x, double y);
void handle_cln(CONTEXTO ctxt, int n_id_selecao, double dx, double dy);
void handle_cmflg(CONTEXTO ctxt, int id_ref, char *cb, char* cp, double w);
void handle_spy(CONTEXTO ctxt, int id_ref);
void handle_blow(CONTEXTO ctxt, int id_ogiva);
void handle_disp(CONTEXTO ctxt, int id_linha, int n_selecao);



#endif