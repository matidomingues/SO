#include "sharedmem.h"

/*Size in bytes of SM space*/
#define SHMSZ 1024

Client* clients;

Client* newClientNode() {
	Client* client = malloc(sizeof(Client));
	return client;
}

static sem_t *sd;

void initmutex(void) {
	if (!(sd = sem_open("/mutex", O_RDWR | O_CREAT, 0666, 1)))
		fatal("sem_open");
}

void enter(void) {
	sem_wait(sd);
}

void leave(void) {
	sem_post(sd);
}

int createConnection_IPC(int pid) {
	initmutex();
	int shmid;
	key_t key = 5678;
	void *shm;

	/*Create the shared memory segment*/
	if ((shmid = shmget(key, SHMSZ, IPC_CREAT | 0666)) < 0) {
		perror("shmget");
	}

	/*Attach segment to data space*/
	if ((shm = shmat(shmid, NULL, 0)) < 0) {
		perror("shmat");
	}

	return openClient_IPC(pid);
}

int openClient_IPC(int pid) {
	int shmid;
	key_t key = 5678;
	void *shm;

	/*Locate the shared memory segment*/
	if ((shmid = shmget(key, SHMSZ, 0666)) < 0) {
		perror("shmget");
	}

	/*Attach segment to data space*/
	if ((shm = shmat(shmid, NULL, 0)) < 0) {
		perror("shmat");
	}

	Client* client = newClientNode();
	client->pid = pid;
	printf("%d\n", shmid);
	client->shmid = shmid;
	client->shm = shm;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", pid);

	return shmid;
}

void sendData_IPC(int pid, void* msg, size_t size) {
	int status;
	void * shm = getClientSHM(pid);
	enter();
	memcpy(shm, msg, size);
	leave();
}

void* listenMessage_IPC(int pid, size_t messageSize) {
	void* data = malloc(messageSize);
	enter();

	void * shm = getClientSHM(pid);
	enter();
	memcpy(data, shm, messageSize);
	leave();

	if (data == NULL) { //TODO:Retry?, ojo con esto loop recursivo raro
		listenMessage_IPC(pid, messageSize);
	} else {
		return data;
	}
}

void* getClientSHM(int pid) {
	Client* aux = clients;
	while (aux != NULL) {
		if (aux->pid == pid) {
			return aux->shm;
		}
		aux = aux->next;
	}
	return NULL;
}

void* getClientSHMID(int pid) {
	Client* aux = clients;
	while (aux != NULL) {
		if (aux->pid == pid) {
			return aux->shmid;
		}
		aux = aux->next;
	}
	return NULL;
}

int acceptConnection_IPC() {
	return getClientSHMID(0);
}

void closeConnection_IPC(int pid) {
	Client* data = getClient(pid);
	/*Detach*/
	shmdt(data->shmid);
	/*Remove*/
	shmctl(data->shmid, IPC_RMID, NULL);
}

/*TODO:Chequear esto*/
void registerClient_IPC(int pid, int shmid) {
	int pidn = pid;
	Client* client;
	pid = (pid * 10) + 1;

	key_t key = 5678;
	shmid = shmget(key, SHMSZ, 0666);
	void * shm = shmat(shmid, NULL, 0);

	if ((client = getClient(pidn)) == NULL) {
		client = newClientNode();
		client->pid = pidn;
		client->next = clients;
		clients = client;
	}

	printf("Client: %d\n", client->pid);
	client->shmid = shmid;
	client->shm = shm;
}
