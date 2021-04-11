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

void printOutput(diffList l) {
  node *p = l;
  int additions = 0, deletions = 0;
  while(p) {
    if(p -> op == '+')
      additions += 1;
    else if(p -> op == '-')
      deletions += 1;
    printf("%c%s\n", p -> op, p -> line);
    p = p -> next; 
  }
  printf("%d additions, %d deletions\n", additions, deletions);
  return;
}

void createPatchFile(diffList l, char *filename) {
  FILE *f;
  f = fopen(filename, "w");
  node *p = l;
  while(p) {
    fprintf(f, "%c%s\n", p -> op, p -> line);
    p = p -> next; 
  }
  fclose(f);
  return;
}