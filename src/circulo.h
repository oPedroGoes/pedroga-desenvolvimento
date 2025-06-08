#ifndef CIRCULO_H
#define CIRCULO_H

typedef void* CIRCLE;

CIRCLE create_circle(int id, double x, double y, double r, char *cb, char *cp);

int get_idC(CIRCLE c);
void set_idC(CIRCLE c, int id);

double get_XC(CIRCLE c);
void set_xC(CIRCLE c, double x);

double get_YC(CIRCLE c);
void set_yC(CIRCLE c, double y);

char* get_cbC(CIRCLE c);
void set_cbC(CIRCLE c, char* cb);

char* get_cpC(CIRCLE c);
void set_cpC(CIRCLE c, char* cp);

/**
 * @brief Pega por meio de um TAD a largura da borda do circulo.
 * @param c Estrutura do circulo para pegar a informação.
 * @return Retorna um double com a largura da borda.
 */
double get_strkWC(CIRCLE c);

/**
 * @brief Muda a largura da borda do circulo.
 * @param c Estrutura do circulo que vai ter a largura da borda mudada.
 * @param sw Nova largura da borda.
 */
void set_strkWC(CIRCLE c, double sw);

double get_areaC(CIRCLE c);

double get_rC(CIRCLE c);

void kill_circ(CIRCLE c);

#endif