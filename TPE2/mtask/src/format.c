#include "fs.h"

int format_main(int argc, char **argv) {
	char clean[512];
	int i;
	for (i = 0; i < 512; i++) {
		if (i < 20) {
			currentdir->subdirectories[i] = NULL;
			currentdir->files[i] = NULL;
		}
		clean[i] = '\0';
	}
	ata_write(ATA0, clean, 512, 0, 0);
	currentdir->subdircount = 0;
	currentdir->filecount = 0;
	return 0;
}
