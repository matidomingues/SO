#include "../include/fs.h"

char sectors[MAX_SECTORS];

directory* currentdir = NULL;

void createFS() {
	directory* root;
	memcpy(root->name, "root", sizeof("root")+1);
	
	root->filecount = 0;
	root->subdircount = 0;
	root->parent = NULL;
	int i;
	for (i = 0; i < 255; ++i) {
		root->files[i] = NULL;
		root->subdirectories[i] = NULL;
	}
	currentdir = root;
}

void initSectors() {
	int i;
	for (i = 0; i < MAX_SECTORS; i++) {
		sectors[i] = 0;
	}
}

int getSector(int sector) {
	return sectors[sector];
}

int getFileFreeSector(int ammount) {
	int i, start, w;
	for (i = FILETABLE_SECTOR_START; i < MAX_SECTORS; i++) {
		if (!getSector(i)) {
			start = i;
			for (w = 0; w < ammount; i++) {
				if (getSector(i + w) != 0) {
					break;
				}
			}
			if (w == ammount) {
				return start;
			}
		}
	}
	return -1;
}

int getFreeSector() {
	int i;
	for (i = 0; i < MAX_SECTORS; i++) {
		if (!sectors[i]) {
			return i;
		}
	}
	return -1;
}

void setSector(int sector) {
	sectors[sector] = 1;
}

void clearSector(int sector, int ammount) {
	int i;
	for (i = 0; i < ammount; i++) {
		sectors[sector + i] = 0;
	}
}

file* getFileFromName(char* name) {
	int i;
	file* elem;
	for (i = 0; i < MAX_FILES; i++) {
		elem = currentdir->files[i];
		if (elem != NULL) {
			if (strcmp(elem->name, name) == 0) {
				return elem;
			}
		}
	}
	return NULL;
}

directory* getDirectoryFromName(char* name) {
	int i;
	directory* elem;
	for (i = 0; i < MAX_DIRECTORIES; i++) {
		elem = currentdir->subdirectories[i];
		if (elem != NULL) {
			if (strcmp(elem->name, name) == 0) {
				return elem;
			}
		}
	}
	return NULL;
}

file* createFile(char* name, int size) {
	file* elem;
	int i, sector;
	memcpy(elem->name, name, strlen(name) + 1);
	sector = getFileFreeSector((int) (size / 512) + 1);
	for (i = 0; i < (int) (size / 512) + 1; i++) {
		setSector(sector + i);
	}
	elem->disksector = sector;
	elem->parent = currentdir;
	elem->size = size;
	return elem;
}

void allocateFile(file* elem) {
	int loc = 0;
	ata_write(ATA0, elem->name, sizeof(elem->name), elem->disksector, loc);
	loc += sizeof(elem->name);
	ata_write(ATA0, (char*)elem->size, sizeof(elem->size), elem->disksector, loc);
	loc += sizeof(elem->size);
	if (elem->parent != NULL) {
		ata_write(ATA0, (char*)elem->parent->disksector, sizeof(elem->parent->disksector),
				elem->disksector, loc);
	} else {
		ata_write(ATA0, (char*)-1, sizeof(int), elem->disksector, loc);
	}
}

void addChild(file* elem, directory* parent) {
	int i = 0;
	parent->filecount += 1;
	for (i = 0; i < MAX_FILES; i++) {
		if (parent->files[i] == NULL) {
			parent->files[i] = elem;
		}
	}
}

file* openFile(char* name) {
	file* elem = getFileFromName(name);
	if (elem == NULL) {
		elem = createFile(name, DEFAULT_FILESIZE);
		allocateFile(elem);
	}
	return elem;
}

int ls() {
	int i;
	printk("Directory: %s\n", currentdir->name);
	for (i = 0; i < MAX_DIRECTORIES; i++) {
		if (currentdir->subdirectories[i] != NULL) {
			printk("%s\n", currentdir->subdirectories[i]->name);
		}
	}
	for (i = 0; i < MAX_FILES; i++) {
		if (currentdir->files[i] != NULL) {
			printk("%s\n", currentdir->files[i]->name);
		}
	}
	return 1;
}

void cd(char* arg) {
	directory* elem;
	if (strcmp(arg, "..") == 0) {
		if (currentdir->parent != NULL) {
			currentdir = currentdir->parent;
		}
	} else {
		if ((elem = getDirectoryFromName(arg)) != NULL) {
			currentdir = elem;
		} else {
			printk("Not a directory");
		}
	}
}

void updateSectorsOnDisk() {
	ata_write(ATA0, sectors, sizeof(sectors), 0, 0);
}

void killChildFile(file* elem, directory* parent) {
	parent->filecount -= 1;
	int i;
	for (i = 0; i < MAX_FILES; i++) {
		if (parent->files[i] != NULL) {
			if (strcmp(parent->files[i]->name, elem->name) == 0) {
				parent->files[i] = NULL;
			}
		}
	}

	updateSectorsOnDisk();
}

void deleteFile(file* elem) {
	clearSector(elem->disksector, (int) (elem->size / 512) + 1);
	killChildFile(elem, elem->parent);
}

void killChild(directory* elem, directory* parent) {
	int i;
	parent->subdircount -= 1;
	for (i = 0; i < MAX_DIRECTORIES; i++) {
		if (parent->subdirectories[i] != NULL) {
			if (strcmp(parent->subdirectories[i]->name, elem->name) == 0) {
				parent->subdirectories[i] = NULL;
			}
		}
	}
	updateSectorsOnDisk();
}

void deleteDirectory(directory* elem) {
	int i;
	for (i = 0; i < MAX_FILES; i++) {
		if (elem->files[i] != NULL) {
			deleteFile(elem->files[i]);
		}

	}
	for (i = 0; i < MAX_DIRECTORIES; i++) {
		if (elem->subdirectories[i] != NULL) {
			deleteDirectory(elem->subdirectories[i]);
		}
	}
	killChild(elem, elem->parent);
	clearSector(elem->disksector, 1);
}

directory* createDirectory(char* name) {
	directory* elem;
	int i, sector;
	memcpy(elem->name, name, strlen(name) + 1);
	sector = getFileFreeSector(1);
	setSector(sector);
	elem->parent = currentdir;
	elem->disksector = sector;
	elem->filecount = 0;
	elem->subdircount = 0;
	for (i = 0; i < MAX_FILES; i++) {
		elem->files[i] = NULL;
	}
	for (i = 0; i < MAX_DIRECTORIES; i++) {
		elem->subdirectories[i] = NULL;
	}
	return elem;
}

void allocateDirectory(directory* elem) {
	int loc = 0, i;
	ata_write(ATA0, elem->name, sizeof(elem->name), elem->disksector, loc);
	loc += sizeof(elem->name);
	if (elem->parent != NULL) {
		ata_write(ATA0, (char*)elem->parent->disksector, sizeof(elem->parent->disksector),
				elem->disksector, loc);
	} else {
		ata_write(ATA0, (char*)-1, sizeof(int), elem->disksector, loc);
	}
	for (i = 0; i < MAX_FILES; i++) {
		if (elem->files[i] != NULL) {
			ata_write(ATA0, (char*)elem->files[i]->disksector, sizeof(int), elem->disksector,
					loc);
			loc += sizeof(int);
		}
	}
	ata_write(ATA0, (char*)-1, sizeof(int), elem->disksector, loc);
	loc += sizeof(int);
	for (i = 0; i < MAX_FILES; i++) {
		if (elem->files[i] != NULL) {
			ata_write(ATA0, (char*)elem->files[i]->disksector, sizeof(int), elem->disksector,
					loc);
			loc += sizeof(int);
		}
	}
	ata_write(ATA0, (char*)-1, sizeof(int), elem->disksector, loc);
	loc += sizeof(int);

}

void mkdir(char* arg) {
	directory* elem = getDirectoryFromName(arg);
	if (elem != NULL) {
		printk("directory* already exists\n");
		return;
	}
	elem = createDirectory(arg);
	allocateDirectory(elem);
}

int rm(char* arg) {
	directory* elem = getDirectoryFromName(arg);
	if (elem == NULL) {
		file* data = getFileFromName(arg);
		if (data != NULL) {
			deleteFile(data);
		} else {
			return -1;
		}
	}
	if (elem->parent == NULL) {
		return -1;
	}
	deleteDirectory(elem);
	return 1;
}

void readFile(file* elem, char* data) {
	int sector = elem->disksector + 1;
	ata_read(ATA0, data, elem->size, sector, 0);
}

int writeFile(file* file, char * buff, int size) {
	if (size > file->size) {
		return -2;
	}
	int sector = file->disksector + 1;
	int loc = 0;

	ata_write(ATA0, buff, size, sector, loc);
	return 0;
}

int edit(char * arg, char* string) {
	openFile(arg);
	file* data = getFileFromName(arg);
	if (data == NULL) {
		return -1;
	}
	printk("Editing %s:\n", data->name);

	writeFile(data, string, strlen(string));
	return 1;
}

int cat(char * arg) {
	file* data = getFileFromName(arg);
	if (data == NULL) {
		return -1;
	}
	char * tmp = (char*) malloc(data->size);

	readFile(data, tmp);
	printk("file* %s:\n", data->name);
	printk("%s\nEOF\n", tmp);
	return 0;
}
