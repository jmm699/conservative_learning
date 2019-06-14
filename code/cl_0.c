#include <stdio.h>
#include <stdlib.h>

// global variables

int datanodes,hiddennodes,classnodes,nodes,edges,maxindeg;
int *indeg,**inedge,**innode,*outdeg,**outedge;



// read network architecture and allocate memory

int getnet(char *netfile)
{
int i,d,in;
FILE *fp;

fp=fopen(netfile,"r");
if(!fp)
	{
	printf("netfile not found\n");
	return 0;
	}

fscanf(fp,"%d%d%d",&datanodes,&hiddennodes,&classnodes);

nodes=datanodes+hiddennodes+classnodes;

indeg=malloc(nodes*sizeof(int));   //Number of incoming edges
outdeg=malloc(nodes*sizeof(int));  //Number of outgoing edges
innode=malloc(nodes*sizeof(int*)); //List of incoming nodes
inedge=malloc(nodes*sizeof(int*)); //List of incoming edges
outedge=malloc(nodes*sizeof(int*));//List of outgoing edges

for(i=0;i<nodes-classnodes;++i)
	outdeg[i]=0;

maxindeg=0;
edges=0;
for(i=datanodes;i<nodes;++i)
	{
	fscanf(fp,"%*d%d",&indeg[i]);

	if(indeg[i]>maxindeg)
		maxindeg=indeg[i];

	innode[i]=malloc(indeg[i]*sizeof(int));
	inedge[i]=malloc(indeg[i]*sizeof(int));

	for(d=0;d<indeg[i];++d)
		{
		inedge[i][d]=edges++;

		fscanf(fp,"%d",&innode[i][d]);

		++outdeg[innode[i][d]];
		}
	}

fclose(fp);

for(i=0;i<nodes-classnodes;++i)
	{
	outedge[i]=malloc(outdeg[i]*sizeof(int));
	outdeg[i]=0;
	}

for(i=datanodes;i<nodes;++i)
for(d=0;d<indeg[i];++d)
	{
	in=innode[i][d];
	outedge[in][outdeg[in]++]=inedge[i][d];
	}

return 1;
}


// for development, use main() to test the parts

int main(int argc,char* argv[])
{
char *netfile;
int i;

if(argc==2)
	{
	netfile=argv[1];
	}
else
	{
	fprintf(stderr,"expected 1 argument: netfile\n");
	return 1;
	}

if(!getnet(netfile))
	return 1;

for(i=datanodes;i<nodes;++i) {
	printf("node %3d   has in-degree %3d\n",i,indeg[i]);
        for (int in=0; in<indeg[i]; in++) {
            printf("Incoming edge: %d -> %d == %d\n", in,i,inedge[i][in]);
                    }
                    }

printf("\n");

for(i=0;i<nodes-classnodes;++i) {
    printf("node %3d   has out-degree %3d\n",i,outdeg[i]);
    //Want to loop through innodes, if i is there, print
    //i -> outnode == edge number
for(int out=datanodes;out<nodes;++out)
for(int d=0;d<indeg[out];++d)
	{
            if (i==innode[out][d]) {
                printf("Outgoing edge: %d -> %d == %d\n",i,out,inedge[out][d]);
	}
}
}

return 0;
}
