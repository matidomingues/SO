#ifndef _TRANSPORTER_H_
#define _TRANSPORTER_H_

#include <sys/types.h>

void createConnection(int id);
void openClient(int pid);
void* listenMessage(int client, size_t messageSize);
void sendData(int id, void* msg, size_t size);
int acceptConnection();
void closeConnection(int pid);
void registerClient(int pid, int fd);

#endif