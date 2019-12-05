#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "globals.h"

void sgd_class_calculate_eps() {
  // Calculates the epsilon matrix for the class-based
  // conservative learning algorithm
  int j,k,jj;
  double delta = 1.0; // the threshold for the correct class
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
}


void sgd_vec_calculate_eps() {
  // Calculates the epsilon matrix for the vector-based
  // conservative learning algorithm
  int j,k,jj;
  for (k=0; k<batchsize; k++) {
    for (j=0; j<classnodes; j++) {
      jj=datanodes + hiddennodes + j;
      e[k][j] = teacher_vec[k][j] - y[k][jj];
    }
  }
}


void sgd_step(void (*eps_func)(void)) {
  int i,j,k,jj;
  // Performs one iteration of SGD to update the weights w
  // xtilde and ytilde should already be initialized using
  // the current weights.  The weights are updated in place

  double s = 1e-2; // learning rate

  // Step 1: compute epsilon[k][j]
  eps_func();

  // Step 2: initialize gammas at the output
  for(k=0; k<batchsize; k++) {
    // Gamma = (X_tilde^T X_tilde)^(-1) (X_tilde^T Delta_W + Epsilon)
    for(j=nodes-classnodes; j<nodes; j++) {
      jj = j-nodes+classnodes; // proper index for e
      g[k][j] = e[k][jj];
    }
  }

  // Step 3: backprop the gammas
  for(k=0; k<batchsize; k++) {
    // for i in H, k in K { gamma_i^k = df_i^k Sum_{i->j} w_{i->j} gamma_j^k }
    for(i=nodes-classnodes-1; i>=datanodes; i--) {
      g[k][i] = 0;
      for(j=0; j<outdeg[i]; j++) {
        g[k][i] += df[k][i] * w[outedge[i][j]] * g[k][outnode[i][j]];
      }
    }
  }

  // Step 4: update the weights
  for (j=datanodes; j<nodes; j++) {
    for (i=0; i<indeg[j]; i++) {
      dw[innode[j][i]] = 0;
      for (k=0; k<batchsize; k++) {
        dw[innode[j][i]] += x[k][innode[j][i]] * g[k][j];
      }
      w[innode[j][i]] += s*dw[innode[j][i]];
    }
  }

}


void vec_sgd_step() {
  sgd_step(sgd_vec_calculate_eps);
}


void class_sgd_step() {
  sgd_step(sgd_class_calculate_eps);
}
