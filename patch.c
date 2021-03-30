#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "diff.h"

void printLine(FILE *f, char *line) {
	char l[128];
	for(int i = 1; line[i] != '\0'; i++)
		fprintf(f, "%c", line[i]); 
	return;
}

void patchFile(FILE *f, FILE *fpatch) {
	char **fileMatrix;
	int len;
	fileMatrix = (char **)malloc(SIZE * sizeof(char *));   
    for (int i = 0; i < SIZE; i++) 
    	fileMatrix[i] = (char *)malloc(SIZE * sizeof(char)); 
	len = storeByLine(&fileMatrix, &fpatch) + 1;
	for(int j = 0; j < len; j++) {
		if(fileMatrix[j][0] == '-' || fileMatrix[j][0] == '\\')
			continue;
		printLine(f, fileMatrix[j]);
	}
	return;
}

int main(int argc, char *argv[]) {
	FILE *f, *fpatch;
	char **fMatrix;
	f = fopen("a.txt", "r+");
	fpatch = fopen(argv[1], "r");
	if(f == NULL)
		exit(1);
	if(fpatch == NULL)
		exit(1);
	printf("patching file a.txt\n");
	patchFile(f, fpatch);
	return 0;
}
