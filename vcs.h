typedef struct node {
	FILE *patchfile;
	char* filename;
	struct node *next, *prev;
} node;

typedef struct fileStorage {
	node **arr;
	int head, numberOfFiles;
} fileStorage;	

void initFileStorage(fileStorage *f);
void newRepository(char *repository);
void changeRepository(char *repository);
void initialize();
void add(fileStorage f, char *filename);
void commit(fileStorage *f, char *filename);
void push(char *filename);