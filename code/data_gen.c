#include <stdio.h>
#include <stdlib.h>

int datanodes = 4;
int lines = 20;

int main() {
  int i,j;
  int count;
  double num;
  for (j=0; j<lines; j++) {
    count = 0;
    for (i=0; i<datanodes; i++) {
      num = (double) rand() / RAND_MAX;
      if (num > 0.5) {
        printf("1.0 ");
        count++;
      } else {
        printf("0.0 ");
      }
    }
    if (count > 2) {
      printf("1\n");
    }
    else {
      printf("0\n");
    }
  }
}
