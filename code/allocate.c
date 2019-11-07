#include "globals.h"
// global declarations

double **x,**y,**g,*w,*dw,*Dw;
double **df, **dy;
double **M; //X_tilde^T X_tilde
double **e; //epsilon_j^k
double **scratch;
double **teacher_vec;
double *teacher_w;
int *class;
int batchsize = 30;

// example of memory allocation

void setup()
{
int k;

x=malloc(batchsize*sizeof(double*));
y=malloc(batchsize*sizeof(double*));
dy=malloc(batchsize*sizeof(double*));
df=malloc(batchsize*sizeof(double*));
g=malloc(batchsize*sizeof(double*));
M=malloc(batchsize*sizeof(double*));
e=malloc(batchsize*sizeof(double*));
teacher_vec=malloc(batchsize*sizeof(double*));
scratch=malloc(batchsize*sizeof(double*));

w=malloc(edges*sizeof(double));
dw=malloc(edges*sizeof(double));
Dw=malloc(edges*sizeof(double));
teacher_w=malloc(edges*sizeof(double));

for(k=0;k<batchsize;++k)
  {
  x[k]=malloc(nodes*sizeof(double));
  y[k]=malloc(nodes*sizeof(double));
  dy[k]=malloc(nodes*sizeof(double));
  df[k]=malloc(nodes*sizeof(double));
  g[k]=malloc(nodes*sizeof(double));
  M[k]=malloc(batchsize*sizeof(double));
  e[k]=malloc(classnodes*sizeof(double));
  teacher_vec[k]=malloc(classnodes*sizeof(double));
  scratch[k]=malloc(nodes*sizeof(double));
  }

class=malloc(batchsize*sizeof(int));
}
