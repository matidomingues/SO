#include "pipe.h"

char* getFullPath(int id);
int getClientFile(int pid, int read);
void createPipe(char* route);
Client* newClientNode();
void openRead(int pid);
Client* getClient(int pid);

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
	int fd = getClientFile(client, 1);
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
	Client* client;
	char* route = getFullPath(pid);
	printf("%s\n", route);
	int fd = open(route, O_RDONLY | O_NONBLOCK);
	if((client = getClient(pid))==NULL){
		client = newClientNode();
		client->pid = pid;
		client->next = clients;
		clients = client;
	}
	client->read = fd;
	printf("Registering client %d\n", pid);
}

void openClient_IPC(int pid){
	Client* client;
	char* route = getFullPath(pid);
	printf("%s\n", route);
	int fd = open(route, O_WRONLY | O_NONBLOCK);
	if((client = getClient(pid)) == NULL){
		client = newClientNode();
		client->pid = pid;
		client->next = clients;
		clients = client;
	}
	client->write = fd;
	
	printf("Registering client %d\n", pid);
}

void sendData_IPC(int id, void* msg, size_t size){
	int status;
	while ((status = write(getClientFile(id,0), msg, size)) <= 0){
		sleep(1);
		printf("fd to write: %d\n",getClientFile(id,0));
		perror("write");
	}
	if(status == -1){
		printf("Message Not Sent\n");
	}else{
		printf("Message Sent: %d\n", status);
	}
	free(msg);
}

Client* getClient(int pid){
	Client* aux = clients;
	while(aux != NULL && aux->pid != pid){
		aux = aux->next;
	}
	return aux;
}

int getClientFile(int pid, int read){
	Client* aux = getClient(pid);
	if(aux == NULL){
		return pid;
	}else{
		if(read == 1){
			return aux->read;
		}else{
			return aux->write;
		}
	}
	
}

int acceptConnection_IPC(){
	return getClientFile(0, 1);
}

void registerClient_IPC(int pid, int fd){
	int pidn = pid;
	Client* client;
	pid = (pid*10)+1;
	char* route = getFullPath(pid);
	fd = open(route, O_RDONLY | O_NONBLOCK);
	if((client = getClient(pidn)) == NULL){
		client = newClientNode();
		client->pid = pidn;
		client->next = clients;
		clients = client;
	}
	printf("Client: %d\n", client->pid);
	client->read = fd;
	route = getFullPath(pidn);
	fd = open(route, O_WRONLY | O_NONBLOCK);
	client->write = fd;
}

void clientRedirectionCreate_IPC(int pid){
	char* route = getFullPath(pid);
	createPipe(route);
	route = getFullPath((pid*10)+1);
	createPipe(route);
}

void clientRedirection_IPC(int pid, int client){
	Client* data = getClient(client);
	char* route = getFullPath(client);
	if(data == NULL){
		data = newClientNode();
		data->pid = pid;
		data->next = clients;
		clients = data;
	}
	int fd = open(route, O_RDONLY | O_NONBLOCK);
	data->read = fd;
	printf("read FD: %d\n", fd);
	route = getFullPath((client*10)+1);
	fd = open(route, O_RDWR | O_NONBLOCK);
	data->write = fd;
	printf("write FD: %d\n", fd);
}

void closeConnection_IPC(int pid){
	Client* data = getClient(pid);
	close(data->read);
	close(data->write);
}