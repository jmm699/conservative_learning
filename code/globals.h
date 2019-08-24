#include <stdio.h>
#include <stdlib.h>
// Variables
extern double **x, **y, **g, *w, *Dw;
extern double **df, **dy;
extern double **M; //X_tilde^T X_tilde
extern double **e; //epsilon_j^k
extern double **scratch;
extern int *class;
extern int batchsize;
extern int datanodes,hiddennodes,classnodes,nodes,edges,maxindeg;
extern int *indeg,**inedge,**innode,*outdeg,**outedge,**outnode;
extern int hp, hm; //Stores how many nodes are in H+ and H-
extern FILE *trainptr,*testptr;
extern fpos_t trainstart,teststart;
// Functions
extern void setup();
extern int choleskyinverse(int m, double **a, double *d);
extern int getnet(char *netfile);
extern int opendata(char *trainfile, char *testfile);
extern void closedata();
extern int readdata(int k,FILE *dataptr,fpos_t datastart);
