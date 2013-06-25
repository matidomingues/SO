#ifndef FS_H
#define FS_H

#include "disk.h"
#include "kernel.h"

#define MAX_SECTORS 15000
#define SECTOR_SIZE 512
#define MAX_FILES 20
#define MAX_DIRECTORIES 20
#define DEFAULT_FILESIZE 512
#define FILETABLE_SECTOR_START 1
#define NAME_LENGTH 64
#define FILE_SYSTEM_VERSION "TP-SO-FILESYSTEM-0.1"

extern struct directory* currentdir;

typedef struct file {
	char name[NAME_LENGTH];
	int disksector;
	int size;
	struct directory* parent;
} file;

typedef struct directory {
	char name[NAME_LENGTH];
	int filecount;
	int subdircount;
	struct directory * parent;
	struct file * files[MAX_FILES];
	struct directory * subdirectories[MAX_DIRECTORIES];
	int disksector;
} directory;

directory* getDirectoryFromName(char*);

#endif
