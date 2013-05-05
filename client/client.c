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
#include "../includes/transporter.h"

#include <time.h>

Message* fillMessageData(char* resource, char* method, char* body);
void dispatchEvent(Message* msg);
void setLogin(char* name);
void writeEmail();
void grabNewEmails();
void readConsole();

int status = 0;
char username[15];

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

void getResponce(int pid){
	Message* msg = listenMessage(pid, sizeof(Message));
	if(msg != NULL){
		dispatchEvent(msg);
	}
}

void recieveEmails(int num){
	int i, error;
	mail* msg;
	for(i=0; i<num; i++){
		msg = listenMessage(0, sizeof(mail));
		if(msg != NULL){
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

void clientRegistered(){
	printf("User registered Successfully, switching socket\n");
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
			clientRegistered();
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
	sendData(0, fillMessageData("mail","send", "1"), sizeof(Message));
	getResponce(0);
	sendData(0, elem, sizeof(mail));
	//free(elem);
	getResponce(0);
}

void grabNewEmails() {
	sendData(0, fillMessageData("mail", "receive", username), sizeof(Message));
	getResponce(0);
}

void getFee(){
	sendData(0, fillMessageData("user", "fee", username), sizeof(Message));
	getResponce(0);
}

void logOut(){
	status = 0;
}

void readConsole() {
	int a;
	char result[30];
	if (status == 0) {
		printf("Ingrese Numero de Opcion:\n");
		printf("1: Login\n");
		printf("2: Registrarse\n");
		printf("3: Cerrar\n");
		scanf("%d", &a);
		if (a == 1) {
			printf(
					"Ingrese Usuario y Contraseña\nSeparados por ',' de la forma Usuario,Contraseña\n");
			scanf("%30s", result);
			sendData(0, fillMessageData("login", "login", result), sizeof(Message));
			getResponce(0);

		} else if (a == 2) {
			printf(
					"Ingrese Nombre, Usuario y Contraseña\nSeparados por ',' de la forma\nNombre,Usuario,Contraseña\n");
			scanf("%30s", result);
			sendData(0, fillMessageData("login", "register", result), sizeof(Message));
			getResponce(0);

		} else if( a == 3){
			sendData(0, fillMessageData("client", "close", ""), sizeof(Message));
			closeConnection(0);
			exit(0);
		}
	} else if (status == 1) {
		printf("Ingrese Numero de Opcion:\n");
		printf("1: Enviar Email\n");
		printf("2: Recibir Emails\n");
		printf("3: Consultar Tarifacion\n");
		printf("4: Log Out\n");
		scanf("%d", &a);
		if (a == 1) {
			writeEmail();
		} else if (a == 2) {
			grabNewEmails();
		} else if( a == 3){
			getFee();
		} else if( a == 4){
			logOut();
		}
	}
}

int main() {
	openClient(0);
	sendData(0, fillMessageData("client", "register", ""), sizeof(Message));
	getResponce(0);
	while (1) {
		readConsole();
	}
	return 1;
}
