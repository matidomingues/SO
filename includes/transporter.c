#include "transporter.h"
#include "conn/socket.h"

void createConnection(int id){
	createConnection_IPC(id);
}

void openClient(int pid){
	openClient_IPC(pid);
}

void* listenMessage(int client, size_t messageSize){
	return listenMessage_IPC(client, messageSize);
}

void sendData(int id, void* msg, size_t size){
	sendData_IPC(id, msg, size);
}

int acceptConnection(){
	acceptConnection_IPC();
}

void closeConnection(int pid){
	closeConnection_IPC(pid);
}

void registerClient(int pid, int fd){
	registerClient_IPC(pid, fd);
}
