#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

#include "SmuTreap.h"
typedef void* BOUNDING_BOX; 


void calcBB_c(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);
void calcBB_r(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);
void calcBB_t(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);
void calcBB_l(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);
void fCalcBB_individual(DescritorTipoInfo d, Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);



#endif