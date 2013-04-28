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
#include "../includes/transporter.h"

#include <signal.h>
#include <pthread.h>

Message* fillMessageData(char* resource, char* method, char* body);
void pushMessage(Message* msg);

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t newMessage;

Task* head;
Task* tail;
linked_list* users;
int reader;

void ManageClient(int referer){
	openClient(referer);
	sendData(referer, fillMessageData("client", "success", ""), sizeof(Message));
}

static void *
clientConn(void* msg){
	int pid = ((Message*)msg)->referer;
	//openClient(pid);
	sendData(pid, fillMessageData("client", "success", ""), sizeof(Message));
	// while(1){
	// 	Message* data = (Message*)listenMessage(0, sizeof(Message));
	// 	pthread_mutex_lock(&mut);
	// 	if(data != NULL){
	// 		pushMessage(data);
	// 		pthread_cond_signal(&newMessage);
	// 	}
	// 	pthread_mutex_unlock(&mut);
	// }
}


Task* newTaskNode(){
	Task* task = malloc(sizeof(Task));
	return task;
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


Message* fillMessageData(char* resource, char* method, char* body){
	Message* msg = malloc(sizeof(Message)); 
	strcpy(msg->protocol, "TPSO 0.12");
	strcpy(msg->method, method);
	strcpy(msg->resource, resource);
	time(&(msg->time));
	msg->referer = getpid();
	strcpy(msg->body, body);
	return msg;

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
				sendData(msg->referer, fillMessageData("register", "success", ""), sizeof(Message));
			}else{
				sendData(msg->referer, fillMessageData("register", "error", ""), sizeof(Message));
			}
		}else{
			sendData(msg->referer, fillMessageData("register", "error", ""), sizeof(Message));
		}
	}else{
		sendData(msg->referer, fillMessageData("register", "error", ""), sizeof(Message));
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
		sendData(msg->referer, fillMessageData("login", "error", "Bad Format"), sizeof(Message));
		return;
	}
	user* elem;
	if((elem = findUserByUsername(tokens)) != NULL){
		if((tokens = strtok(NULL,",")) == NULL){
			sendData(msg->referer, fillMessageData("login", "error", "Bad Format"), sizeof(Message));
			return;
		}
		if(strcmp(elem->password, tokens) == 0){
			printf("User %s logued correctly\n", elem->username);
			sendData(msg->referer, fillMessageData("login", "success", elem->username), sizeof(Message));
		}else{
			printf("Incorrect password, User: %s\n", elem->username);
			sendData(msg->referer, fillMessageData("login", "error", "Incorrect Password"), sizeof(Message));
		}
	}else{
		printf("Incorrect Username");
		sendData(msg->referer, fillMessageData("login", "error", "Incorrect Username"), sizeof(Message));
	}
}

void sendAllMails(int client, user* elem){
	printf("Sending mails of: %s\n", elem->username);
	node* aux = elem->mail_list->head;
	while(aux != NULL){
		sendData(client, (mail*)aux->val, sizeof(mail));
		aux = aux->next;
	}
}

void sendEmails(Message* msg){
	user* elem = findUserByUsername(msg->body);
	char* num;
	if(elem->mail_list == NULL){
		sendData(msg->referer, fillMessageData("mail", "receive", "0"), sizeof(mail));
	}else{
		sprintf(num,"%d",elem->mail_list->length);
		sendData(msg->referer, fillMessageData("mail", "receive", (char*)num), sizeof(Message));
		sendAllMails(msg->referer, elem);
	}
}

void recieveEmail(Message* msg){
	int status = 0;
	user* data;
	sendData(msg->referer, fillMessageData("mail", "continue", ""), sizeof(Message));
	mail* info = (mail*)listenMessage(0, sizeof(mail));
	if(info == NULL){
		sendData(msg->referer, fillMessageData("mail", "error", "Mail Not Sent\n"), sizeof(Message));
		//perror("read");
	}else if(info != NULL){
		data = findUserByUsername(info->to);
		if(data == NULL){
			sendData(msg->referer, fillMessageData("mail", "error", "Wrong Username\n"), sizeof(Message));
		}else{
			printf("Recieved email from %s\n", info->from);
			addNode(data->mail_list, info, true);
			sendData(msg->referer, fillMessageData("mail", "success", "Mail Sent Correctly\n"), sizeof(Message));
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
	sendData(msg->referer, fillMessageData("user", "fee", (char*)num), sizeof(Message));
}

void dumpAll(int sig) {
	signal(sig,SIG_IGN);
	closeConnection(0);
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

static void *
executeActions(void *arg){
	Message* msg;
	while(1){
		pthread_mutex_lock(&mut);
		msg = popMessage();
	
		if(msg != NULL){
			if(strcmp(msg->resource, "client") == 0){
				if(strcmp(msg->method, "register") == 0){
					ManageClient(msg->referer);
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
		else{
			/* Sleep until new Messages */
			printf("sleeping\n");
			pthread_cond_wait(&newMessage, &mut);
			printf("woke up\n");
		}
		pthread_mutex_unlock(&mut);
	}
}


static void createExecuteActions(){
	pthread_t execute_thr;
	pthread_create(&execute_thr, NULL, executeActions, NULL);
	pthread_detach(execute_thr);
}

static void cloneConnection(Message* msg){
	pthread_t conn_thr;
	pthread_create(&conn_thr, NULL, clientConn, msg);
	pthread_detach(conn_thr);
}

void printMessage(Message* msg){
	printf("Resource: %s\n", msg->resource);
	printf("Method: %s\n", msg->method);
	printf("Referer: %d\n", msg->referer);
	printf("Body: %s\n", msg->body);
}

int main() {
	int fd;
	Message* data;
	head = tail = NULL;
	users = createList(NULL);
	initUserList("csv/users.csv", users);
	createConnection(0);
	signal(SIGINT, dumpAll);

	pthread_mutex_init( &mut, NULL );
    pthread_cond_init( &newMessage, NULL );

	createExecuteActions();

	while(1){
		fd = acceptConnection(0);
		printf("acepto\n");
		data = (Message*)listenMessage(fd, sizeof(Message));
		if(data != NULL){
			printMessage(data);
			if(strcmp(data->resource, "client") == 0){
				if(strcmp(data->method, "register") == 0){
					printf("referer: %d\n", data->referer);
					registerClient(data->referer, fd);
					cloneConnection(data);	
				}else{
					printf("Error on basic connection to server 2\n");
				}
			}else{
				printf("Error on basic connection to server 1\n");
			}

		}else{
			printf("data is null\n");
		}
	}
	return 1;
}
