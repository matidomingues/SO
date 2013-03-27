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

void listenForConnection(int fd){
	int status = 0;
	Message* info = malloc(sizeof(Message));
	status = read(fd, info, sizeof(Message));
	if(status <= 0){
		free(info);
		//perror("read");
	}else if(status >=1){
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

void sendData(int fd, char* route){
	int status;
	Message* msg = prepareMessage();
	msg = fillMessageData(msg, "register", "login", getpid(), "");
	while ((status = write(fd, msg, sizeof(Message))) == 0){
		printf("paso2\n");
	}
	if(status == -1){
		printf("ERROR ON PIPE write");
	}else{
		printf("wrote");
	}
}

void registerClient(int referer){
	char* route = getFullPath(referer);
	int fd = open(route, O_WRONLY | O_NONBLOCK);
	Client* client = newClientNode();
	client->pid = referer;
	client->fd = fd;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", referer);
	sendData(fd, route);
}

int getClientFile(int pid){
	Client* aux = clients;
	while(aux != NULL){
		if(aux->pid == pid){
			return aux->fd;
		}
	}
	return 0;
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
	int fd = getClientFile(referer);
	int status = 0;
	while((status = write(fd, msg, sizeof(Message)))<=0);
	if(status > 0){
		printf("Wrote Successfully\n");
	}
}

int main() {
	int fd;
	head = tail = NULL;
	clients = NULL;
	createBasePipe();
	fd = open("/tmp/serv.xxxxx", O_RDONLY | O_NONBLOCK);
	printf("Listening on: /tmp/serv.xxxxx\n");
	while(1){
		listenForConnection(fd);
		executeActions();
	}
	return 1;
}
