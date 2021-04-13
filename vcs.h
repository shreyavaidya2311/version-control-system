typedef struct node {
	char patchfile[256], filename[256];
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
void add(fileStorage *f, char *filename);
void commit(fileStorage *f, char *filename);
void push(fileStorage *f, char *filename);
void revert(fileStorage *f, char *filename, int version);
void printWarranty();
void versionDiff(fileStorage *f, char *filename, int version1, int version2);