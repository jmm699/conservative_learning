#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"

void cl_inner_loop(int max_iter) {
  // Runs the conservative learning inner-loop
  // to update the weights w of the network in place
  // Will terminate after max_iter number of iterations
  // (set to a negative number to run indefinitely)
  // xtilde and ytilde should be initialized with
  // the current weights, along with df
  if (max_iter == 0)
    return;
  int i,j,k,ell,jj;
  double delta = 1.0;
  double threshold = 0.001;
  double r = 1; // relaxation parameter
  double kappa = 10; // stiffness parameter

  // Setup M = X_tilde^T X_tilde and invert
  for(i=0; i<batchsize; i++) {
    for(j=0; j<batchsize; j++) {
      M[i][j] = 0;
      for(k=hp; k<nodes-classnodes; k++) {
        M[i][j] += x[i][k] * x[j][k];
      }
    }
    // Add in stiffness parameter
    M[i][i] += 1.0/kappa;
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
  // Clear dy
  for(k=0; k<batchsize; k++) {
    for(j=0; j<nodes; j++)
      dy[k][j] = 0;
  }

  // Inner Loop
  do {
    // calculate epsilon[k][j]
    for (k=0; k<batchsize; k++) {
      for (j=0; j<classnodes; j++) {
        jj=datanodes + hiddennodes + j;
        if (j == class[k]) {
          e[k][j] = (y[k][jj] > delta) ? 0 : delta - y[k][jj];
        } else {
          e[k][j] = (y[k][jj] < 0) ? 0 : y[k][jj];
        }

        for (i=0; i<indeg[datanodes + hiddennodes + j]; i++) {
          e[k][j] -= df[k][jj] * w[inedge[jj][i]] * dy[k][innode[jj][i]];
        }
      }
    }
    printf("Top of inner loop:\n");
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
            dy[k][j] += r * x[k][innode[j][i]] * x[ell][innode[j][i]] * g[ell][j];
          }
          dy[k][j] -= r * x[k][innode[j][i]] * Dw[inedge[j][i]];
          dy[k][j] += (1-r) * dy[k][j];
        }
      }
    }

      // for j not in H_-, k in K { delta_y[j][k] = Sum_{i->j} (df[i][k] delta_y[i][k] w[i][j] + x_tilde[i][k] (x_tilde[i] * gamma[j] - Delta_w[i][j])
    for(k=0; k<batchsize; k++) {
      for (j=hm; j<nodes; j++) {
        for (i=0; i<indeg[j]; i++) {
          for (ell=0; ell<batchsize; ell++) {
            dy[k][j] += r * x[k][innode[j][i]] * x[ell][innode[j][i]] * g[ell][j];
          }
          dy[k][j] += r * df[k][innode[j][i]] * dy[k][innode[j][i]] * w[inedge[j][i]];
          dy[k][j] -= r * x[k][innode[j][i]] * Dw[inedge[j][i]];
          dy[k][j] += (1-r) * dy[k][j];
        }
      }
    }
    printf("Bottom of inner loop:\n");
    printf("dy:\n");
    print_mat(dy, batchsize, nodes);



    for (k=0; k<batchsize; k++) {
      for (j=0; j<classnodes; j++) {
        jj = classnodes + hiddennodes + j;
        scratch[k][j] -= g[k][jj];
      }
    }
  } while ((norm(scratch, batchsize, classnodes) > threshold) && (max_iter-- > 0));

  // Update the weights
  for (j=datanodes; j<nodes; j++) {
    for (i=0; i<indeg[j]; i++) {
      for (k=0; k<batchsize; k++) {
        dw[innode[j][i]] += x[k][innode[j][i]] * g[k][j] - Dw[innode[j][i]];
      }
      Dw[innode[j][i]] += dw[innode[j][i]];
      w[innode[j][i]] += dw[innode[j][i]];
    }
  }

}
