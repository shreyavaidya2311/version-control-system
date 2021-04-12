typedef struct node {
	char *patchfile, *filename;
	struct node *next, *prev;
	int version;
} node;

typedef struct fileStorage {
	node **arr;
	int head, numberOfFiles;
} fileStorage;	

void initFileStorage(fileStorage *f);
void newRepository(char *repository);
void changeRepository(char *repository);
void initialize();
void add(fileStorage *f, char *filename);
void commit(fileStorage *f, char *filename);
void push(fileStorage *f, char *filename);