#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <sys/socket.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct _client{
	int pid;
	int fd;
	struct _client * next;
}Client;

/*Creates Server side socket and starts listening to a port*/
int createConnection_IPC(int pid);

/*Creates Client side socket and connects to 127.0.0.1*/
int openClient_IPC(int pid);

/*Writes message into socket*/
void sendData_IPC(int pid, void* msg, size_t size);

/*Read from socket*/
void* listenMessage_IPC(int pid, size_t messageSize);

#endif
