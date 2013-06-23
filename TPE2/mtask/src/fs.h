#ifndef FS_H
#define FS_H

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
	struct file files[255];
	struct directory subdirectories[255];
} directory;

directory root = createFS();
directory currentdir;

directory createFS();

#endif
