#include "fs.h"

int touch_main(int argc, char **argv) {
	if (argc < 2) {
		printk("touch: Falta el parametro archivo\n");
	} else {
		int i;
		for (i = 1; i < argc; ++i) {
			openFile(argv[i]);
		}
	}
	return 0;
}
