#include "globals.h"
#include <stdio.h>
#include <stdlib.h>
// global declarations

FILE *trainptr,*testptr;
fpos_t trainstart,teststart;


// initialize pointers for reading from training- and test-data files

int opendata(char *trainfile, char *testfile)
{
int datanodes1,datanodes2,classnodes1,classnodes2;

trainptr=fopen(trainfile,"r");
if(!trainptr)
  {
  printf("trainfile not found\n");
  return 0;
  }

testptr=fopen(testfile,"r");
if(!testptr)
  {
  printf("testfile not found\n");
  return 0;
  }

fscanf(trainptr,"%d",&datanodes1);
fscanf(testptr,"%d",&datanodes2);

fscanf(trainptr,"%d",&classnodes1);
fscanf(testptr,"%d",&classnodes2);

if(datanodes!=datanodes1 || datanodes!=datanodes2)
  {
  printf("datanodes = %d\n", datanodes);
  printf("datanodes1 = %d\n", datanodes1);
  printf("datanodes2 = %d\n", datanodes2);
  printf("trainfile or testfile is not compatible with netfile\n");
  return 0;
  }

if(classnodes1!=classnodes2)
  {
  printf("trainfile and testfile differ in class number\n");
  return 0;
  }

fgetpos(trainptr,&trainstart);
fgetpos(testptr,&teststart);

return 1;
}


// close data streams

void closedata()
{
fclose(trainptr);
fclose(testptr);
}


// reading training or test data from stream
// k is the data item index
// each line of data file consists of data vector (doubles) followed by class label (int)

int readdata(int k,FILE *dataptr,fpos_t datastart)
{
int i;

for(i=0;i<datanodes;++i)
  if(fscanf(dataptr,"%lf",&x[k][i])==EOF)
    {
    fsetpos(dataptr,&datastart);
    return 0;
    }

fscanf(dataptr,"%d",&class[k]);

return 1;
}
