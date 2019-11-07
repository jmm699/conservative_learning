#ifndef GLOBAL_H
#define GLOBAL_H
#include <stdio.h>
#include <stdlib.h>
// Variables
extern double **x, **y, **g, *w, *dw, *Dw;
extern double **df, **dy;
extern double **M; //X_tilde^T X_tilde
extern double **e; //epsilon_j^k
extern double **scratch;
extern double **teacher_vec;
extern double *teacher_w;
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
extern void sgd_step(void (*eps_func)(void));
extern void class_sgd_step();
extern void vec_sgd_step();
extern void cl_inner_loop(int max_iter, void (*eps_func)(void));
extern void cl_class_inner_loop(int max_iter);
extern void cl_vec_inner_loop(int max_iter);
extern void print_vec(double *vector, int len);
extern void print_mat(double **matrix, int rows, int columns);
extern double norm(double **matrix, int rows, int columns);
extern double vec_norm(double *vector, int len);
#endif
