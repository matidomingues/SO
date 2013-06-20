#include "disk.h"

int diskwrite_main(int argc, char **argv) {

	printk("Writing to disk.\n");
	ata_write(ATA0, argv[1], 20, 700, 0);
	printk("Wrote: %s\n", argv[1]);

	return 0;
}
