#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diff.h"
#include "patch.h"
#include "vcs.h"

void initFileStorage(fileStorage *f) {
    f -> arr = (node **)malloc(sizeof(node *) * 512);
    f -> head = 0;
    f -> numberOfFiles = 0;
    return;
}

void newRepository(char *repository) {
    char* directory = repository;
    if(!mkdir(directory, 0777))
        printf("%s created\n", directory);
    else 
        printf("Unable to create repository\n");
    chdir(repository);
    return;
}

void changeRepository(char *repository) {
	if(!chdir(repository)) {
    	system("dir");
    	return;
	}
    else 
        printf("Unable to change repository\n");
    return;
}

void initialize() {
    if(!mkdir(".stagingArea", 0755))
        printf("Repository initialized\n");
    else {
        printf("Initialization failed\n");
        exit(1);
    }
    return;
}

void add(fileStorage f, char *filename) {
    node *newnode;
    char cwd[2048], nwd[2048]; 
    int flag = 0, i;
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        strcat(cwd, "/");
        strcpy(nwd, cwd);
        strcat(nwd, ".stagingArea/");
        strcat(nwd, filename);
        strcat(cwd, filename);
        if(rename(cwd, nwd)) {
            printf("Addition to staging area failed\n");
            return;
        }
    }
    for(i = 0; i < f.numberOfFiles; i++) {
        if(strcmp(f.arr[i] -> filename, filename) == 0) {
            flag = 1;
            break;
        }
        else
            flag = 0;
    }
    if(flag == 1)
        f.head = i;
    else {
        newnode = (node *)malloc(sizeof(node));
        if(!newnode)
            return;
        newnode -> patchfile = NULL;
        newnode -> filename = filename;
        printf("%s\n", newnode -> filename);
        newnode -> next = NULL;
        newnode -> prev = NULL;
        f.arr[i] = newnode;
        f.numberOfFiles += 1;
        f.head = f.numberOfFiles;
        printf("%s added to staging area\n", filename);
    }
    printf("%d\n", f.head);
    free(newnode);
    return;
}

void commit(fileStorage *f, char *filename) {
    chdir(".stagingArea");
    diff("buffer.txt", filename, 'c');
    return;
}

void push(char *filename) {
    char cwd[2048], nwd[2048];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        strcat(cwd, "/");
        strcat(cwd, filename);
    }
    patch("buffer.patch");
    chdir("..");
    if(getcwd(nwd, sizeof(nwd)) != NULL) {
        strcat(nwd, "/");
        strcat(nwd, filename);
    }
    if(rename(cwd, nwd))
        printf("Could not push %s\n", filename);
    return;
}