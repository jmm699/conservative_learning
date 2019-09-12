// Teacher network
// writes the weights used to weights file
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"

void print_vec(double *vector, int len) {
  // Prints a vector
  int i;
  for (i=0; i<len; i++) {
    printf("%lf ", vector[i]);
    if ((len == nodes) && ((i==datanodes-1) || (i==datanodes + hiddennodes-1))) {
      printf(" |  ");
    }
  }
  printf("\n");
}

double f( double x) {
  // Activation function
  if (x > 0) {
    return x;
  } else {
    return 0;
  }
}

int main() {
  int i,j,k,class;
  char netfile[] = "net3_4_2";
  int batchsize = 30;
  srand(time(NULL));
  if (!getnet(netfile)) {
    return 1;
  }

  // Allocate
  double *x, *y, *w;
  x=malloc(nodes*sizeof(double));
  y=malloc(nodes*sizeof(double));
  w=malloc(edges*sizeof(double));

  // Pick some weights
  for(i=0; i<edges; i++) {
    w[i] = 2 * (double) rand() / RAND_MAX - 1;
  }

  // Generate teaching data
  printf("3 2\n"); //format for train/testfile
  k=0;
  while (k<batchsize) {
    // Generate inputs
    for (i=0; i<datanodes; i++) {
      x[i] = 10*(double) rand() / RAND_MAX - 5;
    }
    // Run the network
    for(j=datanodes; j<nodes; j++) {
      y[j] = 0;
      for(i=0; i<indeg[j]; i++) {
        y[j] += x[innode[j][i]] * w[inedge[j][i]];
      }
        x[j] = f(y[j]);
    }
    // Determine class
    class = (y[nodes-2] > y[nodes-1]) ? 0 : 1;
    // Check if this is a good example to teach from
    // i.e. network produces negative output for the wrong output node,
    // output over 1 for correct node
    if (!class) { //class = 0 case
      if ((y[nodes-2] > 1) && (y[nodes-1]<0)) {
        // Print inputs and class
        printf("%lf %lf %lf %d\n", x[0], x[1], x[2], class);
        k++;
      }
    } else { //class=1 case
      if ((y[nodes-2] < 0) && (y[nodes-1] > 1)) {
        // Print inputs and class
        printf("%lf %lf %lf %d\n", x[0], x[1], x[2], class);
        k++;
      }
    }
  }
  printf("Weights: ");
  print_vec(w, edges);

  return 0;
}
