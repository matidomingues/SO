#include "mesqueue.h"

Client* newClientNode();
char* getFullPath(int id);
int getClientFD(int pid);
Client* getClient(int pid);

Client* clients;

Client* newClientNode() {
	Client* client = malloc(sizeof(Client));
	return client;
}


int createConnection_IPC(int pid) {
	Client* client;
	char* path = getFullPath(pid);
	open(path, O_RDWR | O_CREAT);
	key_t key = ftok(path, '#');

	int mq = msgget(key, 0666 | IPC_CREAT);

	/*Init client list*/
	if((client = getClient(pid))== NULL){
		client = newClientNode();
		client->pid = pid;
		client->next = clients;
		clients = client;
	}
	if(pid == 0){
		client->readpid = pid+1;
	}else{
		client->readpid = pid;
	}
	client->fd = mq;

	printf("Registering client %d\n", pid);
	printf("Message Queue ID %d\n", mq);
	return mq;
}

/*Connects to Message Queue*/
int openClient_IPC(int pid) {
	Client* client;
	char* path = getFullPath(pid);
	open(path, O_RDWR | O_CREAT);
	key_t key = ftok(path, '#');

	int mq = msgget(key, 0666);
	if((client = getClient(pid))== NULL){
		client = newClientNode();
		client->pid = pid;
		client->next = clients;
		clients = client;
	}
	if(pid == 0){
		client->readpid = 1;
	}else{
		client->readpid = pid;
	}
	client->fd = mq;
	printf("Registering client %d\n", pid);
	printf("Message Queue ID %d\n", mq);
	return mq;
}

/*Writes to Message Queue*/
void sendData_IPC(int pid, void* msg, size_t size) {
	int attempts = 0;
	Msg* data = malloc(sizeof(Msg));
	strcpy(data->mtext, marshall(msg, size));
	Client* client = getClient(pid);
	if(pid == 0){
		data->mtype = 1;
	}else{
		data->mtype = pid;
	}
	printf("Write on mq: %d with pid: %d\n", client->fd, pid);	
	while (msgsnd(client->fd, data, strlen(data->mtext)+1, 0) == -1 && attempts < 100){
		attempts++;
	}
		printf("Message Sent\n");
}

/*Reads from Message Queue*/
void* listenMessage_IPC(int pid, size_t messageSize) {
	int status;
	Msg* data = malloc(sizeof(Msg));
	int mq = getClientFD(pid);
	/*Read from */
	if(mq == pid){
		pid = 1;
	}else{
		pid = getClient(pid)->readpid;
	}
	printf("Reading on mq: %d with pid: %d\n", mq, pid);
	status = msgrcv(mq, data, MSGSIZE, pid, 0);
	if(status <= 0){
		perror("status");
	}

	if (status >= 1) {
		printf("paso %s\n", data->mtext);
		return unmarshall(data->mtext);
	}
}

int getClientFD(int pid) {
	Client* aux = getClient(pid);
	if(aux != NULL){
		return aux->fd;
	}else{
		return pid;
	}
}

int getPidKey(int pid){
	if(pid == 0){
		return 5000;
	}else{
		return pid;
	}
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

Client* getClient(int pid){
	Client* aux = clients;
	while(aux != NULL && aux->pid != pid){
		aux = aux->next;
	}
	return aux;
}

int acceptConnection_IPC(){
	return getClientFD(0);
}

void closeConnection_IPC(int pid){
	return;
}

void registerClient_IPC(int pid, int fd){
	openClient_IPC(pid);
	Client* aux = getClient(pid);
	aux->readpid = 1;
}

void clientRedirection_IPC(int pid, int client){
	int data = getClientFD(client);
	Client* aux = getClient(pid);
	aux->fd = data;
	aux->readpid = client;
}

void clientRedirectionCreate_IPC(int pid){
	createConnection_IPC(pid);
}
