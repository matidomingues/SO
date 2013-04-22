#ifndef _TRANSPORTER_H_
#define _TRANSPORTER_H_

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
	int fd;
	struct _client * next;
}Client;

void createConnection(int id);
void openClient(int pid);
void* listenMessage(int client, size_t messageSize);
void sendData(int id, void* msg, size_t size);

#endif