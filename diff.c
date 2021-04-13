#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "diff.h"
#include "diffoutput.h"

int storeByLine(char ***fileMatrix, FILE **file) { 
	char **matrix = *fileMatrix;
	FILE *nfile = *file;
	int i = 0, j = 0;
	while(fscanf(nfile, "%c", &(matrix[i][j])) != EOF) {
		if(matrix[i][j] == '\n') {
			matrix[i][j] = '\0';
			i += 1;
			j = -1;
		}
		j += 1;
	}
	return i;
}

int max(int a, int b) {
  if (a > b)
    return a;
  return b;
}

int formLCSMatrix(int ***LCSMatrix, char ***f1Matrix, char ***f2Matrix, int l1, int l2) {
	char **f1, **f2;
	int i, j, x, y, **lcs = *LCSMatrix;
	f1 = *f1Matrix;
	f2 = *f2Matrix;
	for(i = 0; i <= l2; i++) 
		lcs[i][0] = 0;
	for(j = 0; j <= l1; j++) 
		lcs[0][j] = 0;
	for(x = 1; x <= l2; x++) { 
		for(y = 1; y <= l1; y++) { 
			if(strcmp(f1[y - 1], f2[x - 1]) == 0)
				lcs[x][y] = lcs[x - 1][y - 1] + 1;
			else
				lcs[x][y] = max(lcs[x][y - 1], lcs[x - 1][y]);
		} 
	}
	return lcs[x - 1][y - 1];
}

void computeDiff(int **LCSMatrix, char **f1Matrix, char **f2Matrix, int l1, int l2) {
	diffList diff;
	int iflag = 0, bflag = 0, wflag = 0;
	init(&diff);
	while(l1 != 0 || l2 != 0) {
		if(l1 == 0) {
			insertBegin(&diff, '+', f2Matrix[l2 - 1]);
			l2 -= 1;
		}
		else if(l2 == 0) {
			insertBegin(&diff, '-', f1Matrix[l1 - 1]);
			l1 -= 1;
		}
		else if(strcmp(f1Matrix[l1 - 1], f2Matrix[l2 - 1]) == 0) {
			insertBegin(&diff, ' ', f1Matrix[l1 - 1]);
            l1 -= 1;
            l2 -= 1;
		}
		else if(LCSMatrix[l1 - 1][l2] <= LCSMatrix[l1][l2 - 1]) {
			insertBegin(&diff, '+', f2Matrix[l2 - 1]);
			l2 -= 1;
      	}
    	else {
      		insertBegin(&diff, '-', f1Matrix[l1 - 1]);
			l1 -= 1;
		}          
	}
	printOutput(diff);
  return;
}

void makeDiffFile(int **LCSMatrix, char **f1Matrix, char **f2Matrix, int l1, int l2, char *filename) {
	diffList diff;
	char patchfilename[128];
	int i = 0;
	init(&diff);
	while(l1 != 0 || l2 != 0) {
		if(l1 == 0) {
			insertBegin(&diff, '+', f2Matrix[l2 - 1]);
			l2 -= 1;
		}
		else if(l2 == 0) {
			insertBegin(&diff, '-', f1Matrix[l1 - 1]);
			l1 -= 1;
		}
		else if(strcmp(f1Matrix[l1 - 1], f2Matrix[l2 - 1]) == 0) {
			insertBegin(&diff, ' ', f1Matrix[l1 - 1]);
            l1 -= 1;
            l2 -= 1;
		}
		else if(LCSMatrix[l1 - 1][l2] <= LCSMatrix[l1][l2 - 1]) {
			insertBegin(&diff, '+', f2Matrix[l2 - 1]);
			l2 -= 1;
      	}
    	else {
    		insertBegin(&diff, '-', f1Matrix[l1 - 1]);
			l1 -= 1;
    	}		        
	}
	while(filename[i] != '.') {
		patchfilename[i] = filename[i];
		i += 1;
	}
	patchfilename[i] = '\0';
	strcat(patchfilename, ".patch");
	createPatchFile(diff, patchfilename);
	printf("%s commited to working tree\n", filename); 
  return;
}

void diff(char *filename1, char *filename2, char o) {
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
    f1 = fopen(filename1, "r");
	f2 = fopen(filename2, "r");
	if(f1 == NULL)
		f1 = fopen(filename1, "w");
	if(f2 == NULL) {
		printf("%s does not exist\n", filename2); 
		exit(1);
	}
	l1 = storeByLine(&f1Matrix, &f1) + 1; 
	l2 = storeByLine(&f2Matrix, &f2) + 1; 
	formLCSMatrix(&LCSMatrix, &f1Matrix, &f2Matrix, l1, l2);
	if(o == 'p')
		computeDiff(LCSMatrix, f1Matrix, f2Matrix, l1, l2);
	else
		makeDiffFile(LCSMatrix, f1Matrix, f2Matrix, l1, l2, filename1);
	fclose(f1);
	fclose(f2);
	return;
}