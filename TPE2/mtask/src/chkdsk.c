#include "disk.h"

int chkdsk_main(int argc, char **argv) {

	ata_checkDrive(ATA0);

	char *send = "Hola";
	char *rcv = malloc(sizeof(char) * 5);

	printk("Writing to disk.\n");
	ata_write(ATA0, send, 4, 2, 0);
	printk("Wrote %s.\n", send);

	printk("Reading from disk.\n");
	ata_read(ATA0, rcv, 4, 2, 0);
	printk("Read: %s \n", rcv);

	return 0;
}
