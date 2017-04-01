/*
AUTHOR - PRIYANSH RASTOGI
SGG4 PROJECT 2: MULTITHREAD SORTING

HOW TO USE:
1. USE MAKEFILE TO COMPILE
2. ./msort <num1> <num2>...
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int* arr;
int* solarr; //solution array

typedef struct {
	int start;
	int end;
}parameters;

void *sort(void *params){
	parameters* param = (parameters*)params;
	int begin = param->start, end = param->end;

	//sort
	for (int i = begin + 1; i <= end - 1; i++) {
  	int j = i;
		while ( j > begin && arr[j] < arr[j-1]) {
    	int temp = arr[j];
      arr[j]   = arr[j-1];
    	arr[j-1] = temp;
			j--;
  	}
  }

  for(int i = begin; i < end; i++) {
	   printf("%d ", arr[i]);
  }
	printf("\n");
	pthread_exit(0);
}

void *merge(void* param) {
	int* n = (int*)param;
	int i = 0, j = *n/2, k = 0;

  while (i < *n/2 && j < *n) {
    if (arr[i] < arr[j]) {
      solarr[k++] = arr[i++];
    }
    else
      solarr[k++] = arr[j++];
    }
    while (i < *n/2)
      solarr[k++] = arr[i++];
    while (j < *n)
      solarr[k++] = arr[j++];

    pthread_exit(0);
}

int main(int argc, char* argv[]){

  int n = argc-1;
  //allocate memory for array
	arr = (int *)malloc(n*sizeof(n));
	solarr = (int *)malloc(n*sizeof(n));

	for(int i = 0; i < n; i++){
    arr[i]=atoi(argv[i+1]);
	}
	//create parameters
	parameters *left_params = (parameters *) malloc(sizeof(parameters));
	left_params->start = 0;
	left_params->end = n/2;

  parameters *right_params = (parameters *) malloc(sizeof(parameters));
	right_params->start = n/2;
	right_params->end = n;

	//create threads
	pthread_t tid_left, tid_right, tid_merge;
	pthread_attr_t attr;

	pthread_attr_init(&attr);

	pthread_create(&tid_left, &attr, sort, left_params);
	pthread_create(&tid_right, &attr, sort, right_params);

	pthread_join(tid_left, NULL);
	pthread_join(tid_right, NULL);

	pthread_create(&tid_merge, &attr, merge, &n);

	pthread_join(tid_merge, NULL);

	//output
	for(int i = 0; i < n; i++) {
		printf("%d ", solarr[i]);
  }

  printf("\n");

	return 0;
}
