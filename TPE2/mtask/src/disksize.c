#include "disk.h"

int disksize_main(int argc, char **argv) {
	//TODO INCOMPLETO
	char buffer[1] = "a";
	int i;
	for (i = 0; i < 10; ++i) {
		ata_read(ATA0, buffer, 512, i, 0);
		printk("Read: %s \n", buffer);
	}
	return 0;
}
