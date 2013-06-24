#ifndef FS_H
#define FS_H

#define MAX_SECTORS 15000
#define SECTOR_SIZE 512
#define MAX_FILES 20
#define MAX_DIRECTORIES 20
#define DEFAULT_FILESIZE 512
#define FILETABLE_SECTOR_START 10

typedef struct file {
	char name[128];
	int disksector;
	int size = 512;
	struct directory parent;
} file;

typedef struct directory {
	char name[64];
	int filecount;
	int subdircount;
	struct directory parent;
	struct file files[MAX_FILES];
	struct directory subdirectories[MAX_DIRECTORIES];
	int disksector;
} directory;

directory root = createFS();
directory currentdir;

directory createFS();

#endif
