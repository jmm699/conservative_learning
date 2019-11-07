#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
  //printf("Computing norm of:\n");
  //print_mat(matrix, rows, columns);
  for (i=0; i<rows; i++) {
    for (j=0; j<columns; j++) {
      total += matrix[i][j] * matrix[i][j];
    }
  }
  return total;
}

double vec_norm(double *vector, int len) {
  // Compute norm squared of vector
  int i;
  double tot=0;
  for (i=0; i<len; i++) {
    tot += vector[i] * vector[i];
  }
  return tot;
}
