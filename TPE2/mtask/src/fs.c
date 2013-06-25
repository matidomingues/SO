#include "../include/fs.h"

void updateSectorsOnDisk();
void allocateDirectory(directory* elem);
directory* createDirectory(char* arg);
void updateDirectoryOnDisk(directory* parent);
directory* createBaseDirectory();

char sectors[MAX_SECTORS];

directory* currentdir = NULL;

void initSectors() {
	int i;
	for (i = 0; i < MAX_SECTORS; i++) {
		sectors[i] = 0;
	}
	for(i = 0; i<(int)(MAX_SECTORS/512)+1; i++){
		sectors[FILETABLE_SECTOR_START + i] = 1;
	}
	updateSectorsOnDisk();
}

void initFileSystem(){
	initSectors();
	ata_write(ATA0, FILE_SYSTEM_VERSION, sizeof(FILE_SYSTEM_VERSION)+1,0,0);
	directory* root = createDirectory("root");
	currentdir = root;
}

file* loadFile(int sector, directory* parent){
	int loc=0, length = 0;
	file* elem = malloc(sizeof(file));
	ata_read(ATA0, elem->name, NAME_LENGTH, sector, loc);

	loc+=NAME_LENGTH;
	elem->disksector = sector;
	ata_read(ATA0, (char*)(&length), sizeof(int), sector, loc);
	elem->size = length;
	printk("name: %s, sector: %d, file size: %d\n", elem->name, elem->disksector, length);
	return elem;
}

directory* loadDirectories(int sector, directory* parent){
	int finish, i, loc = 0;
	int num;
	directory* elem = createBaseDirectory();
	ata_read(ATA0, elem->name, NAME_LENGTH, sector,loc);
	printk("sector: %d name: %s\n", sector, elem->name);
	loc+=NAME_LENGTH;
	elem->parent = parent;
	elem->disksector = sector;
	for(i=0, finish =0; i<10 && finish == 0; i++){
		ata_read(ATA0, (char*)(&num), sizeof(int), sector, loc);
		if(num == -1){
			finish = 1;
		}else{
			elem->files[elem->filecount++] = loadFile(num, elem);
		}
		loc+=sizeof(int);
	}
	for(i=0, finish =0; i<MAX_DIRECTORIES && finish == 0; i++){
	 	ata_read(ATA0, (char*)(&num), sizeof(int), sector, loc);
	 	if(num == -1){
	 		finish = 1;
	 	}else{
	 		elem->subdirectories[elem->subdircount++] = loadDirectories(num, elem);
	 	}
	 	loc+=sizeof(int);
	}
	return elem;
}

void loadFileSystem(){
	int loc = FILETABLE_SECTOR_START;
	ata_read(ATA0, sectors, sizeof(sectors), loc, 0);
	loc += (int)(sizeof(sectors)/512)+1;
	currentdir = loadDirectories(loc, NULL);
}

void createFS() {
	char* version_check = malloc(sizeof(FILE_SYSTEM_VERSION));
	ata_read(ATA0, version_check, sizeof(FILE_SYSTEM_VERSION)+1, 0, 0); // Read on first segment to check if filesystem exists
	if(strcmp(FILE_SYSTEM_VERSION, version_check) == 0){
		loadFileSystem();
	}else{
		initFileSystem();
	}
}

void initFileName(file* elem){
	int i;
	for(i=0; i< NAME_LENGTH; i++){
		elem->name[i] = 0;
	}
}

void initDirectoryName(directory* elem){
	int i;
	for(i=0; i< NAME_LENGTH; i++){
		elem->name[i] = 0;
	}
}

int getSector(int sector) {
	return sectors[sector];
}

int getFileFreeSector(int ammount) {
	int i, start, w;
	for (i = FILETABLE_SECTOR_START; i < MAX_SECTORS; i++) {
		if ((int)(getSector(i)) == 0) {
			start = i;
			printk("entro con %d\n", i);
			for (w = 0; w < ammount; w++) {
				if ((int)(getSector(i + w)) != 0) {
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
	updateSectorsOnDisk();
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
	file* elem = malloc(sizeof(file));
	int i, sector;
	initFileName(elem);
	memcpy(elem->name, name, strlen(name)+1);
	sector = getFileFreeSector((int) (size / 512) + 1);
	printk("%d\n", sector);
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
	ata_write(ATA0, elem->name, NAME_LENGTH, elem->disksector, loc);
	loc += sizeof(elem->name);
	ata_write(ATA0, (char*)(&(elem->size)), sizeof(int), elem->disksector, loc);
}

void addChild(file* elem, directory* parent) {
	int i = 0;
	parent->filecount += 1;
	for (i = 0; i < MAX_FILES; i++) {
		if (parent->files[i] == NULL) {
			parent->files[i] = elem;
			break;
		}
	}
	updateDirectoryOnDisk(parent);
}

void addDirectoryChild(directory* elem, directory* parent) {
	int i = 0;
	parent->subdircount += 1;
	for (i = 0; i < MAX_DIRECTORIES; i++) {
		if (parent->subdirectories[i] == NULL) {
			parent->subdirectories[i] = elem;
			break;
		}
	}
	updateDirectoryOnDisk(parent);
}



file* openFile(char* name) {
	file* elem = getFileFromName(name);
	if (elem == NULL) {
		elem = createFile(name, DEFAULT_FILESIZE);
		allocateFile(elem);
		addChild(elem, currentdir);
	}
	return elem;
}

int ls(int argc, char **argv) {
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
	return 0;
}

int cd(int argc, char **argv) {
	directory* elem;
	if (strcmp(argv[1], "..") == 0) {
		if (currentdir->parent != NULL) {
			currentdir = currentdir->parent;
		}else{
			printk("Already on root\n");
			return -1;
		}
	} else {
		if ((elem = getDirectoryFromName(argv[1])) != NULL) {
			currentdir = elem;
		} else {
			printk("Not a directory");
			return -1;
		}
	}
	return 0;
}

void updateSectorsOnDisk() {
	ata_write(ATA0, sectors, sizeof(sectors), FILETABLE_SECTOR_START, 0);
}

void updateDirectoryOnDisk(directory* parent){
	allocateDirectory(parent);
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
	updateDirectoryOnDisk(parent);
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
	updateDirectoryOnDisk(parent);
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

directory* createBaseDirectory(){
	int i;
	directory* elem = malloc(sizeof(directory));
	initDirectoryName(elem);
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

directory* createDirectory(char* arg) {
	int sector;
	directory* elem = createBaseDirectory();
	memcpy(elem->name, arg, strlen(arg)+1);
	printk("directory name: %s\n", elem->name);
	sector = getFileFreeSector(1);
	setSector(sector);
	elem->parent = currentdir;
	elem->disksector = sector;
	return elem;
}

void allocateDirectory(directory* elem) {
	int loc = 0, i, e = -1;
	ata_write(ATA0, elem->name, NAME_LENGTH, elem->disksector, loc);
	loc += NAME_LENGTH;
	for (i = 0; i < MAX_FILES; i++) {
		if (elem->files[i] != NULL) {
			ata_write(ATA0, (char*)(&(elem->files[i]->disksector)), sizeof(int), elem->disksector,
					loc);
			loc += sizeof(int);
		}
	}
	ata_write(ATA0, (char*)(&e), sizeof(int), elem->disksector, loc);
	loc += sizeof(int);
	for (i = 0; i < MAX_DIRECTORIES; i++) {
		if (elem->subdirectories[i] != NULL) {
			ata_write(ATA0, (char*)(&(elem->subdirectories[i]->disksector)), sizeof(int), elem->disksector,
					loc);
			loc += sizeof(int);
		}
	}
	ata_write(ATA0, (char*)(&e), sizeof(int), elem->disksector, loc);
	loc += sizeof(int);
	printk("wrote elem on segment: %d and loc: %d\n",elem->disksector, loc);

}

int mkdir(int argc, char **argv) {
	directory* elem = getDirectoryFromName(argv[1]);
	if (elem != NULL) {
		printk("directory already exists\n");
		return -1;
	}
	elem = createDirectory(argv[1]);
	allocateDirectory(elem);
	addDirectoryChild(elem, currentdir);
	return 0;
}

int rm(int argc, char **argv) {
	directory* elem = getDirectoryFromName(argv[1]);
	if (elem == NULL) {
		file* data = getFileFromName(argv[1]);
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
	return 0;
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

int edit(int argc, char **argv) {
	if(argc != 3){
		return -1;
	}
	
	file* data = openFile(argv[1]);

	printk("Editing %s:\n", data->name);

	writeFile(data, argv[2], strlen(argv[2]));
	return 0;
}

int cat(int argc, char **argv) {
	file* data = getFileFromName(argv[1]);
	if (data == NULL) {
		return -1;
	}
	char * tmp = (char*) malloc(data->size);

	readFile(data, tmp);
	printk("file* %s:\n", data->name);
	printk("%s\nEOF\n", tmp);
	return 0;
}

int print_sectors(int argc, char **argv){
	int i;
	for(i=0; i<500; i++){
		printk("%d", getSector(i));
	}
	printk("\n");
	return 0;
}

int format(int argc, char **argv){
	char clean[512];
	int i;
	for(i=0; i<512; i++){
		clean[i] = '\0';
	}
	ata_write(ATA0, clean, 512, 0, 0);
	return 0;

}
