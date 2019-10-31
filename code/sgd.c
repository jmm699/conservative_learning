#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"

void sgd_step() {
  int i,j,k,jj;
  // Performs one iteration of SGD to update the weights w
  // xtilde and ytilde should already be initialized using
  // the current weights.  The weights are updated in place

  double delta = 1.0; // the threshold for the correct class
  double kappa = 10; // stiffness parameter
  // calculate epsilon[k][j]

  // Step 1: compute epsilon[k][j]
  for (k=0; k<batchsize; k++) {
    for (j=0; j<classnodes; j++) {
      jj=datanodes + hiddennodes + j;
      if (j == class[k]) {
        e[k][j] = (y[k][jj] > delta) ? 0 : delta - y[k][jj];
      } else {
        e[k][j] = (y[k][jj] < 0) ? 0 : -y[k][jj];
      }
    }
  }

  // Step 2: setup M = X_tilde^T X_tilde and invert
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

  // Step 3: initialize gammas at the output
  for(k=0; k<batchsize; k++) {
    // Gamma = (X_tilde^T X_tilde)^(-1) (X_tilde^T Delta_W + Epsilon)
    for(j=nodes-classnodes; j<nodes; j++) {
      jj = j-nodes+classnodes; // proper index for e
      g[k][j] = 0;
      for(i=0; i<batchsize; i++) {
        g[k][j] += M[k][i] * e[i][jj];
      }
    }
  }

  // Step 4: backprop the gammas
  for(k=0; k<batchsize; k++) {
    // for i in H, k in K { gamma_i^k = df_i^k Sum_{i->j} w_{i->j} gamma_j^k }
    for(i=nodes-classnodes-1; i>=datanodes; i--) {
      for(j=0; j<outdeg[i]; j++) {
        g[k][i] += df[k][i] * w[outedge[i][j]] * g[k][outnode[i][j]];
      }
    }
  }

  // Step 5: update the weights
  for (j=datanodes; j<nodes; j++) {
    for (i=0; i<indeg[j]; i++) {
      for (k=0; k<batchsize; k++) {
        dw[innode[j][i]] += x[k][innode[j][i]] * g[k][j];
      }
      w[innode[j][i]] += dw[innode[j][i]];
    }
  }

}
