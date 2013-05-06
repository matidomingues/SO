#ifndef _PIPE_H_
#define _PIPE_H_

#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

typedef struct _client {
	int pid;
	int write;
	int read;
	struct _client * next;
} Client;

/*Creates Server Pipe*/
int createConnection_IPC(int pid);

/*Creates Client Pipe*/
int openClient_IPC(int pid);

/*Writes to Pipe*/
void sendData_IPC(int pid, void* msg, size_t size);

/*Reads from Pipe*/
void* listenMessage_IPC(int pid, size_t messageSize);

/*Establishes pipe connection*/
int acceptConnection_IPC();

/*Closes pipes*/
void closeConnection_IPC(int pid);

/*Registers client*/
void registerClient_IPC(int pid, int fd);

/*Redirection for pipe*/
void clientRedirection_IPC(int pid, int client);
void clientRedirectionCreate_IPC(int pid);

#endif
