#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "message.h"

Task* head;
Task* tail;
Client* clients;

void createBasePipe(){
	char route[16];
	strcpy(route, "/tmp/serv.xxxxx");
	mkfifo(route,S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
}

Task* newTaskNode(){
	Task* task = malloc(sizeof(Task));
	return task;
}

Client* newClientNode(){
	Client* client = malloc(sizeof(Client));
	return client;
}

void pushMessage(Message* msg){
	Task* task = newTaskNode();
	Task* aux;
	task->msg = msg;
	task->next = NULL;
	
	if(head == NULL){
		head = tail = task;
	}else{
		tail->next = task;
		tail = task;
	}
}

Message* popMessage(){
	Message* msg;
	if(head == NULL){
		return NULL;
	}else{
		msg = head->msg;
		head = head->next;
	}
	return msg;
}

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

void listenForConnection(FILE* fd){
	int status = 0;
	Message* info = malloc(sizeof(Message));
	while ((status = fread(info, sizeof(Message), 1, fd)) == 0);
	if(status == -1){
		//perror("read");
	}else{
		printf("Recieved:\n");
		printf("Protocol: %s\n", info->protocol);
		printf("Method: %s\n", info->method);
		printf("Resource: %s\n", info->resource);
		printf("Referer: %i\n", info->referer);
		printf("Body: %s\n", info->body);
		pushMessage(info);
	}
}

char* getFullPath(int id){
	char pid[7];
	char* route = malloc(sizeof(char)*16);
	sprintf(pid,"%d", id);
	strcpy(route, "/tmp/serv.");
	strcat(route,pid);
	return route;
}

void registerClient(int referer){
	char* route = getFullPath(referer);
	FILE* fd = fopen(route,"w+b");
	Client* client = newClientNode();
	client->pid = referer;
	client->fd = fd;
	client->next = clients;
	clients = client;
}

FILE* getClientFile(int pid){
	Client* aux = clients;
	while(aux != NULL){
		if(aux->pid == pid){
			return aux->fd;
		}
	}
	return NULL;
}

void executeActions(){
	Message* msg;
	while((msg = popMessage()) != NULL){
		if(strcmp(msg->resource, "login") == 0){
			if(strcmp(msg->method, "register") == 0){
				registerClient(msg->referer);
			}
		}
	}
}

void writeResponse(int referer, Message* msg){
	FILE* fd = getClientFile(referer);
	int status = 0;
	while((status = fwrite(msg, sizeof(Message), 1, fd))<=0);
	if(status > 0){
		printf("Wrote Successfully\n");
	}
}

int main() {
	FILE* fd;
	head = tail = NULL;
	clients = NULL;
	createBasePipe();
	fd = fopen("/tmp/serv.xxxxx", "r+b");
	printf("Listening on: /tmp/serv.xxxxx\n");
	while(1){
		listenForConnection(fd);
		executeActions();
	}
	return 1;
}
