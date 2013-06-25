#include "disk.h"

int chkdsk_main(int argc, char **argv) {
	disk_identify();
	return 0;
}
