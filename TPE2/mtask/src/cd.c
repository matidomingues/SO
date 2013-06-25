#include "fs.h"

int cd_main(int argc, char **argv) {
	directory* elem;
	if (strcmp(argv[1], "..") == 0) {
		if (currentdir->parent != NULL) {
			currentdir = currentdir->parent;
		} else {
			//return 0;
		}
	} else {
		if ((elem = getDirectoryFromName(argv[1])) != NULL) {
			currentdir = elem;
		} else {
			printk("cd: %s: No existe el archivo o directorio\n", argv[1]);
		}
	}
	return 0;
}
