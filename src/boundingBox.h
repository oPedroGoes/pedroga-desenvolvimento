#ifndef BOUNDINGBOX_H
#define BOUNDINGBOX_H

//c: 1, r: 2, t: 3, l: 4
typedef void* Info;
typedef int DescritorTipoInfo;
typedef void* BB; 


void calcBB_c(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);
void calcBB_r(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);
void calcBB_t(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);
void calcBB_l(Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);
void *fCalcBB_individual(DescritorTipoInfo d, Info i, double *bbA_x, double *bbA_y, double *bbA_w, double *bbA_h);
void uniaoBB(BB bb_dest, BB bb1, BB bb2);
;
;

#endif