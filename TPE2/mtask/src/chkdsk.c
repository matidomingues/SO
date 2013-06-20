#include "disk.h"

int chkdsk_main(int argc, char **argv) {
	ata_checkDrive(ATA0);
	return 0;
}
