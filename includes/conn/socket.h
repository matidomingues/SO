#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/*Creates Server side socket and starts listening to a port*/
int createServerSocket();

/*Creates Client side socket and connects to 127.0.0.1*/
int createClientSocket();

/*Writes message into socket*/
void writeToSocket(int sockfd, char* msg);

/*Read from socket*/
char* readFromSocket(int sockfd);

#endif
