#ifndef _MESQUEUE_H_
#define _MESQUEUE_H_

#include <sys/msg.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include "../marshalling.h"


#define MSGSIZE 150
#define KEY 12345

typedef struct _client {
	int pid;
	int fd;
	int readpid;
	struct _client * next;
} Client;

typedef struct _msg {
	long int mtype;
	char mtext[MSGSIZE];
} Msg;



/*Creates Message Queue*/
int createConnection_IPC(int pid);

/*Connects to Message Queue*/
int openClient_IPC(int pid);

/*Writes to Message Queue*/
void sendData_IPC(int pid, void* msg, size_t size);

/*Reads from Message Queue*/
void* listenMessage_IPC(int pid, size_t messageSize);

/*Establishes connection*/
int acceptConnection_IPC();

/*Closes MQ*/
void closeConnection_IPC(int pid);

/*Registers client*/
void registerClient_IPC(int pid, int fd);

void clientRedirection_IPC(int pid, int client);

void clientRedirectionCreate_IPC(int pid);


#endif
