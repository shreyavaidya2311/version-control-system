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
  while(p) {
    if(p -> op == 'n')
      printf("\\ No newline at end of file\n");
    else
      printf("%c%s\n", p -> op, p -> line);
    p = p -> next; 
  }
  return;
}



