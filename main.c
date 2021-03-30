#include <stdio.h>
#include <stdlib.h>
#include "diff.h"

int main(int argc, char *argv[]) {
	FILE *f1, *f2;
	int l1, l2;
	char **f1Matrix, **f2Matrix;
	int **LCSMatrix;
	f1Matrix = (char **)malloc(SIZE * sizeof(char *)); 
	f2Matrix = (char **)malloc(SIZE * sizeof(char *)); 
	LCSMatrix = (int **)malloc(SIZE * sizeof(int *));  
    for (int i = 0; i < SIZE; i++) {
    	f1Matrix[i] = (char *)malloc(SIZE * sizeof(char)); 
    	f2Matrix[i] = (char *)malloc(SIZE * sizeof(char));
        LCSMatrix[i] = (int *)malloc(SIZE * sizeof(int));
    }
    if(argv[argc - 2][0] == '>') {
    	f1 = fopen(argv[argc - 4], "r");
		f2 = fopen(argv[argc - 3], "r");
    } else {
    	f1 = fopen(argv[argc - 2], "r");
		f2 = fopen(argv[argc - 1], "r");
    }
	if(f1 == NULL)
		exit(1);
	if(f2 == NULL)
		exit(1);
	l1 = storeByLine(&f1Matrix, &f1) + 1; 
	l2 = storeByLine(&f2Matrix, &f2) + 1; 
	formLCSMatrix(&LCSMatrix, &f1Matrix, &f2Matrix, argc, argv, l1, l2);
	computeDiff(LCSMatrix, f1Matrix, f2Matrix, l1, l2, argc, argv);
	return 0;
}