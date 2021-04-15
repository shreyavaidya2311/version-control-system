#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "vcs.h"

int main(int argc, char *argv[]) {
	fileStorage f;
	initFileStorage(&f);
	char command[128], args[128];
	int v1, v2;
	printWarranty();
	do {
		printf("> ");
		scanf("%s", command);
		if(strcmp(command, "newrepo") == 0) {
			scanf("%s", args);
			newRepository(args);
		}
		else if(strcmp(command, "changerepo") == 0) {
			scanf("%s", args);
			changeRepository(args);
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
			scanf("%d", &v1);
			revert(&f, args, v1);
		}
		else if(strcmp(command, "diff") == 0) {
			scanf("%s", args);
			scanf("%d", &v1);
			scanf("%d", &v2);
			versionDiff(&f, args, v1, v2);
		}
		else if(strcmp(command, "help") == 0)
			printHelp();
		else if(strcmp(command, "exit") == 0)
			exit(1);
	} while(1);
	return 0;
}