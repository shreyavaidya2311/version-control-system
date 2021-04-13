#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vcs.h"

int main(int argc, char *argv[]) {
	fileStorage f;
	initFileStorage(&f);
	char command[128], args[128];
	int v;
	do {
		scanf("%s", command);
		if(strcmp(command, "newrepo") == 0) {
			scanf("%s", args);
			newRepository(args);
		}
		else if(strcmp(command, "changerepo") == 0) {
			scanf("%s", args);
			changeRepository(args);
		}
		else if(strcmp(command, "init") == 0) {
			initialize();
		}
		else if(strcmp(command, "add") == 0) {
			scanf("%s", args);
			add(&f, args);
		}
		else if(strcmp(command, "commit") == 0) {
			scanf("%s", args);
			commit(&f, args);
		}
		else if(strcmp(command, "push") == 0) {
			scanf("%s", args);
			push(&f, args);
		}
		else if(strcmp(command, "revert") == 0) {
			scanf("%s", args);
			scanf("%d", &v);
			revert(&f, args, v);
		}
		else if(strcmp(command, "exit") == 0)
			exit(1);
	} while(1);
	return 0;
}