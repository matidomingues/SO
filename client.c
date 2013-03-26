#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "message.h"
#include <time.h>

Message* prepareMessage(){
	Message* info = (Message*) malloc(sizeof(Message)); 
	return info;
}

Message* fillMessageData(Message* msg, char* method, char* resource, int referer, char* body){
	strcpy(msg->protocol, "TPSO 0.12");
	strcpy(msg->method, method);
	strcpy(msg->resource, resource);
	time(&(msg->time));
	msg->referer = referer;
	strcpy(msg->body, body);
	return msg;

}

char* createPipe(){
	char pid[7];
	char* route = malloc(sizeof(char)*16);
	sprintf(pid,"%d", getpid());
	strcpy(route, "/tmp/serv.");
	strcat(route,pid);
	printf("%d",mkfifo(route, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH));
	return route;
}

void sendData(FILE* fd, char* route){
	int status;
	Message* msg = prepareMessage();
	msg = fillMessageData(msg, "register", "login", getpid(), "");

	printf("paso\n");

	while ((status = fwrite(msg, sizeof(Message), 1, fd)) == 0);
	if(status == -1){
		printf("ERROR ON PIPE");
	}else{
		printf("wrote");
	}
}

void listenForConnection(FILE* fd2){
	int status = 0;
	char test[6];
	while ((status = fread(test,6,1,fd2)) == 0);
	if(status == -1){
		printf("ERROR ON PIPE");
	}else{
		printf("%s\n", test);
	}
}

int main() {
	FILE* fd, fd2;
	char* route; 
	route = createPipe(); 
	printf("%s\n", route);
	fd2 = fopen((char*)route, "r");
	
	fd = fopen("/tmp/serv.xxxxx", "w+b");
	sendData(fd, route);
	//listenForConnection(fd2);
	return 1;
}
