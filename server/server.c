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
#include "../includes/clients.h"

#include <signal.h>
#include <pthread.h>

static pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t newMessage, newMail, newData[10];
Task* head;
Task* tail;
ListMail* mailhead;
ListMail* mailtail;
linked_list* users;
int clientNum = 0;

void initPthread();
void printMessage(Message* msg);
static void cloneConnection(Message* msg);
static void createExecuteActions();
static void createReciveMails();
void dumpAll(int sig);
void sendUserFee(Message* msg);
void sendEmails(Message* msg);
void loginUser(Message* msg);
user* findUserByUsername(char* username);
void registerUser(Message* msg);
Message* fillMessageData(char* resource, char* method, char* body);
Message* popMessage();
mail* popMail();
void pushMessage(Message* msg);
void pushMail(mail* msg);
Task* newTaskNode();
ListMail* newMailNode();
void clientSendEmails(Message* msg, int id);
void pushWait(int id, Message* data);
void sendAllMails(int pid, linked_list* mails);
int getClientNum();

int getClientNum(){
	int id;
	pthread_mutex_lock(&mut);
	id = clientNum++;
	pthread_mutex_unlock(&mut);
	return id;
}

void clientReciveMails(Message* msg, int id){
	sendData(msg->referer, fillMessageData("mail", "continue", ""), sizeof(Message));
	mail* info = (mail*)listenMessage(msg->referer, sizeof(mail));
	if(info != NULL){
		pthread_mutex_lock(&mut);
		if(findUserByUsername(info->to) != NULL){
			pushMail(info);
			sendData(msg->referer, fillMessageData("mail", "success", "Mail Sent Correctly\n"), sizeof(Message));
			pthread_cond_signal(&newMail);
		}else{
			sendData(msg->referer, fillMessageData("mail", "error", "Wrong Username\n"), sizeof(Message));
		}
		pthread_mutex_unlock(&mut);
	}
}

void sendAllMails(int pid, linked_list* mails){
	pthread_mutex_lock(&mut);
	if(mails == NULL){
		return;
	}
	node* aux = mails->head;
	while(aux != NULL){
		sendData(pid, (mail*)aux->val, sizeof(mail));
		aux = aux->next;
	}
	pthread_mutex_unlock(&mut);
}

void pushWait(int id, Message* data){
	pthread_mutex_lock(&mut);
	pushMessage(data);
	printf("Message Pushed, Waking up Execute\n");
	pthread_cond_signal(&newMessage);
	printf("Sleeping until new Data\n");
	pthread_cond_wait(&newData[id], &mut);
	printf("New Data, Woke Up\n");
	pthread_mutex_unlock(&mut);
}

void clientSendEmails(Message* msg, int id){
	pushWait(id, msg);
	pthread_mutex_lock(&mut);
	Message* data = grabMessage(msg->referer);
	printf("pid: %d\n", msg->referer);
	sendData(msg->referer, data, sizeof(Message));
	pthread_mutex_unlock(&mut);
	sendAllMails(msg->referer, getMailList(msg->referer));
}

static void *
clientConn(void* info){
	Message* msg;
	int pid = ((Message*)info)->referer;
	int id = getClientNum();
	addClientNode(pid, id);

	sendData(pid, fillMessageData("client", "success", ""), sizeof(Message));
	while(1){
		printf("Waiting for new Message\n");
		Message* data = (Message*)listenMessage(pid, sizeof(Message));
		if(data != NULL){
			
			if(strcmp(data->resource, "mail") == 0){
				if(strcmp(data->method, "send") == 0){
					clientReciveMails(data, id);	
				}else if(strcmp(data->method, "receive") == 0){
					clientSendEmails(data, id);
				}
			}else if(strcmp(data->resource, "client") == 0){
				if(strcmp(data->method, "close") == 0){
					printf("%d\n", pid);
					closeConnection(pid);
					break;
				}
			}else{
				pushWait(id,data);
				while((msg = grabMessage(pid)) != NULL){
					sendData(pid, msg, sizeof(Message));
				}
			}
		}
	}
	printf("Salio \n");
	return NULL;
}

ListMail* newMailNode(){
	ListMail* msg = malloc(sizeof(ListMail));
	return msg;
}

Task* newTaskNode(){
	Task* task = malloc(sizeof(Task));
	return task;
}

void pushMail(mail* msg){
	ListMail* elem = newMailNode();
	ListMail* aux;
	elem->msg = msg;
	elem->next = NULL;
	
	if(mailhead == NULL){
		mailhead = mailtail = elem;
	}else{
		mailtail->next = elem;
		mailtail = elem;
	}
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

mail* popMail(){
	mail* msg;
		if(mailhead == NULL){
		return NULL;
	}else{
		msg = mailhead->msg;
		mailhead = mailhead->next;
	}
	return msg;
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
				addMessageClient(msg->referer, fillMessageData("register", "success", ""));
			}else{
				addMessageClient(msg->referer, fillMessageData("register", "error", ""));
			}
		}else{
			addMessageClient(msg->referer, fillMessageData("register", "error", ""));
		}
	}else{
		addMessageClient(msg->referer, fillMessageData("register", "error", ""));
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
		addMessageClient(msg->referer, fillMessageData("login", "error", "Bad Format"));
		return;
	}
	user* elem;
	if((elem = findUserByUsername(tokens)) != NULL){
		if((tokens = strtok(NULL,",")) == NULL){
			addMessageClient(msg->referer, fillMessageData("login", "error", "Bad Format"));
			return;
		}
		if(strcmp(elem->password, tokens) == 0){
			printf("User %s logued correctly\n", elem->username);
			addMessageClient(msg->referer, fillMessageData("login", "success", elem->username));
		}else{
			printf("Incorrect password, User: %s\n", elem->username);
			addMessageClient(msg->referer, fillMessageData("login", "error", "Incorrect Password"));
		}
	}else{
		printf("Incorrect Username\n");
		addMessageClient(msg->referer, fillMessageData("login", "error", "Incorrect Username"));
	}
	printf("salio\n");
}

void sendEmails(Message* msg){
	user* elem = findUserByUsername(msg->body);
	char* num;
	if(elem->mail_list == NULL){
		addMessageClient(msg->referer, fillMessageData("mail", "receive", "0"));
	}else{
		sprintf(num,"%d",elem->mail_list->length);
		addMessageClient(msg->referer, fillMessageData("mail", "receive", (char*)num));
		addMailList(msg->referer, elem->mail_list);
	}
}

void sendUserFee(Message* msg){
	char* num;
	user* data = findUserByUsername(msg->body);
	sprintf(num, "%f", data->fee);
	printf("Sending %s fee\n", data->username);
	addMessageClient(msg->referer, fillMessageData("user", "fee", (char*)num));
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
reciveMails(void* arg){
	mail* info;
	int status = 0;
	user* data;
	while(1){
		pthread_mutex_lock(&mut);
		info = popMail();
		if(info != NULL){
			data = findUserByUsername(info->to);
			if(data == NULL){
				printf("Wrong Username\n");
			}else{
				printf("Recieved email from %s\n", info->from);
				addNode(data->mail_list, info, true);
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
		}else{
			printf("Mails Queue Empty, Sleeping\n");
			pthread_cond_wait(&newMail, &mut);
			printf("Woke Up to Consume Mail List\n");
		}
		pthread_mutex_unlock(&mut);
	}
}

static void *
executeActions(void *arg){
	Message* msg;
	int client;
	while(1){
		pthread_mutex_lock(&mut);
		msg = popMessage();
		if(msg != NULL){
			if(strcmp(msg->resource, "login") == 0){
				if(strcmp(msg->method, "register") == 0){
					registerUser(msg);
				}else if(strcmp(msg->method, "login") == 0){
					loginUser(msg);
				}
			}else if(strcmp(msg->resource, "user") == 0){
				if(strcmp(msg->method, "fee") == 0){
					sendUserFee(msg);
				}
			}else if(strcmp(msg->resource, "mail") == 0){
				if(strcmp(msg->method, "receive") == 0){
					sendEmails(msg);
				}
			}
			client = getClientCond(msg->referer);
			pthread_cond_signal(&newData[client]);
		}
		else{
			/* Sleep until new Messages */
			printf("Stack Empty, Sleeping\n");
			pthread_cond_wait(&newMessage, &mut);
			printf("Woke up to consume Stack\n");
		}
		pthread_mutex_unlock(&mut);
	}
}

static void createReciveMails(){
	pthread_t execute_thr;
	pthread_create(&execute_thr, NULL, reciveMails, NULL);
	pthread_detach(execute_thr);
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

void initPthread(){
	int i;
	pthread_mutex_init( &mut, NULL );
	pthread_cond_init( &newMail, NULL );
	pthread_cond_init( &newMessage, NULL );
	for(i = 0; i < 10; i++){
		pthread_cond_init( &newData[i], NULL );
	}

	createExecuteActions();
	createReciveMails();
}
int main() {
	int fd;
	Message* data;
	head = tail = NULL;
	mailhead = mailtail = NULL;
	users = createList(NULL);
	initUserList("csv/users.csv", users);
	createConnection(0);
	signal(SIGINT, dumpAll);

    initPthread();

	while(1){
		fd = acceptConnection(0);
		data = (Message*)listenMessage(fd, sizeof(Message));
		if(data != NULL){
			if(strcmp(data->resource, "client") == 0){
				if(strcmp(data->method, "register") == 0){
					registerClient(data->referer, fd);
					cloneConnection(data);
				}else{
					printf("Error on basic connection to server 2\n");
				}
			}else{
				printf("Error on basic connection to server 1\n");
			}

		}
	}
	closeConnection(0);
	return 1;
}
