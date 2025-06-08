#ifndef formas_h
#define formas_h

#include "SmuTreap.h"
#include "formas.h"
#include "circulo.h"
#include "retangulo.h"
#include "texto.h"
#include "linha.h"

typedef enum{
    TIPO_DESCONHECIDO = 0, 
    TIPO_CIRCULO = 1, 
    TIPO_RETANGULO = 2, 
    TIPO_TEXTO = 3, 
    TIPO_LINHA = 4
} TIPO_FORMA;

int get_idF(Info forma_info, DescritorTipoInfo tipo);
const char* get_NameStrF(DescritorTipoInfo tipo);
void forma_get_ancora_para_svg(Info forma_info, DescritorTipoInfo tipo, double* x, double* y);
double get_areaF(Info forma_info, DescritorTipoInfo tipo);

/**
 * @brief Obtém as coordenadas da(s) âncora(s) de uma forma geométrica.
 * * @param forma A Info da forma.
 * @param tipo O descritor do tipo da forma.
 * @param x1 Ponteiro para armazenar a coordenada X da primeira âncora.
 * @param y1 Ponteiro para armazenar a coordenada Y da primeira âncora.
 * @param x2 Ponteiro para armazenar a coordenada X da segunda âncora (usado apenas para linhas).
 * @param y2 Ponteiro para armazenar a coordenada Y da segunda âncora (usado apenas para linhas).
 * @return int Retorna o número de âncoras preenchidas (1 para formas com uma âncora, 2 para linhas, 0 para erro).
 */
int get_anchorF(Info forma, DescritorTipoInfo tipo, double *x1, double *y1, double *x2, double *y2);

bool formaTotalmenteContidaCallback(SmuTreap t, Node n_node, Info forma_info, double reg_x1, double reg_y1, double reg_x2, double reg_y2);

void set_corF(Info forma, DescritorTipoInfo tipo, const char* new_corb, const char* new_corp);

void set_strkWF(Info forma, DescritorTipoInfo tipo, double strokeWidth);



void killF(Info forma, DescritorTipoInfo tipo);

#endif