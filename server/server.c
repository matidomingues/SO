#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../includes/message.h"
#include "../includes/user.h"
#include "../includes/linkedlist.h"
#include "../includes/csv.h"


Task* head;
Task* tail;
Client* clients;
linked_list* users;
fd_set active_fd_set;

void createBasePipe(){
	char route[16];
	strcpy(route, "/tmp/serv.xxxxx");
	mkfifo(route,S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH);
}

Task* newTaskNode(){
	Task* task = malloc(sizeof(Task));
	return task;
}

Client* newClientNode(){
	Client* client = malloc(sizeof(Client));
	return client;
}

void pushMessage(Message* msg){
	Task* task = newTaskNode();
	Task* aux;
	task->msg = msg;
	task->next = NULL;
	
	if(head == NULL){
		head = tail = task;
	}else{
		tail->next = task;
		tail = task;
	}
}

Message* popMessage(){
	Message* msg;
	if(head == NULL){
		return NULL;
	}else{
		msg = head->msg;
		head = head->next;
	}
	return msg;
}

Message* prepareMessage(){
	Message* info = (Message*) malloc(sizeof(Message)); 
	return info;
}

void listenForConnection(int fd){
	int status = 0;
	Message* info = malloc(sizeof(Message));
	if(select(FD_SETSIZE, &active_fd_set, NULL, NULL, NULL) <0){
		perror("select");
	}
	status = read(fd, info, sizeof(Message));
	if(status <= 0){
		free(info);
		//perror("read");
	}else if(status >=1){
		printf("Recieved:\n");
		printf("Protocol: %s\n", info->protocol);
		printf("Method: %s\n", info->method);
		printf("Resource: %s\n", info->resource);
		printf("Referer: %i\n", info->referer);
		printf("Body: %s\n", info->body);
		pushMessage(info);
	}
}

char* getFullPath(int id){
	char pid[7];
	char* route = malloc(sizeof(char)*16);
	sprintf(pid,"%d", id);
	strcpy(route, "/tmp/serv.");
	strcat(route,pid);
	return route;
}

Message* fillMessageData(Message* msg, char* method, char* resource, char* body){
	strcpy(msg->protocol, "TPSO 0.12");
	strcpy(msg->method, method);
	strcpy(msg->resource, resource);
	time(&(msg->time));
	msg->referer = getpid();
	strcpy(msg->body, body);
	return msg;

}

void sendData(int fd, char* resource, char* method, char* body){
	int status;
	Message* msg = prepareMessage();
	msg = fillMessageData(msg, method, resource, body);
	while ((status = write(fd, msg, sizeof(Message))) == 0){
		printf("paso2\n");
	}
	if(status == -1){
		printf("ERROR ON PIPE write\n");
	}else{
		printf("wrote\n");
	}
	free(msg);
}

void registerClient(int referer){
	char* route = getFullPath(referer);
	printf("%s\n", route);
	int fd = open(route, O_WRONLY | O_NONBLOCK);
	Client* client = newClientNode();
	client->pid = referer;
	client->fd = fd;
	client->next = clients;
	clients = client;
	printf("Registering client %d\n", referer);
	sendData(fd, "client", "success", "");
}

int getClientFile(int pid){
	Client* aux = clients;
	while(aux != NULL){
		if(aux->pid == pid){
			printf("%d\n", aux->fd);
			return aux->fd;
		}
	}
	return 0;
}



void registerUser(Message* msg){
	char* data = msg->body;
	char* tokens;
	time_t timer;
	user* elem = malloc(sizeof(user));
	tokens = strtok(data,",");
	time(&timer);
	elem->registration_date = timer;
	elem->name = malloc(strlen(tokens));
	strcpy(elem->name, tokens);
	tokens = strtok(NULL,",");
	elem->username = malloc(strlen(tokens));
	strcpy(elem->username, tokens);
	tokens = strtok(NULL,",");
	elem->password = malloc(strlen(tokens));
	strcpy(elem->password, tokens);
	elem->fee = 0;
	elem->mail_list = NULL;

	addNode(users, elem, true);
	printf("Registered User: %s\n", elem->username);
	sendData(getClientFile(msg->referer), "register", "success", "");
}

void loginUser(Message* msg){
	int finished = 0;
	char* data = msg->body;
	node* aux = users->head;
	char* tokens = strtok(data,",");
	user* elem;
	while(aux != NULL && finished != 1){
		elem = (user*)aux->val;
		if(strcmp(elem->username, tokens) == 0){
			tokens = strtok(NULL,",");
			if(strcmp(elem->password, tokens) == 0){
				finished = 1;
				printf("User %s logued correctly\n", elem->username);
				sendData(getClientFile(msg->referer), "login", "success", elem->username);
			}else{
				finished = 1;
				printf("Incorrect password, User: %s\n", elem->username);
				sendData(getClientFile(msg->referer), "login", "error", "Incorrect Password");
			}
		}
		aux = aux->next;
	}
	if(aux == NULL && finished == 0){
		printf("Incorrect Username");
		sendData(getClientFile(msg->referer), "login", "error", "Incorrect Username");
	}
}

void executeActions(){
	Message* msg;
	while((msg = popMessage()) != NULL){
		if(strcmp(msg->resource, "client") == 0){
			if(strcmp(msg->method, "register") == 0){
				registerClient(msg->referer);
			}
		}else if(strcmp(msg->resource, "login") == 0){
			if(strcmp(msg->method, "register") == 0){
				registerUser(msg);
			}else if(strcmp(msg->method, "login") == 0){
				loginUser(msg);
			}
		}else if(strcmp(msg->resource, "mail") == 0){
			if(strcmp(msg->method, "send") == 0){

			}else if(strcmp(msg->method, "receive") == 0){

			}
		}
	}
}

void writeResponse(int referer, Message* msg){
	int fd = getClientFile(referer);
	int status = 0;
	while((status = write(fd, msg, sizeof(Message)))<=0);
	if(status > 0){
		printf("Wrote Successfully\n");
	}
}

int main() {
	int fd;
	head = tail = NULL;
	clients = NULL;
	users = createList(NULL);
	init_userlist("csv/users.csv", users);
	createBasePipe();
	fd = open("/tmp/serv.xxxxx", O_RDONLY | O_NONBLOCK);
	FD_ZERO (&active_fd_set);
    FD_SET (fd, &active_fd_set);
	printf("Listening on: /tmp/serv.xxxxx\n");
	while(1){
		listenForConnection(fd);
		executeActions();
	}
	return 1;
}
