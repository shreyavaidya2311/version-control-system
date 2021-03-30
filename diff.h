#define SIZE 256

int storeByLine(char ***fileMatrix, FILE **file);
void printCharMatrix(char **fileMatrix);
void printIntMatrix(int **LCSMatrix, int l1, int l2);
int formLCSMatrix(int ***LCSMatrix, char ***f1Matrix, char ***f2Matrix, int argc, char **argv, int l1, int l2);
void computeDiff(int **LCSMatrix, char **f1Matrix, char **f2Matrix, int l1, int l2, int argc, char **argv);