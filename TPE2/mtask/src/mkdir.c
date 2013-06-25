#include "fs.h"

int mkdir_main(int argc, char **argv) {
	directory* elem = getDirectoryFromName(argv[1]);
	if (elem == NULL) {
		elem = createDirectory(argv[1]);
		allocateDirectory(elem);
		addDirectoryChild(elem, currentdir);
	} else {
		printk("No se puede crear el directorio '%s': Ya existe\n", argv[1]);
	}
	return 0;
}
