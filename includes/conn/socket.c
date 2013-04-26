#include "socket.h"

int createServerSocket() {
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

	return listenfd;
}

int createClientSocket() {
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
	return sockfd;
}

void writeToSocket(int sockfd, char* msg) {
	char buffer[1025];
	snprintf(buffer, sizeof(buffer), "%s\n", msg);
	write(sockfd, buffer, strlen(buffer));
}

char* readFromSocket(int sockfd) {
	int n = 0;
	char *buffer = (char*) malloc(sizeof(char) * 1024);
	while ((n = read(sockfd, buffer, sizeof(buffer) - 1)) > 0) {
		buffer[n] = 0;
		if (fputs(buffer, stdout) == EOF) {
			printf("\n Error : Fputs error\n");
		}
	}
	return buffer;
}
