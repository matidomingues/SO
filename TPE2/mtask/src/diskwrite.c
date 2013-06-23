#include "../include/disk.h"

int diskwrite_main(int argc, char **argv) {

	typedef struct{
		char* filename;
		int filesize;
		int startingssector;
	} file;


	printk("Writing to disk.\n");
	ata_write(ATA0, argv[1], 20, atoi((argv[2])), 0);
	printk("Wrote: %s\n", argv[1]);

	return 0;
}
