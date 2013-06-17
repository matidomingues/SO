#ifndef _SHAREDMEM_H_
#define _SHAREDMEM_H_

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>

typedef struct _client{
	int pid;
	int shmid;
	void * shm;
	struct _client * next;
}Client;

int createConnection_IPC(int pid);

int openClient_IPC(int pid);

void sendData_IPC(int pid, void* msg, size_t size);

void* listenMessage_IPC(int pid, size_t messageSize);

int acceptConnection_IPC();

void closeConnection_IPC(int pid);

void registerClient_IPC(int pid, int fd);

#endif
