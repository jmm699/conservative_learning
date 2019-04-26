// global declarations

double **x,**y,**g,*w;
int *class;

// example of memory allocation

void setup()
{
int k;

x=malloc(batchsize*sizeof(double*));
y=malloc(batchsize*sizeof(double*));
g=malloc(batchsize*sizeof(double*));

w=malloc(edges*sizeof(double));

for(k=0;k<batchsize;++k)
	{
	x[k]=malloc(nodes*sizeof(double));
	y[k]=malloc(nodes*sizeof(double));
	g[k]=malloc(nodes*sizeof(double));
	}
	
class=malloc(batchsize*sizeof(int));
}