#ifndef _MQUEUE_H_
#define _MQUEUE_H_

#include<sys/msg.h>

typedef struct _client {
	int pid;
	int fd;
	struct _client * next;
} Client;

/*Creates Message Queue*/
int createConnection_IPC(int pid);

/*Connects to Message Queue*/
int openClient_IPC(int pid);

/*Writes to Message Queue*/
void sendData_IPC(int pid, void* msg, size_t size);

/*Reads from Message Queue*/
void* listenMessage_IPC(int pid, size_t messageSize);

#endif
