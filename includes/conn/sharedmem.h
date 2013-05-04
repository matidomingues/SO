#ifndef _SHAREDMEM_H_
#define _SHAREDMEM_H_

#include <sys/socket.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <semaphore.h>

typedef struct _client{
	int pid;
	int fd;
	struct _client * next;
}Client;

#define SIZE 1000


int createConnection_IPC(int pid);

int openClient_IPC(int pid);

void sendData_IPC(int pid, void* msg, size_t size);

void* listenMessage_IPC(int pid, size_t messageSize);

int acceptConnection_IPC();

void closeConnection_IPC(int pid);

void registerClient_IPC(int pid, int fd);

#endif
