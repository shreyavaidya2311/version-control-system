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

void add(fileStorage *f, char *filename) {
    node *newnode;
    char cwd[2048], nwd[2048], check[512]; 
    int flag = 0, i, j = 0;
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
    for(i = 0; i < f -> numberOfFiles; i++) {
        while(f -> arr[i] -> filename[j] != '_') {
            check[j] = f -> arr[i] -> filename[j];
            j += 1;
        }
        check[j] = '\0';
        strcat(check, ".txt");
        if(strcmp(check, filename) == 0) {
            flag = 1;
            break;
        }
        else
            flag = 0;
        strcpy(check, "");
    }
    if(flag == 1)
        f -> head = i;
    else {
        newnode = (node *)malloc(sizeof(node));
        if(!newnode)
            return;
        newnode -> patchfile = NULL;
        newnode -> filename = filename;
        newnode -> next = NULL;
        newnode -> version = 0;
        f -> arr[i] = newnode;
        f -> head = f -> numberOfFiles;
        f -> numberOfFiles += 1;
        printf("%s added to staging area\n", filename);
    }
    free(newnode);
    return;
}

 void reverse(char s[]) {
    int i, j;
    char c;
    for (i = 0, j = strlen(s)-1; i<j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
 }

void itoa(int n, char s[]) {
    int i, sign;
    if ((sign = n) < 0)  
        n = -n;         
    i = 0;
    do {       
        s[i++] = n % 10 + '0';   
    } while ((n /= 10) > 0);   
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
}

void commit(fileStorage *f, char *filename) {
    node *newnode, *p, *q;
    int i = 0;
    char file[512], patch[512], version[5];
    p = f -> arr[f -> head];
    while (p -> next)
        p = p -> next;
    itoa(p -> version, version);
    while(filename[i] != '.') {
        file[i] = filename[i];
        patch[i] = filename[i];
        i += 1;
    }
    file[i] = '\0';
    patch[i] = '\0';
    strcat(file, "_v");
    strcat(file, version);
    strcat(file, ".txt");
    strcat(patch, "_v");
    strcat(patch, version);
    strcat(patch, ".patch");
    newnode = (node *)malloc(sizeof(node));
    chdir(".stagingArea");
    diff(file, filename, 'c');
    if(!newnode)
        return;
    newnode -> patchfile = patch;
    newnode -> filename = file;
    newnode -> next = NULL;
    newnode -> version = f -> arr[f -> head] -> version + 1;
    q = f -> arr[f -> head];
    while (q -> next)
        q = q -> next;
    q -> next = newnode;
    return;
}

void push(fileStorage *f, char *filename) {
    char cwd[2048], nwd[2048];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        strcat(cwd, "/");
        strcat(cwd, filename);
    }
    printf("%s\n", f -> arr[f -> head] -> filename);
    patch(f -> arr[f -> head] -> patchfile);
    chdir("..");
    if(getcwd(nwd, sizeof(nwd)) != NULL) {
        strcat(nwd, "/");
        strcat(nwd, filename);
    }
    if(rename(cwd, nwd))
        printf("Could not push %s\n", filename);
    return;
}