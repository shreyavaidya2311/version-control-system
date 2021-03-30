typedef struct node {
  char op;
  char line[128];
  struct node *next;
} node;

typedef node *diffList;

void init(diffList *l);
void insertBegin(diffList *l, char op, char* line);
void printOutput(diffList l);
void createPatchFile(diffList l, char *filename);