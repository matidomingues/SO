#include "socket.h"

Client* newClientNode();

Client* clients;

Client* newClientNode() {
	Client* client = malloc(sizeof(Client));
	return client;
}

int createConnection_IPC(int pid) {
	int listenfd = 0;
	int port = 0;
	struct sockaddr_in serv_addr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Error : Could not create socket \n");
		return 1;
	}
	if(pid == 0){
		port = 5000;
	}else{
		port = pid;
	}

	/*Set socket to NONBLOCK*/
	int flags;
	flags = fcntl(listenfd, F_GETFL, 0);
	fcntl(listenfd, F_SETFL, flags);

	/*Initialize Structs*/
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(port);

	/*Bind socket to address*/
	bind(listenfd, (struct sockaddr*) &serv_addr, sizeof(serv_addr));

	/*Listen to 10 connections at most*/
	listen(listenfd, 10);

	/*Init client list*/
	Client* client = newClientNode();
	client->pid = pid;
	client->fd = listenfd;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", pid);

	return listenfd;
}

int openClient_IPC(int pid) {
	int sockfd = 0;
	int port = 0;
	struct sockaddr_in serv_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	if(pid == 0){
		port = 5000;
	}else{
		port = pid;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(port);

	if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
		printf("\n inet_pton error occurred\n");
		return 1;
	}

	if (connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))
			< 0) {
		printf("\n Error : Connect Failed \n");
		return 1;
	}

	Client* client = newClientNode();
	client->pid = pid;
	printf("%d\n", sockfd);
	client->fd = sockfd;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", pid);
	printf("Socket FD %d\n", sockfd);
	return sockfd;
}

void sendData_IPC(int pid, void* msg, size_t size) {
	int status;
	printf("pid: %d clientFD: %d\n", pid, getClientFD(pid));
	while ((status = write(getClientFD(pid), msg, size)) <= 0){
		sleep(0.1);
		perror("write");
	}
	if (status == -1) {
		printf("Message Not Sent\n");
	}else{
		printf("Message Sent data: %d\n", status);
	}
}

int getClientFD(int pid) {
	printf("%d\n", pid);
	Client* aux = clients;
	while (aux != NULL) {
		if (aux != NULL && aux->pid == pid) {
			return aux->fd;
		}
		aux = aux->next;
	}
	return pid;
}

void* listenMessage_IPC(int pid, size_t messageSize) {
	int status;
	void* info = malloc(messageSize);
	int fd = getClientFD(pid);
	status = read(fd, info, messageSize);
	if (status <= 0) {
		free(info);
		//perror("read");
	} else if (status >= 1) {
		return info;
	}
}

 int acceptConnection_IPC(int pid){
 	int listenfd = getClientFD(pid);
 	int connfd = accept(listenfd, (struct sockaddr*) NULL, NULL); //Blocks (if block) until connection present

		if (connfd == -1) {
			printf("Awaiting connection...\n");
		} else {
			printf("connfd: %d\n", connfd);
			return connfd;
		}
 }

void closeConnection_IPC(int pid){
 	close(getClientFD(pid));
 	// Client* info = clients;
 	// Client* before = NULL;
 	// while(info != NULL){
 	// 	if(info->pid = pid){
 	// 		if(before == NULL){
 	// 			clients = clients->next;
 	// 			free(info);
 	// 		}else{
 	// 			before->next = info->next;
 	// 			free(info);
 	// 		}
 	// 	}
 	// 	before = info;
 	// 	info = info->next;
 	// }
}

void registerClient_IPC(int pid, int fd){
	Client* client = newClientNode();
	client->pid = pid;
	printf("%d\n", fd);
	client->fd = fd;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", pid);

}

void clientRedirection_IPC(int pid, int client){
	return;
}

void clientRedirectionCreate_IPC(int pid){
	return;
}
