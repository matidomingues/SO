#include "transporter.h"
#include "conn/mesqueue.h"

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
	return acceptConnection_IPC();
}

void closeConnection(int pid){
	closeConnection_IPC(pid);
}

void registerClient(int pid, int fd){
	registerClient_IPC(pid, fd);
}

void clientRedirection(int pid, int client){
	clientRedirection_IPC(pid, client);
}

void clientRedirectionCreate(int pid){
	clientRedirectionCreate_IPC(pid);
}