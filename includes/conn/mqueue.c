#include "mqueue.h"

Client* newClientNode();

Client* clients;

Client* newClientNode() {
	Client* client = malloc(sizeof(Client));
	return client;
}

/*Message Queue identifier*/
#define KEY 500

/*Structure required to send/rcv Messages*/
struct msg {
	long int type;
	char a[1024];
	void* data;
} Msg;

int createConnection_IPC(int pid) {
	/*Create MQ*/
	int mq = msgget(KEY, 0666 | IPC_CREAT);

	/*Init client list*/
	Client* client = newClientNode();
	client->pid = pid;
	client->fd = mq;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", pid);

	return mq;
}

/*Connects to Message Queue*/
int openClient_IPC(int pid) {
	/*Connect to MQ*/
	int mq = msgget(KEY, 0);

	Client* client = newClientNode();
	client->pid = pid;
	client->fd = mq;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", pid);
	printf("Message Queue ID %d\n", mq);
	return mq;
}

/*Writes to Message Queue*/
void sendData_IPC(int pid, void* msg, size_t size) {

	/*Set message type to 1*/
	Msg.type = 1;

	/*Set the message we want to send*/
	Msg.data = msg;

	int status;
	while ((status = msgsnd(getClientFD(pid), &Msg, sizeof(Msg), 0)) <= 0)
		;
	if (status == -1) {
		printf("Message Not Sent\n");
	} else {
		printf("Message Sent data: %d\n", status);
	}
}

/*Reads from Message Queue*/
void* listenMessage_IPC(int pid, size_t messageSize) {
	int status;
	void* info = malloc(messageSize);
	int mq = getClientFD(pid);
	/*Read from */
	status = msgrcv(mq, &Msg, sizeof(Msg), 1, 0);

	info = Msg.data;	//return Msg.data directo? Preg a Mati.
	if (status <= 0) {
		free(info);
		//perror("read");
	} else if (status >= 1) {
		return info;
	}
}

int getClientFD(int pid) {
	Client* aux = clients;
	while (aux != NULL) {
		if (aux->pid == pid) {
			return aux->fd;
		}
		aux = aux->next;
	}
	return pid;
}

