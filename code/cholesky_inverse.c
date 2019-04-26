// in-place inverse of matrix a
// returns 0 if non-invertible
// array d of diagonal elements is scratch space that must be allocated

int choleskyinverse(int m,double **a,double *d)
{
int i,j,k;
double s;

for(i=0;i<m;++i)
	{
	if(a[i][i]==0.)
		return 0;
		
	d[i]=a[i][i];
	}

for(i=0;i<m;++i)
	{
	a[i][i]=d[i];
	for(j=0;j<i;++j)
		a[i][i]-=a[j][i]*a[j][i];
		
	a[i][i]=sqrt(a[i][i]);
	
	for(k=i+1;k<m;++k)
		{
		a[i][k]=a[k][i];
		for(j=0;j<i;++j)
			a[i][k]-=a[j][i]*a[j][k];
			
		if(a[i][i]==0.)
			return 0;
		
		a[i][k]/=a[i][i];
		}
	}
	
for(i=0;i<m;++i)
	{
	if(a[i][i]==0.)
		return 0;
		
	a[i][i]=1./a[i][i];
	}
	
for(k=m-1;k>0;--k)
for(i=k-1;i>=0;--i)
	{
	a[i][k]*=-a[k][k];
	for(j=i+1;j<k;++j)
		a[i][k]-=a[i][j]*a[j][k];
		
	a[i][k]*=a[i][i];
	}
	
for(j=0;j<m;++j)
for(i=j;i<m;++i)
	{
	s=.0;
	for(k=i;k<m;++k)
		s+=a[j][k]*a[i][k];
		
	a[j][i]=s;
	}
	
for(i=0;i<m;++i)
for(j=i+1;j<m;++j)
	a[j][i]=a[i][j];
	
return 1;
}