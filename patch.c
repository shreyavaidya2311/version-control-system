#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "diff.h"
#include "patch.h"

void printLine(FILE *f, char *line) {
	char l[SIZE];
	for(int i = 1; line[i] != '\0'; i++)
		fprintf(f, "%c", line[i]); 
	fprintf(f, "\n");
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

void patch(char* patchfile) {
	FILE *f, *fpatch;
	char **fMatrix;
	char filename[128];
	int i = 0;
	while(patchfile[i] != '.') {
		filename[i] = patchfile[i];
		i += 1;
	}
	filename[i] = '\0';
	strcat(filename, ".txt");
	f = fopen(filename, "w");
	fpatch = fopen(patchfile, "r");
	if(f == NULL)
		exit(1);
	if(fpatch == NULL)
		exit(1);
	printf("patching file %s\n", filename);
	patchFile(f, fpatch);
	fclose(f);
	fclose(fpatch);
	return;
}