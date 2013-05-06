#ifndef _SHAREDMEM_H_
#define _SHAREDMEM_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <sys/stat.h>
//#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

typedef struct _client{
	int pid;
	int fd;
	void * shm;
	struct _client * next;
}Client;

//#define SIZE 1000

int createConnection_IPC(int pid);

int openClient_IPC(int pid);

void sendData_IPC(int pid, void* msg, size_t size);

void* listenMessage_IPC(int pid, size_t messageSize);

int acceptConnection_IPC();

void closeConnection_IPC(int pid);

void registerClient_IPC(int pid, int fd);

#endif
