#include <stdio.h>
#include <stdlib.h>

double norm(double** matrix, int rows, int columns) {
	// Compute the norm of matrix
	double total = 0;
	int i;
	int j;
	for (i=0; i<rows; i++) {
		for (j=0; j<rows; j++) {
			total += matrix[i][j] * matrix[i][j];
		}
	}
	return total;
}

double f(double x) {
	//Network activation function
	return x;
}

int main(int argc,char* argv[])
{
// command line inputs here

if(!getnet(netfile))
	return 1;

if(!opendata(trainfile,testfile))
	return 1;

setup();

// train & test

// Determine where H+ starts
// Assumption: all nodes in H+ are grouped together,
// and they all point to all the classnodes
int hp;
hp = nodes - classnodes - indeg[nodes - classnodes][0];

for(i=0; i<edges; i++) {
	Dw[i] = 0;
}
// Outer Loop
do {
	// Run the network on the batch
	for(k=0; k<batchsize; k++) {
		if(!readdata(k, trainfile, trainstart)) {
			return 1;
		}

		for(j=datanodes; j<nodes; j++) {
			y[k][j] = 0;
			for(i=0; i<indeg[j]; i++) {
				y[k][j] += x[k][i] * w[inedge[j][i]];
			}
				x[k][j] = f(y[k][j]);
				df[k][j] = (f(y[k][j] + 0.001) - f(y[k][j]-0.001))/2;
		}

		//TODO: calculate epsilon[k][j](0)
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

	// Inner Loop
	do {
		for(k=0; k<batchsize; k++) {
			// Gamma = (X_tilde^T X_tilde)^(-1) (X_tilde^T Delta_W + Epsilon)
			for(j=nodes-classnodes; j<nodes; j++) {
				for(ell=0; ell<batchsize; ell++) {
					for(i=hp; i<nodes; i++) {
						g[k][j] += M[k][ell] * x[ell][i] * Dw[inedge[j][i]];
					}
					g[k][j] += M[k][ell] * epsilon[ell][j];
				}
			}

			// for i in H, k in K { gamma_i^k = df_i^k Sum_{i->j} w_{i->j} gamma_j^k }
			//for(i=nodes-classnodes-1; i>=datanodes; i--) {}
			//	for(j=
			//	g[k][i] += df[k][i] *

			// for j in H_-, k in K { delta_y_j^k = Sum_{i->j} x_tilde[i][k] (x_tilde[i] * gamma[j] - Delta_w[i][j]) }
			for (j=datanodes; j<hm; j++) {
				for (i=0; i<indeg[j]; i++) {
					for (ell=0; ell<batchsize; ell++) {
						dy[k][j] += x[k][innode[j][i]] * x[ell][innode[j][i]] * g[ell][j];
					}
					d[k][j] -= x[k][innode[j][i]] * D[w[inedge[j][i]]];
				}
			}

			// for j not in H_-, k in K { delta_y[j][k] = Sum_{i->j} (df[i][k] delta_y[i][k] w[i][j] + x_tilde[i][k] (x_tilde[i] * gamma[j] - Delta_w[i][j])
			for (j=hm; j<nodes; j++) {
				for (i=0; i<indeg[j]; i++) {
					for (ell=0; ell<batchsize; ell++) {
						dy[k][j] += x[k][innode[j][i]] * x[ell][innode[j][i]] * g[ell][j];
					}
					d[k][j] += df[k][innode[j][i]] * dy[k][innode[j][i]] * w[inedge[j][i]] - x[k][innode[j][i]] * D[w[inedge[j][i]]];
				}
			}
		}

		//TODO Update epsilon[k][j]


	} while (norm(Gamma_new - Gamma) > threshold);

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
} while (norm(Epsilon) > threshold);

closedata();

return 0;
}
