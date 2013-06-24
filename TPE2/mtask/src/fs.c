#include "fs.h"

int sectors[MAX_SECTORS]
directory currentdir = NULL;

directory createFS() {
	directory root;
	root.name = "root";
	root.filecount = 0;
	root.subdircount = 0;
	root.parent = NULL;
	int i;
	for (i = 0; i < 255; ++i) {
		root.files[i] = NULL;
		root.subdirectories[i] = NULL;
	}
	currentdir = null;
	return root;
}

void initSectors(){
	int i;
	for(i=0; i<MAX_SECTORS; i++){
		sectors[i] = 0;
	}
}

int getFreeSector(){
	int i;
	for(i=0; i<MAX_SECTORS; i++){
		if(!sectors[i]){
			return i;
		}
	}
	return -1;
}

void setSector(int sector){
	sectors[sector] = 1;
}

void clearSector(int sector, int ammount){
	int i;
	for(i=0; i<ammount; i++){
		sectors[sector+i] = 0;
	}
}

file getFileFromName(char* name){
	int i;
	file elem;
	for(i=0; i< MAX_FILES; i++){
		elem = currentdir.files[i]
		if(elem != NULL){
			if(strcmp(elem.name, name) == 0){
				return elem;
			}
		}
	}
	return NULL;
}

directory getDirectoryFromName(char* name){
	int i;
	directory elem;
	for(i=0; i< MAX_DIRECTORIES; i++){
		elem = currentdir.subdirectories[i]
		if(elem != NULL){
			if(strcmp(elem.name, name) == 0){
				return elem;
			}
		}
	}
	return NULL;
}

file* openFile(char* name){
	file elem = getFileFromName(name);
	if(elem == NULL){
		return -1;
	}else{
		return &elem;
	}

}

void ls(){
	int i;
	printf("Directory: %s\n", currentdir.name);
	for(i=0; i<MAX_DIRECTORIES; i++){
		if(currentdir.subdirectories[i] != NULL){
			printf("%s\n", currentdir.subdirectories[i].name);
		}
	}
	for(i=0; i<MAX_FILES; i++){
		if(currentdir.files[i] != NULL){
			printf("%s\n", currentdir.files[i].name);
		}
	}
}

void cd(char* arg){
	directory elem;
	if(strcmp(arg, "..") == 0){
		if(currentdir.parent != NULL){
			currentdir = currentdir.parent;
		}
	}else{
		if((elem = getDirectoryFromName(name)) != NULL){
			currentdir = elem;
		}else{
			printf("Not a directory");
		}
	}
}

void killChildFile(file elem, directory parent){
	parent.filecount -= 1;
	for(i=0; i<MAX_FILES; i++){
		if(parent.files[i] != NULL){
			if(strcmp(parent.files[i].name, elem.name) == 0){
				parent.files[i] = NULL;
			}
		}
	}
}

void deleteFile(file elem){
	clearSector(elem.disksector, (int)(elem.size/512)+1);
	killChildFile(elem, elem.parent);
}

void killChild(directory elem, directory parent){
	int i;
	parent.subdircount -= 1;
	for(i=0; i<MAX_DIRECTORIES; i++){
		if(parent.subdirectories[i] != NULL){
			if(strcmp(parent.subdirectories[i].name, elem.name) == 0){
				parent.subdirectories[i] = NULL;
			}
		}
	}
}


void deleteDirectory(directory elem){
	int i;
	for(i=0; i<MAX_FILES; i++){
		if(elem.files[i]!= NULL){
			deleteFile(elem.files[i]);
		}
		
	}
	for(i=0; i<MAX_DIRECTORIES; i++){
		if(elem.subdirectories[i] != NULL){
			deleteDirectory(directory.subdirectories[i]);
		}
	}
	killChild(elem, elem.parent);
	clearSector(elem.disksector, 1);


}

int rm(char* arg){
	directory elem = getDirectoryFromName(arg);
	if(elem.parent == NULL){
		return -1;
	}
	deleteDirectory(elem);
}
