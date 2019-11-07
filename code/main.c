#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"

//Network activation function and its derivative
double f(double x) {
  if (x>0) {
    return x;
  } else {
    return 0.0;
  }
}

double fprime(double x) {
  if (x>0) {
    return 1.0;
  } else {
    return 0.0;
  }
}

int main(int argc,char* argv[])
{
// command line inputs here
char *netfile, *trainfile, *testfile;
int i,j,k,ell,jj;
double delta = 1.0;
double threshold = 1e-8;
double r = 1; // relaxation parameter
double kappa = 10; // stiffness parameter
srand(time(NULL));

if(argc==4)
  {
  netfile=argv[1];
  trainfile=argv[2];
  testfile=argv[3];
  }
else
  {
  fprintf(stderr,"expected 3 arguments: netfile, trainfile, testfile\n");
  return 1;
  }

if(!getnet(netfile))
  return 1;

if(!opendata(trainfile,testfile))
  return 1;

setup();

// train & test

// Initialize weights
// Read in initial weights from file
char weightfile[] = "vec_weights";
FILE *weight_file;
weight_file=fopen(weightfile,"r");
if (!weight_file) {
  printf("Could not open weight file\n");
  return 1;
}

for(i=0; i<edges; i++) {
//  w[i] = 2*(double) rand() / RAND_MAX - 1;
  fscanf(weight_file,"%lf",&teacher_w[i]); //Exact correct weight
  // Apply small perturbation to weights
  w[i] = teacher_w[i] + 2 * (double) rand() / RAND_MAX - 1;
  dw[i] = 0;
  Dw[i] = 0;
}
// Outer Loop
printf("Iteration\tCost\tGradient norm\nWeight distance");
int iter = 0;
double cost, weight_dist, grad_dist;
do {
  // Read in new batch
  batchsize = 1; // required for sgd
  for (k=0; k<batchsize; k++) {
    if(!readdata(k, trainptr, trainstart)) {
      printf("problem reading data");
      return 1;
    }
  }
  // Run the network on the batch
  for(k=0; k<batchsize; k++) {
    for(j=datanodes; j<nodes; j++) {
      y[k][j] = 0;
      for(i=0; i<indeg[j]; i++) {
        y[k][j] += x[k][innode[j][i]] * w[inedge[j][i]];
      }
        x[k][j] = f(y[k][j]);
        df[k][j] = fprime(y[k][j]);
    }
    // Initialize dy to 0
    for(j=0; j<nodes; j++) {
      dy[k][j] = 0;
    }
  }
  //printf("Top of outer loop:\n");
  //printf("x:\n");
  //print_mat(x, batchsize, nodes);
  //printf("y:\n");
  //print_mat(y, batchsize, nodes);
  //printf("Weights:\n");
  //print_vec(w, edges);
  //printf("Enter to continue...");
  //getchar();

  //cl_inner_loop(-1);
  vec_sgd_step();
  cost = norm(e, batchsize, classnodes);
  grad_dist = vec_norm(dw, edges);
  for (i=0; i<edges; i++)
    Dw[i] = w[i] - teacher_w[i];
  weight_dist = vec_norm(Dw, edges);
  printf("%d\t%lf\t%lf\t%lf\n", iter, cost, grad_dist, weight_dist);
  iter++;

} while ((cost > threshold) && (iter<1000));

closedata();

return 0;
}
