#include "socket.h"

Client* newClientNode();

Client* clients;

Client* newClientNode() {
	Client* client = malloc(sizeof(Client));
	return client;
}

int createConnection_IPC(int pid) {
	int listenfd = 0;
	struct sockaddr_in serv_addr;

	if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	/*Set socket to NONBLOCK*/
	int flags;
	flags = fcntl(listenfd, F_GETFL, 0);
	fcntl(listenfd, F_SETFL, flags | O_NONBLOCK);

	/*Initialize Structs*/
	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	serv_addr.sin_port = htons(5000);

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

	struct sockaddr_in serv_addr;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		printf("\n Error : Could not create socket \n");
		return 1;
	}

	memset(&serv_addr, '0', sizeof(serv_addr));

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons(5000);

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

	return sockfd;
}

void sendData_IPC(int id, void* msg, size_t size) {
	int status;
	while ((status = write(getClientFile(id), msg, size)) <= 0)
		;
	if (status == -1) {
		printf("Message Not Sent\n");
	}
	free(msg);
}

int getClientFD(int pid) {
	Client* aux = clients;
	while (aux != NULL) {
		if (aux->pid == pid) {
			return aux->fd;
		}
		aux = aux->next;
	}
	return 0;
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
