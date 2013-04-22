#include "pipe.h"

char* getFullPath(int id);
int getClientFile(int pid);
void createPipe(char* route);
Client* newClientNode();
void openRead(int pid);

Client* clients;

Client* newClientNode(){
	Client* client = malloc(sizeof(Client));
	return client;
}

void createPipe(char* route){
	mkfifo(route,S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
}

void* listenMessage_IPC(int client, size_t messageSize){
	int status;
	void* info = malloc(messageSize);
	int fd = getClientFile(client);
	fd_set active_fd_set;
	FD_ZERO (&active_fd_set);
    FD_SET (fd, &active_fd_set);
	if(select(FD_SETSIZE, &active_fd_set, NULL, NULL, NULL) <0){
		perror("select");
	}
	status = read(fd, info, messageSize);
	if(status <= 0){
		free(info);
		//perror("read");
	}else if(status >=1){
		return info;
	}
}

void createConnection_IPC(int id){
	char* path;
	path = getFullPath(id);
	createPipe(path);
	openRead(id);
}

char* getFullPath(int id){
	char pid[7];
	char* route = malloc(sizeof(char)*16);
	if(id == 0){
		strcpy(pid,"xxxxx");
	}else{
		sprintf(pid,"%d", id);
	}
	strcpy(route, "/tmp/serv.");
	strcat(route,pid);
	return route;
}

void openRead(int pid){
	char* route = getFullPath(pid);
	printf("%s\n", route);
	int fd = open(route, O_RDONLY | O_NONBLOCK);
	Client* client = newClientNode();
	client->pid = pid;
	client->fd = fd;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", pid);
}

void openClient_IPC(int pid){
	char* route = getFullPath(pid);
	printf("%s\n", route);
	int fd = open(route, O_WRONLY | O_NONBLOCK);
	Client* client = newClientNode();
	client->pid = pid;
	printf("%d\n", fd);
	client->fd = fd;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", pid);
}

void sendData_IPC(int id, void* msg, size_t size){
	int status;
	while ((status = write(getClientFile(id), msg, size)) <= 0);
	if(status == -1){
		printf("Message Not Sent\n");
	}
	free(msg);
}

int getClientFile(int pid){
	Client* aux = clients;
	while(aux != NULL){
		if(aux->pid == pid){
			return aux->fd;
		}
		aux = aux->next;
	}
	return 0;
}