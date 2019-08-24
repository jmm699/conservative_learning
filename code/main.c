#include <stdio.h>
#include <stdlib.h>
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

void print_mat(double** matrix, int rows, int columns) {
  // Prints the matrix
  int i;
  for (i=0;i<rows;i++) {
    print_vec(matrix[i], columns);
  }
}

double norm(double** matrix, int rows, int columns) {
  // Compute the norm of matrix
  double total = 0;
  int i;
  int j;
  printf("Computing norm of:\n");
  print_mat(matrix, rows, columns);
  for (i=0; i<rows; i++) {
    for (j=0; j<columns; j++) {
      total += matrix[i][j] * matrix[i][j];
    }
  }
  return total;
}

double f(double x) {
  //Network activation function
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
int i,j,k,ell,ii,jj;
double delta = 1.0;
double threshold = 0.001;

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

for(i=0; i<edges; i++) {
  w[i] = 2*(double) rand() / RAND_MAX - 1;
  Dw[i] = 0;
}
printf("Weights:\n");
print_vec(w, edges);
// Outer Loop
do {
  // Run the network on the batch
  for(k=0; k<batchsize; k++) {
    //if(!readdata(k, trainfile, trainstart)) {
    if(!readdata(k, trainptr, trainstart)) {
      printf("problem reading data");
      return 1;
    }

    for(j=datanodes; j<nodes; j++) {
      y[k][j] = 0;
      for(i=0; i<indeg[j]; i++) {
        y[k][j] += x[k][i] * w[inedge[j][i]];
      }
        x[k][j] = f(y[k][j]);
        df[k][j] = fprime(y[k][j]);
    }
    // Initialize dy to 0
    for(j=0; j<nodes; j++) {
      dy[k][j] = 0;
    }


  }

  // Setup M = X_tilde^T X_tilde and invert
  for(i=0; i<batchsize; i++) {
    for(j=0; j<batchsize; j++) {
      for(k=hp; k<nodes-classnodes; k++) {
        M[i][j] += x[i][k] * x[j][k];
      }
    }
  }
  //Invert
  double *d;
  d = malloc(batchsize*sizeof(double));
  choleskyinverse(batchsize, M, d);
  free(d);

  // Epsilon = epsilon[k][classnodes:]
  // delta_W = X_tilde (X_tilde^T X_tilde)^(-1) Epsilon

  // Clear the scratch space
  for (i=0; i<batchsize; i++) {
    for (j=0; j<nodes; j++) {
      scratch[i][j] = 0;
    }
  }
  // Clear gamma
  for (k=0; k<batchsize; k++) {
    for (j=0; j<nodes; j++) {
      g[k][j] = 0;
    }
  }

  printf("Top of outer loop:\n");
  printf("M = X^T X:\n");
  print_mat(M, batchsize, batchsize);
  printf("Enter to continue...");
  getchar();

  // Inner Loop
  do {
    // calculate epsilon[k][j]
    for (k=0; k<batchsize; k++) {
      for (j=0; j<classnodes; j++) {
        jj=datanodes + hiddennodes + j;
        printf("Class: %d\n", class[k]);
        if (j == class[k]) {
          e[k][j] = (y[k][jj] > delta) ? 0 : delta - y[k][jj];
        } else {
          e[k][j] = (y[k][jj] < 0) ? 0 : y[k][jj];
        }

        for (i=0; i<indeg[datanodes + hiddennodes + j]; i++) {
          e[k][j] += df[k][innode[datanodes + hiddennodes + j][i]] * w[inedge[datanodes+hiddennodes+j][i]] * dy[k][innode[datanodes + hiddennodes + j][i]];
        }
      }
    }
    printf("Top of inner loop:\n");
    printf("x:\n");
    print_mat(x, batchsize, nodes);
    printf("y:\n");
    print_mat(y, batchsize, nodes);
    printf("Epsilon:\n");
    print_mat(e, batchsize, classnodes);
    printf("Enter to continue...");
    getchar();

    // Copy Gamma (old) into scratch
    for (k=0; k<batchsize; k++) {
      for (j=0; j<classnodes; j++) {
        scratch[k][j] = g[k][j+datanodes+hiddennodes];
      }
    }

    for(k=0; k<batchsize; k++) {
      // Gamma = (X_tilde^T X_tilde)^(-1) (X_tilde^T Delta_W + Epsilon)
      for(j=nodes-classnodes; j<nodes; j++) {
        jj = j-nodes+classnodes; // proper index for e
        g[k][j] = 0;
        for(ell=0; ell<batchsize; ell++) {
          for(i=0; i<indeg[j]; i++) {
            g[k][j] += M[k][ell] * x[ell][innode[j][i]] * Dw[inedge[j][i]];
          }
          //printf("g[%d][%d] += M[%d][%d] * e[%d][%d] = %lf * %lf\n",
          //    k, j, k, ell, ell, jj, M[k][ell], e[ell][jj]);
          //printf("Enter to continue...");
          //getchar();
          g[k][j] += M[k][ell] * e[ell][jj];
        }
      }
    }

    for(k=0; k<batchsize; k++) {
      // for i in H, k in K { gamma_i^k = df_i^k Sum_{i->j} w_{i->j} gamma_j^k }
      for(i=nodes-classnodes-1; i>=datanodes; i--) {
        for(j=0; j<outdeg[i]; j++) {
          g[k][i] += df[k][i] * w[outedge[i][j]] * g[k][outnode[i][j]];
        }
      }
    }

      printf("Gamma:\n");
      print_mat(g, batchsize, nodes);
      printf("Enter to continue...");
      getchar();

      // for j in H_-, k in K { delta_y_j^k = Sum_{i->j} x_tilde[i][k] (x_tilde[i] * gamma[j] - Delta_w[i][j]) }
    for(k=0; k<batchsize; k++) {
      for (j=datanodes; j<hm; j++) {
        for (i=0; i<indeg[j]; i++) {
          for (ell=0; ell<batchsize; ell++) {
            dy[k][j] += x[k][innode[j][i]] * x[ell][innode[j][i]] * g[ell][j];
          }
          dy[k][j] -= x[k][innode[j][i]] * Dw[inedge[j][i]];
        }
      }
    }

      // for j not in H_-, k in K { delta_y[j][k] = Sum_{i->j} (df[i][k] delta_y[i][k] w[i][j] + x_tilde[i][k] (x_tilde[i] * gamma[j] - Delta_w[i][j])
    for(k=0; k<batchsize; k++) {
      for (j=hm; j<nodes; j++) {
        for (i=0; i<indeg[j]; i++) {
          for (ell=0; ell<batchsize; ell++) {
            dy[k][j] += x[k][innode[j][i]] * x[ell][innode[j][i]] * g[ell][j];
          }
          dy[k][j] += df[k][innode[j][i]] * dy[k][innode[j][i]] * w[inedge[j][i]] - x[k][innode[j][i]] * Dw[inedge[j][i]];
        }
      }
    }
    printf("Bottom of inner loop:\n");
    printf("dy:\n");
    print_mat(dy, batchsize, nodes);



    //WEIGHTS NEED TO GET UPDATED BY delta_W AFTER EACH INNER LOOP
    for (k=0; k<batchsize; k++) {
      for (j=0; j<classnodes; j++) {
        scratch[k][j] -= g[k][j];
      }
    }
  } while (norm(scratch, batchsize, classnodes) > threshold);

  // Delta_w[i][j] = x_tilde[i] * gamma[j]
  for (j=datanodes; j<nodes; j++) {
    for (i=0; i<indeg[j]; i++) {
      for (k=0; k<batchsize; k++) {
        Dw[innode[j][i]] += x[k][innode[j][i]] * g[k][j];
      }
    }
  }
  // Re-run the network, update x_tilde, y_tilde, df
  // Compute Epsilon

} while (norm(e, batchsize, classnodes) > threshold);

closedata();

return 0;
}
