#include "sharedmem.h"

static sem_t *sd;
char* mem;

void
initmutex(void)
{
	if ( !(sd = sem_open("/mutex", O_RDWR|O_CREAT, 0666, 1)) )
		fatal("sem_open");
}

void
enter(void)
{
	sem_wait(sd);
}

void
leave(void)
{
	sem_post(sd);
}

char *
getmem(void)
{
	int fd;
	char *mem;
	
	if ( (fd = shm_open("/message", O_RDWR|O_CREAT, 0666)) == -1 )
		fatal("sh_open");
	ftruncate(fd, SIZE);
	if ( !(mem = mmap(NULL, SIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0)) )
		fatal("mmap");
	close(fd);
	return mem;
}

int createConnection_IPC(int pid){
	mem = getmem();
	memset(msg, 0, SIZE);
	initmutex();
}

int openClient_IPC(int pid){
	mem = getmem();
	initmutex();
}

void sendData_IPC(int pid, void* msg, size_t size){

	enter();
	memcpy(mem, msg, size);
	leave();

}

void* listenMessage_IPC(int pid, size_t messageSize){
	void* data = malloc(messageSize);
	enter();
	memcpy(data, mem, messageSize);
	leave();
	if(data == NULL){
		listenMessage_IPC(pid,messageSize);
	}else{
		return data;
	}
}

int acceptConnection_IPC(){

}

void closeConnection_IPC(int pid){

}

void registerClient_IPC(int pid, int fd){

}