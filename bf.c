#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include "bf.h"

bool*                               bf_vector;
int                                 n;   // nr vector bits 
int                                 m;   // nr elements
int                                 k;   // nr hash functions
int*                                training_set;
int*                                tab;
int                                 hash_mask;

int main(int argc, char**argv){
  if(argc!=3){
    printf("test: input error!\n");
    printf("usage: test [number of bloom filter vector bits] [number of elements]\n");
    exit(-1);
  }
  srand((unsigned)time(0));
  n=atoi(argv[1]);
  m=atoi(argv[2]);
  bf_create();
  bf_train();
  double p1=bf_query_false_positive_rate();
  double p2=bf_theoretical_false_positive_rate();
  printf("%d, %d, %8.3f, %8.3f, %8.3f, %8.3f\n",n,m, (double)n/m,p1,p2, ((double)p1/p2));
  bf_destroy();
  return 0;
}  
