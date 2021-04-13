#include "diffoutput.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void init(diffList *l) {
  *l = NULL;
  return;
}

void insertBegin(diffList *l, char op, char* line) {
  node *newnode, *q;
  newnode = (node *)malloc(sizeof(node));
  if (newnode) {
    newnode -> op = op;
    strcpy(newnode -> line, line);
    newnode -> next = NULL;
  } 
  else
    return;
  newnode -> next = *l;
  *l = newnode;
  return;
}

void red () {
  printf("\033[1;31m");
  return;
}

void green () {
  printf("\033[1;32m");
  return;
}

void white () {
  printf("\033[0;37m");
  return;
}

void printOutput(diffList l) {
  node *p = l;
  int additions = 0, deletions = 0;
  while(p) {
    if(p -> op == '+') {
      green();
      additions += 1;
    }
    else if(p -> op == '-') {
      red();
      deletions += 1;
    }
    printf("%c%s\n", p -> op, p -> line);
    p = p -> next;
    white(); 
  }
  printf("%d additions(+), %d deletions(-)\n", additions, deletions);
  return;
}

void createPatchFile(diffList l, char *filename) {
  FILE *f;
  f = fopen(filename, "w");
  node *p = l;
  while(p -> next) {
    fprintf(f, "%c%s\n", p -> op, p -> line);
    p = p -> next; 
  }
  fprintf(f, "%c%s", p -> op, p -> line);
  fclose(f);
  return;
}