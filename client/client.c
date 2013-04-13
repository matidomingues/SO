#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include <string.h>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>

#include "../includes/message.h"
#include <time.h>

int status = 0;
int sender;
int fd;
char username[7];

Message* prepareMessage(){
	Message* info = (Message*) malloc(sizeof(Message)); 
	return info;
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

char* createPipe(){
	char pid[7];
	char* route = malloc(sizeof(char)*16);
	sprintf(pid,"%d", getpid());
	strcpy(route, "/tmp/serv.");
	strcat(route,pid);
	printf("%d",mkfifo(route, S_IWUSR | S_IRUSR | S_IRGRP | S_IROTH));
	return route;
}

void sendData(char* resource, char* method, char* body){
	int status;
	Message* msg = prepareMessage();
	msg = fillMessageData(msg, method, resource, body);
	while ((status = write(sender, msg, sizeof(Message))) == 0){
		printf("paso2\n");
	}
	if(status == -1){
		printf("ERROR ON PIPE write");
	}else{
		printf("wrote");
	}
	free(msg);
}

void setLogin(char* name){
	strcpy(username, name);
	status = 1;
	printf("Welcome %s to your Mail\n", name);
}

void dispatchEvent(Message* msg){
	if(strcmp(msg->resource, "login") == 0){
		if(strcmp(msg->method, "success") == 0){
			setLogin(msg->body);
		}else if(strcmp(msg->method, "error") == 0){
			printf("Error on login. Try again\n");
		}
	}else if(strcmp(msg->resource, "register") == 0){
		if(strcmp(msg->method, "success") == 0){
			printf("User Registered Successfully, proceed to Log In\n");
		}else if(strcmp(msg->method, "error") == 0){
			printf("Error on registration. Try again\n");
		}
	}
}

void listenForConnection(int fd){
	int status = 0;
	Message* msg = malloc(sizeof(Message));
	status = read(fd, msg, sizeof(Message));
	if(status == -1){
		free(msg);
		//perror("read");
	}else if(status >=1){
		dispatchEvent(msg);
	}
}

void writeEmail(){
	char result[30];
	printf("Send Email");
}

void grabNewEmails(){
	sendData("mail", "receive", "");
}

void readConsole(){
	int a;
	char result[30];
	if(status == 0){
		printf("Ingrese Numero de Opcion:\n");
		printf("1: Login\n");
		printf("2: Registrarse\n");
		scanf("%d",&a);
		if(a == 1){
			printf("Ingrese Usuario y Contraseña\n Separados por ',' de la forma Usuario,Contraseña\n");
			scanf("%30s", result);
			sendData("login", "login", result);
		}else if(a == 2){
			printf("Ingrese Nombre, Usuario y Contraseña\nSeparados por ',' de la forma\nNombre,Usuario,Contraseña\n");
			scanf("%30s", result);
			sendData("login", "register", result);

		}
	}else if(status == 1){
		printf("Ingrese Numero de Opcion:\n");
		printf("1: Enviar Email\n");
		printf("2: Recibir Emails\n");
		scanf("%d",&a);
		if(a == 1){
			writeEmail();	
		}else if(a == 2){
			grabNewEmails();
		}
	}
}

int main() {
	printf("make anda\n");
	int fd2;
	char* route; 
	route = createPipe(); 
	printf("%s\n", route);
	sender = open("/tmp/serv.xxxxx", O_WRONLY | O_NONBLOCK);
	fd2 = open(route, O_RDONLY | O_NONBLOCK);
	sendData("client", "register", " ");
	while(1){
		listenForConnection(fd2);
		readConsole();
	}
	return 1;
}
