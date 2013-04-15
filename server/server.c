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
#include "../includes/mail.h"

#include <signal.h>

Task* head;
Task* tail;
Client* clients;
linked_list* users;
fd_set active_fd_set;
int reader;

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
	if((tokens = strtok(data,",")) != NULL){
		time(&timer);
		elem->registration_date = timer;
		//elem->name = malloc(strlen(tokens));
		//strcpy(elem->name, tokens);
		if((tokens = strtok(NULL,",")) != NULL){
			elem->username = malloc(strlen(tokens));
			strcpy(elem->username, tokens);
			if((tokens = strtok(NULL,",")) != NULL){
				elem->password = malloc(strlen(tokens));
				strcpy(elem->password, tokens);
				elem->fee = 0;
				elem->mail_list = createList(NULL);

				addNode(users, elem, true);
				printf("Registered User: %s\n", elem->username);
				sendData(getClientFile(msg->referer), "register", "success", "");
			}else{
				sendData(getClientFile(msg->referer), "register", "error", "");
			}
		}else{
			sendData(getClientFile(msg->referer), "register", "error", "");
		}
	}else{
		sendData(getClientFile(msg->referer), "register", "error", "");
	}
}

user* findUserByUsername(char* username){
	node* aux = users->head;
	user* node;
	while(aux != NULL){
		node = (user*) aux->val;
		if(strcmp(node->username, username) == 0){
			return node;
		}
		aux = aux->next;
	}
	return NULL;
}

void loginUser(Message* msg){
	int finished = 0;
	char* data = msg->body;
	node* aux = users->head;
	char* tokens;
	if((tokens = strtok(data,",")) == NULL){
		sendData(getClientFile(msg->referer), "login", "error", "Bad Format");
		return;
	}
	user* elem;
	if((elem = findUserByUsername(tokens)) != NULL){
		if((tokens = strtok(NULL,",")) == NULL){
			sendData(getClientFile(msg->referer), "login", "error", "Bad Format");
			return;
		}
		if(strcmp(elem->password, tokens) == 0){
			printf("User %s logued correctly\n", elem->username);
			sendData(getClientFile(msg->referer), "login", "success", elem->username);
		}else{
			printf("Incorrect password, User: %s\n", elem->username);
			sendData(getClientFile(msg->referer), "login", "error", "Incorrect Password");
		}
	}else{
		printf("Incorrect Username");
		sendData(getClientFile(msg->referer), "login", "error", "Incorrect Username");
	}
}

void sendAllMails(int client, user* elem){
	printf("Sending mails of: %s\n", elem->username);
	node* aux = elem->mail_list->head;
	while(aux != NULL){
		write(client, (mail*)aux->val, sizeof(mail));
		aux = aux->next;
	}
}

void sendEmails(Message* msg){
	user* elem = findUserByUsername(msg->body);
	char* num;
	int client = getClientFile(msg->referer);
	if(elem->mail_list == NULL){
		sendData(client, "mail", "receive", "0");
	}else{
		sprintf(num,"%d",elem->mail_list->length);
		sendData(client, "mail", "receive", (char*)num);
		sendAllMails(client, elem);
	}
}

void recieveEmail(Message* msg){
	int status = 0;
	user* data;
	int client = getClientFile(msg->referer);
	sendData(client,"mail", "continue", "");
	mail* info = malloc(sizeof(mail));
	if(select(FD_SETSIZE, &active_fd_set, NULL, NULL, NULL) <0){
		perror("select");
	}
	status = read(reader, info, sizeof(mail));
	if(status <= 0){
		free(info);
		sendData(client, "mail", "error", "Mail Not Sent\n");
		//perror("read");
	}else if(status >=1){
		data = findUserByUsername(info->to);
		if(data == NULL){
			sendData(client, "mail", "error", "Wrong Username\n");
		}else{
			printf("Recieved email from %s\n", info->from);
			addNode(data->mail_list, info, true);
			sendData(client, "mail", "success", "Mail Sent Correctly\n");
			if(info->attachments[0] != '0'){
				FILE *fp = fopen(info->attachments, "r");
				if (fp == NULL) {
					data->fee +=1;
					printf("While opening file %s.\n", info->attachments);
					perror("File open error");
				}else{
					struct stat st;
					stat(info->attachments, &st);
					int filesize = st.st_size;
					data->fee += filesize;
				}
			}else{
				data->fee +=1; 
			}
		}
	}
}

void sendUserFee(Message* msg){
	char* num;
	user* data = findUserByUsername(msg->body);
	sprintf(num, "%f", data->fee);
	printf("Sending %s fee\n", data->username);
	sendData(getClientFile(msg->referer), "user", "fee", (char*)num);
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
				recieveEmail(msg);
			}else if(strcmp(msg->method, "receive") == 0){
				sendEmails(msg);
			}
		}else if(strcmp(msg->resource, "user") == 0){
			if(strcmp(msg->method, "fee") == 0){
				sendUserFee(msg);
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

void dumpAll(int sig) {
	signal(sig,SIG_IGN);
	printf("Dumping data \n");
	dumpUsersToCSVFile(users);
	int i;
	node* current = users->head;
	for (i = 0; i < length(users); i++) {
		dumpMailsToCSVFile(((user*) (current->val))->mail_list, (user*)current->val);
		current = current->next;
	}
	exit(0);
}

int main() {
	int fd;
	head = tail = NULL;
	clients = NULL;
	users = createList(NULL);
	initUserList("csv/users.csv", users);
	createBasePipe();
	reader = fd = open("/tmp/serv.xxxxx", O_RDONLY | O_NONBLOCK);
	FD_ZERO (&active_fd_set);
    FD_SET (fd, &active_fd_set);
	printf("Listening on: /tmp/serv.xxxxx\n");
	signal(SIGINT, dumpAll);
	while(1){
		listenForConnection(fd);
		executeActions();
	}
	return 1;
}
