#include "../include/disk.h"

int diskread_main(int argc, char **argv) {

	char buffer[512] = "";

	printk("Reading from disk.\n");
	ata_read(ATA0, buffer, 20, atoi(argv[1]), 0);
	printk("Read: %s \n", buffer);

	return 0;
}
