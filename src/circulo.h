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

double get_areaC(CIRCLE c);

double get_rC(CIRCLE c);

void kill_circ(CIRCLE c);

#endif