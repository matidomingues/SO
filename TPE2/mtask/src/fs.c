#include "fs.h"

directory createFS() {
	directory root;
	root.name = "root";
	root.filecount = 0;
	root.subdircount = 0;
	root.parent = 0;
	int i;
	for (i = 0; i < 255; ++i) {
		root.files[i] = 0;
		root.subdirectories[i] = 0;
	}

	return root;
}
