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

void setSector(sector){
	sectors[sector] = 1;
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
