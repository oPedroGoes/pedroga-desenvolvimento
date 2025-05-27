#ifndef circulo_h
#define circulo_h

typedef void* CIRCLE;

CIRCLE create_circle(int i, double x, double y, double r, char *cb, char *cp);

int get_idC(CIRCLE c);
void set_idC(CIRCLE c, int i);

float get_XC(CIRCLE c);
void set_xC(CIRCLE c, float x);

float get_YC(CIRCLE c);
void set_yC(CIRCLE c, float y);

char* get_cbC(CIRCLE c);
void set_cbC(CIRCLE c, char* cb);

char* get_cpC(CIRCLE c);
void set_cpC(CIRCLE c, char* cp);

float get_areaC(CIRCLE c);

float get_rC(CIRCLE c);

void kill_circ(CIRCLE c);

#endif