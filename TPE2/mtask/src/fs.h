#ifndef FS_H
#define FS_H

#define MAX_SECTORS 15000
#define SECTOR_SIZE 512
#define MAX_FILES 100
#define MAX_DIRECTORIES 100

typedef struct file {
	char name[128];
	int disksector;
	int size = 512;
} file;

typedef struct directory {
	char name[64];
	int filecount;
	int subdircount;
	struct directory parent;
	struct file files[MAX_FILES];
	struct directory subdirectories[255];
} directory;

directory root = createFS();
directory currentdir;

directory createFS();

#endif
