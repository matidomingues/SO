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

typedef struct _client{
	int pid;
	int write;
	int read;
	struct _client * next;
}Client;

void sendData_IPC(int id, void* msg, size_t size);
void openClient_IPC(int pid);
void createConnection_IPC(int id);
void* listenMessage_IPC(int client, size_t messageSize);

int acceptConnection_IPC();

void closeConnection_IPC(int pid);

void registerClient_IPC(int pid, int fd);

void clientRedirection_IPC(int pid, int client);

void clientRedirectionCreate_IPC(int pid);

#endif