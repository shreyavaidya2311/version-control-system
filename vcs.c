#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "diff.h"
#include "patch.h"
#include "vcs.h"

//initializes the fileStorage struct
void initFileStorage(fileStorage *f) {
    f -> arr = (node **)malloc(sizeof(node *) * 512);
    f -> head = 0;
    f -> numberOfFiles = 0;
    for(int i = 0; i < 512; i++) 
        f -> arr[i] = NULL;
    return;
}

//initializes the staging area inside a repository
void initialize() {
    if(!mkdir(".stagingArea", 0755))
        printf("Repository initialized\n");
    else {
        printf("Initialization failed\n");
        exit(1);
    }
    return;
}

//creates a new repository
void newRepository(char *repository) {
    char* directory = repository;
    if(!mkdir(directory, 0777)) {
        printf("%s created\n", directory);
        chdir(repository);
        initialize();
        chdir("..");
    }
    else 
        printf("Unable to create repository\n");
    return;
}

//changes current directory to given repository
void changeRepository(char *repository) {
	if(!chdir(repository)) {
        printf("Changed repository to %s\n", repository);
    	system("dir");
    	return;
	}
    else 
        printf("Unable to change repository\n");
    return;
}

//adds given file to staging area and sets the head of the fileStorage accordingly
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
        strcpy(newnode -> patchfile, "");
        strcpy(newnode -> filename, filename);
        newnode -> next = NULL;
        newnode -> version = 0;
        f -> arr[i] = newnode;
        f -> arr[i] -> next = NULL;
        f -> head = f -> numberOfFiles;
        f -> numberOfFiles += 1;
    }
    printf("%s added to staging area\n", filename);
    return;
}

void reverse(char s[]) {
    int i, j;
    char c;
    for(i = 0, j = strlen(s) - 1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
    return;
 }

//function to convert integer to string
void itoa(int n, char s[]) {
    int i, sign;
    if((sign = n) < 0)  
        n = -n;         
    i = 0;
    do {       
        s[i++] = n % 10 + '0';   
    } while((n /= 10) > 0);   
    if(sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
    return;
}

//creates new node to store file (every version) and attaches it to the fileStorage
void commit(fileStorage *f, char *filename) {
    node *newnode, *q, *p;
    int i = 0;
    char file[512], patch[512], version[5];
    if(f -> arr[f -> head] == NULL) {
        printf("Nothing to commit, empty working tree\n");
        return;
    }
    itoa(f -> arr[f -> head] -> version + 1, version);
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
    if(!newnode)
        return;
    strcpy(newnode -> patchfile, patch);
    strcpy(newnode -> filename, file);
    newnode -> version = f -> arr[f -> head] -> version + 1;
    q = f -> arr[f -> head];
    newnode -> next = q;
    f -> arr[f -> head] = newnode; 
    diff(newnode -> filename, filename, 'c');  
    return;
}

//patches the file with its current version and moves it back to root of repository
void push(fileStorage *f, char *filename) {
    node *p;
    if(f -> arr[f -> head] == NULL) {
        printf("Nothing to push, empty working tree\n");
        return;
    }
    p = f -> arr[f -> head];
    char cwd[2048], nwd[2048];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        strcat(cwd, "/");
        strcat(cwd, filename);
    }
    patch(p -> filename, p -> patchfile);
    chdir("..");
    if(getcwd(nwd, sizeof(nwd)) != NULL) {
        strcat(nwd, "/");
        strcat(nwd, filename);
    }
    if(rename(cwd, nwd)) {
        printf("Could not push %s\n", filename);
        return;
    }
    printf("%s pushed successfully\n", filename);
    return;
}

//reverts current version of file to given version of file
void revert(fileStorage *f, char *filename, int version) {
    node *p;
    if(f -> arr[f -> head] == NULL) {
        printf("Head not set correctly\n");
        return;
    }
    if(version > f -> arr[f -> head] -> version || version < 1) {
        printf("Invalid version number\n");
        return;
    }
    char cwd[2048], nwd[2048], dir[2048];
    if(getcwd(cwd, sizeof(cwd)) != NULL) {
        strcat(cwd, "/");
        strcpy(nwd, cwd);
        strcat(nwd, ".stagingArea/");
        strcat(nwd, filename);
        strcat(cwd, filename);
        if(rename(cwd, nwd)) {
            printf("%s failed to move to staging area\n", filename);
            return;
        }
    }
    chdir(".stagingArea");
    p = f -> arr[f -> head]; 
    while(p -> version != version)
        p = p -> next;
    patch(filename, p -> patchfile);
    if(rename(nwd, cwd)) {
        printf("Could not revert %s\n", filename);
        return;
    }
    printf("%s reverted to version %d\n", filename, version);
    chdir("..");
    return;
}

//compares and generates the difference between two versions of a file
void versionDiff(fileStorage *f, char *filename, int version1, int version2) {
    char filename1[512], filename2[512], v1[5], v2[5];
    int i = 0;
    if(f -> arr[f -> head] == NULL) {
        printf("Head not set correctly\n");
        return;
    }
    if(version1 > f -> arr[f -> head] -> version || version1 < 1) {
        printf("Invalid version number\n");
        return;
    }
    if(version2 > f -> arr[f -> head] -> version || version2 < 1) {
        printf("Invalid version number\n");
        return;
    }
    itoa(version1, v1);
    itoa(version2, v2);
    while(filename[i] != '.') {
        filename1[i] = filename[i];
        filename2[i] = filename[i];
        i += 1;
    }
    filename1[i] = '\0';
    filename2[i] = '\0';
    strcat(filename1, "_v");
    strcat(filename1, v1);
    strcat(filename1, ".txt");
    strcat(filename2, "_v");
    strcat(filename2, v2);
    strcat(filename2, ".txt");
    chdir(".stagingArea");
    diff(filename1, filename2, 'p'); 
    chdir("..");
    return;
}

void printWarranty() {
  printf("\n   vcs - Version Control System\n");
  printf("   Copyright 2021 Shreya Vaidya, MIS - 111903156\n\n");
  printf("   This program is free software; you can redistribute it and/or modify it\n   in line with any application you wish to build.\n\n");
  printf("   This program is distributed in the hope that it will be useful,\n   but WITHOUT ANY WARRANTY; without even the implied warranty of\n   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.\n");
  printf("   To find out more about the commands available, type `help`\n\n");
  return;
}

void printHelp() {
  printf("   Commands for Version Control System\n\n");
  printf("   newrepo `repository_name`                            : creates new repository and initializes staging area\n");
  printf("   changerepo `repository_name`                         : changes current directory to repository entered\n");
  printf("   add `file_name`                                      : adds file to staging area and sets the head accordingly\n");
  printf("   commit `file_name`                                   : creates node with filename and patchfile of the current version and attaches it to the file storage\n");
  printf("   push `file_name`                                     : current version file in staging area is patched and original file is moved back to root of repository\n");
  printf("   revert `file_name` `version_number`                  : file is reverted back to a previous version retaining all versions of the file\n");
  printf("   diff `file_name` `version_number1` `version_number2` : shows difference between two versions of a file \n");
  return;
}