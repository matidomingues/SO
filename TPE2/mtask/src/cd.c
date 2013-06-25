#include "fs.h"
#include "shell.h"

int cd_main(int argc, char **argv) {
	directory* elem;
	if (strcmp(argv[1], "..") == 0) {
		if (currentdir->parent != NULL) {
			currentdir = currentdir->parent;
			promptdirup(prompt,strlen(prompt)-strlen(currentdir->name)-4);
		}
	} else {
		if ((elem = getDirectoryFromName(argv[1])) != NULL) {
			currentdir = elem;
			strins(prompt,currentdir->name,strlen(prompt)-2);
		} else {
			printk("cd: %s: No existe el archivo o directorio\n", argv[1]);
		}
	}
	return 0;
}
