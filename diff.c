#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "diff.h"
#include "diffoutput.h"

int storeByLine(char ***fileMatrix, FILE **file) { //Function to read content from file and store in 2D Array
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

void printCharMatrix(char **fileMatrix) {
	int i, j;
	for(i = 0; i < SIZE; i++) {
		for(j = 0; j < SIZE; j++) {
			if(fileMatrix[i][j] == '\0')
				break;
			else
				printf("%c", fileMatrix[i][j]);
		}
	}
	printf("\n");	
	return;
}

void printIntMatrix(int **LCSMatrix, int l1, int l2) {
	int i, j;
	for(i = 0; i <= l2; i++) {
		for(j = 0; j <= l1; j++) 
			printf("%d ", LCSMatrix[i][j]);
		printf("\n");
	}	
	return;
}

int max(int a, int b) {
  if (a > b)
    return a;
  return b;
}

int iFlagCompare(char *line1, char *line2) { //Function to compare two lines of two files case insensitively
	char l1[SIZE], l2[SIZE];
	int i, j;
	for(i = 0; line1[i] != '\0'; i++) {
		if(line1[i] >= 'A' && line1[i] <= 'Z') 
			l1[i] = tolower(line1[i]);
		else
			l1[i] = line1[i];
	} 
	l1[i] = '\0';
	for(j = 0; line2[j] != '\0'; j++) {
		if(line2[j] >= 'A' && line2[j] <= 'Z') 
			l2[j] = tolower(line2[j]);
		else
			l2[j] = line2[j];
	}
	l2[j] = '\0';
	return strcmp(l1, l2);
}

int wFlagCompare(char *line1, char *line2) { //Function to compare two lines of two files ignoring all spaces
	char l1[SIZE], l2[SIZE];
	int i, j;
	for(i = 0; line1[i] != '\0'; i++) {
		if(line1[i] == ' ') 
			continue;
		else
			l1[i] = line1[i];
	} 
	l1[i] = '\0';
	for(j = 0; line2[j] != '\0'; j++) {
		if(line2[j] == ' ') 
			continue;
		else
			l2[j] = line2[j];
	}
	l2[j] = '\0';
	return strcmp(l1, l2);
}

int bFlagCompare(char *line1, char *line2) { //Function to compare two lines of two files converting 
	char l1[SIZE], l2[SIZE];				 //all spaces to one space
	int i, j, count = 0;
	for(i = 0; line1[i] != '\0'; i++) {
		if(line1[i] == ' ' && count == 0) {
			l1[i] = line1[i];
			count = 1;
		} 
		else if(line1[i] == ' ' && count != 0) {
			continue;
		}
		else
			l1[i] = line1[i];
	} 
	l1[i] = '\0';
	count = 0;
	for(j = 0; line2[j] != '\0'; j++) {
		if(line2[j] == ' ' && count == 0) {
			l2[j] = line2[j];
			count = 1;
		} 
		else if(line2[j] == ' ' && count != 0) {
			continue;
		}
		else
			l2[j] = line2[j];
	} 
	l2[j] = '\0';
	return strcmp(l1, l2);
}

// Compares two lines according to the flags being set
int compareLine(char *line1, char *line2, int iflag, int bflag, int wflag) {
	if(iflag == 1 && bflag == 0 && wflag == 0) 
		return iFlagCompare(line1, line2);
	else if(iflag == 0 && bflag == 1 && wflag == 0) 
		return bFlagCompare(line1, line2);
	else if(iflag == 0 && bflag == 0 && wflag == 1) 
		return wFlagCompare(line1, line2);
	else 
		return strcmp(line1, line2);
}

// Sets flags as per entered by user
// void setFlags(int argc, char **argv, int *iflag, int *bflag, int *wflag) {
// 	if(argc == 3)
// 		return;
// 	for(int i = 1; i < argc - 2; i++) {
// 		switch(argv[i][1]) { 
// 			case 'i': *iflag = 1;
// 						break;
// 			case 'b': *bflag = 1;
// 						break;
// 			case 'w': *wflag = 1;
// 						break;
// 			default: break;
// 		}
// 	}
// 	return;	
// }

//Forms Longest Subsequent Matrix from the two file matrices
int formLCSMatrix(int ***LCSMatrix, char ***f1Matrix, char ***f2Matrix, int l1, int l2) {
	char **f1, **f2;
	int i, j, x, y, **lcs = *LCSMatrix;
	int iflag = 0, bflag = 0, wflag = 0;
	// setFlags(argc, argv, &iflag, &bflag, &wflag);
	f1 = *f1Matrix;
	f2 = *f2Matrix;
	for(i = 0; i <= l2; i++) 
		lcs[i][0] = 0;
	for(j = 0; j <= l1; j++) 
		lcs[0][j] = 0;
	for(x = 1; x <= l2; x++) { 
		for(y = 1; y <= l1; y++) { 
			if(compareLine(f1[y - 1], f2[x - 1], iflag, bflag, wflag) == 0)
				lcs[x][y] = lcs[x - 1][y - 1] + 1;
			else
				lcs[x][y] = max(lcs[x][y - 1], lcs[x - 1][y]);
		} 
	}
	return lcs[x - 1][y - 1];
}

//Creates a diffList to print final diff output as a unified output
void computeDiff(int **LCSMatrix, char **f1Matrix, char **f2Matrix, int l1, int l2) {
	diffList diff;
	int iflag = 0, bflag = 0, wflag = 0;
	// setFlags(argc, argv, &iflag, &bflag, &wflag);
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
		else if(compareLine(f1Matrix[l1 - 1], f2Matrix[l2 - 1], iflag, bflag, wflag) == 0) {
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
	// if(argv[argc - 2][0] == '>') 
		// createPatchFile(diff, argv[argc - 1]);
	// else 
		printOutput(diff);
  return;
}

//Creates a diffList to print final diff output as a unified output
void makeDiffFile(int **LCSMatrix, char **f1Matrix, char **f2Matrix, int l1, int l2, char *filename) {
	diffList diff;
	int iflag = 0, bflag = 0, wflag = 0;
	char patchfilename[128];
	int i = 0;
	// setFlags(argc, argv, &iflag, &bflag, &wflag);
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
		else if(compareLine(f1Matrix[l1 - 1], f2Matrix[l2 - 1], iflag, bflag, wflag) == 0) {
			insertBegin(&diff, ' ', f1Matrix[l1 - 1]);
            l1 -= 1;
            l2 -= 1;
		}
		else if(LCSMatrix[l1 - 1][l2] <= LCSMatrix[l1][l2 - 1]) {
			insertBegin(&diff, '+', f2Matrix[l2 - 1]);
			l2 -= 1;
      	}
    	else 
    		continue;          
	}
	while(filename[i] != '.') {
		patchfilename[i] = filename[i];
		i += 1;
	}
	patchfilename[i] = '\0';
	strcat(patchfilename, ".patch");
	createPatchFile(diff, patchfilename);
	printOutput(diff);
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
	if(f2 == NULL)
		exit(1);
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