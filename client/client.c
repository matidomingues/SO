#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>

#include "../includes/message.h"
#include "../includes/mail.h"
#include <time.h>

Message* prepareMessage();
Message* fillMessageData(Message* msg, char* method, char* resource, char* body);
void dispatchEvent(Message* msg);
void listenForConnection();
char* createPipe();
void sendData(char* resource, char* method, char* body);
void setLogin(char* name);
void writeEmail();
void grabNewEmails();
void readConsole();

int status = 0;
int receiver;
int sender;
char username[15];
fd_set active_fd_set;

Message* prepareMessage() {
	Message* info = (Message*) malloc(sizeof(Message));
	return info;
}

Message* fillMessageData(Message* msg, char* method, char* resource, char* body) {
	strcpy(msg->protocol, "TPSO 0.12");
	strcpy(msg->method, method);
	strcpy(msg->resource, resource);
	time(&(msg->time));
	msg->referer = getpid();
	strcpy(msg->body, body);
	return msg;

}

void recieveEmails(int num){
	int i, error;
	mail* msg = malloc(sizeof(mail));
	for(i=0; i<num; i++){
		if (select(FD_SETSIZE, &active_fd_set, NULL, NULL, NULL ) < 0) {
			perror("select");
		}
		error = read(receiver, msg, sizeof(mail));
		if (error == -1) {
			printf("Error on mail\n");
			//perror("read");
		} else if (error >= 1) {
			printf("%d: From: %s, Subject: %s Time: %s", i+1, msg->from, msg->header, ctime(&(msg->senttime)));
			printf("Message:%s\n", msg->body);
			if(msg->attachments[0] != '0'){
				printf("Attachment: %s\n", msg->attachments);
			}
		}
	}
	free(msg);
}

void printMessage(Message* msg){
	printf("Resource: %s\n", msg->resource);
	printf("Method: %s\n", msg->method);
	printf("Referer: %d\n", msg->referer);
	printf("Body: %s\n", msg->body);
}

void dispatchEvent(Message* msg) {
	printMessage(msg);
	if (strcmp(msg->resource, "login") == 0) {
		if (strcmp(msg->method, "success") == 0) {
			setLogin(msg->body);
		} else if (strcmp(msg->method, "error") == 0) {
			printf("%s\n", msg->body);
		}
	} else if (strcmp(msg->resource, "register") == 0) {
		if (strcmp(msg->method, "success") == 0) {
			printf("User Registered Successfully, proceed to Log In\n");
		} else if (strcmp(msg->method, "error") == 0) {
			printf("%s\n", msg->body);
		}
	} else if (strcmp(msg->resource, "client") == 0) {
		if (strcmp(msg->method, "success") == 0) {
			printf("Client Registered Successfully\n");
		}
	}else if(strcmp(msg->resource, "mail") == 0){
		if(strcmp(msg->method, "receive") == 0){
			recieveEmails(atoi(msg->body));
		}else if(strcmp(msg->method, "success") == 0){
			printf("%s\n", msg->body);
		}else if(strcmp(msg->method, "error") == 0){
			printf("%s\n", msg->body);
		}else if(strcmp(msg->method, "continue") == 0){
			printf("Continue with message\n");
		}
	}else if(strcmp(msg->resource, "user") == 0){
		if(strcmp(msg->method, "fee") == 0){
			printf("Tarifacion: %s\n", msg->body);
		}
	}
}

void listenForConnection() {
	int error = 0;
	Message* msg = malloc(sizeof(Message));
	if (select(FD_SETSIZE, &active_fd_set, NULL, NULL, NULL ) < 0) {
		perror("select");
	}
	error = read(receiver, msg, sizeof(Message));
	if (error == -1) {
		free(msg);
		//perror("read");
	} else if (error >= 1) {
		dispatchEvent(msg);
	}

}

char* createPipe() {
	char pid[7];
	char* route = malloc(sizeof(char) * 16);
	sprintf(pid, "%d", getpid());
	strcpy(route, "/tmp/serv.");
	strcat(route, pid);
	printf("%d", mkfifo(route, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH));
	return route;
}

void sendData(char* resource, char* method, char* body) {
	int status;
	Message* msg = prepareMessage();
	msg = fillMessageData(msg, method, resource, body);
	while ((status = write(sender, msg, sizeof(Message))) == 0) {
		printf("paso2\n");
	}
	if (status == -1) {
		printf("ERROR ON PIPE write\n");
	}
	free(msg);
	listenForConnection();
}

void setLogin(char* name) {
	strcpy(username, name);
	status = 1;
	printf("Welcome %s to your Mail\n", name);
}

void writeEmail() {
	time_t sendtime;
	time(&sendtime);
	mail* elem = malloc(sizeof(mail));
	int num;
	printf("Ingrese destinatario\n");
	scanf("%15s",elem->to);
	strcpy(elem->from, username);
	printf("Ingrese el Asunto\n");
	scanf("%30s", elem->header);
	printf("Ingrese el Mensaje\n");
	scanf("%100s",elem->body);
	printf("1: Agregar Attachment\n2: No Agregar Attachment\n");
	scanf("%d",&num);
	if(num == 1){
		printf("Ingrese el path del attachment\n");
		scanf("%30s", elem->attachments);
	}else{
		strcpy(elem->attachments, "0");
	}
	elem->read = 0;
	elem->senttime = sendtime;
	sendData("mail","send", "1");
	printf("wrote: %d\n", (int)write(sender, elem, sizeof(mail)));
	free(elem);
	listenForConnection();
}

void grabNewEmails() {
	sendData("mail", "receive", username);
}

void getFee(){
	sendData("user", "fee", username);
}

void readConsole() {
	int a;
	char result[30];
	if (status == 0) {
		printf("Ingrese Numero de Opcion:\n");
		printf("1: Login\n");
		printf("2: Registrarse\n");
		scanf("%d", &a);
		if (a == 1) {
			printf(
					"Ingrese Usuario y Contraseña\nSeparados por ',' de la forma Usuario,Contraseña\n");
			scanf("%30s", result);
			sendData("login", "login", result);

		} else if (a == 2) {
			printf(
					"Ingrese Nombre, Usuario y Contraseña\nSeparados por ',' de la forma\nNombre,Usuario,Contraseña\n");
			scanf("%30s", result);
			sendData("login", "register", result);

		}
	} else if (status == 1) {
		printf("Ingrese Numero de Opcion:\n");
		printf("1: Enviar Email\n");
		printf("2: Recibir Emails\n");
		printf("3: Consultar Tarifacion\n");
		scanf("%d", &a);
		if (a == 1) {
			writeEmail();
		} else if (a == 2) {
			grabNewEmails();
		} else if( a == 3){
			getFee();
		}
	}
}

int main() {
	char* route;
	route = createPipe();
	printf("%s\n", route);
	sender = open("/tmp/serv.xxxxx", O_WRONLY | O_NONBLOCK);
	receiver = open(route, O_RDONLY | O_NONBLOCK);
	FD_ZERO(&active_fd_set);
	FD_SET(receiver, &active_fd_set);
	sendData("client", "register", "");
	while (1) {
		readConsole();
	}
	return 1;
}
