#include "fs.h"

int rm(int argc, char **argv) {
	directory* elem = getDirectoryFromName(argv[1]);
	if (elem == NULL) {
		file* data = getFileFromName(argv[1]);
		if (data != NULL) {
			deleteFile(data);
		} else {
			printk("rm: No se puede remover '%s': No existe el archivo o directorio.\n", argv[1]);
			return 0;
		}
	}
	if (elem->parent == NULL) {
		return -1;
	}
	deleteDirectory(elem);
	return 0;
}
