#include "fs.h"

int ls_main(int argc, char **argv) {
	int i;
	char flag = 0;
	for (i = 0; i < MAX_DIRECTORIES; i++) {

		if (currentdir->subdirectories[i] != NULL) {
			flag = 1;
			cprintk(LIGHTBLUE, BLACK, "%s ",
					currentdir->subdirectories[i]->name);
		}
	}
	for (i = 0; i < MAX_FILES; i++) {
		if (currentdir->files[i] != NULL) {
			flag = 1;
			cprintk(LIGHTGREEN, BLACK, "%s ", currentdir->files[i]->name);
		}
	}
	if (flag) {
		printk("\n");
	}
	return 0;
}
