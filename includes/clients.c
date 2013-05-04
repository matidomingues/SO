#include "clients.h"

Clients* clientsinfo;

void initClients(){
	clientsinfo = NULL;
}

void addClientNode(int pid, int id){
	Clients* data = malloc(sizeof(Clients));
	Clients* aux = clientsinfo;
	data->pid = pid;
	data->id = id;
	data->messages = NULL;
	data->mailList = NULL;
	data->next = NULL;
	if(clientsinfo == NULL){
		clientsinfo = data;
	}else{
		while(aux->next != NULL){
			aux = aux->next;
		}
		if(aux != NULL){
			aux->next = data;
		}
	}
}

void addMessageClient(int pid, Message* msg){
	Clients* aux = clientsinfo;
	while(aux != NULL && aux->pid != pid){
		aux = aux->next;
	}
	if(aux == NULL){
		return;
	}
	MessageList* list = aux->messages;
	MessageList* data = malloc(sizeof(MessageList));
	data->msg = msg;
	data->next = NULL;

	if(list == NULL){
		aux->messages = data;
	}else{
		while(list->next != NULL){
			list = list->next;
		}
		if(list!=NULL){
			list->next = data;
		}
	}
}

void addMailList(int pid, linked_list* mailList){
	Clients* aux = clientsinfo;
	while(aux != NULL && aux->pid != pid){
		aux = aux->next;
	}
	if(aux == NULL){
		return;
	}
	aux->mailList = mailList;
}

linked_list* getMailList(int pid){
		Clients* aux = clientsinfo;
	while(clientsinfo != NULL && clientsinfo->pid != pid){
		aux = aux->next;
	}
	if(aux == NULL){
		return;
	}
	return aux->mailList;
}

int getClientCond(int pid){
	Clients* aux = clientsinfo;
	while(aux != NULL && aux->pid != pid){
		aux = aux->next;
	}
	if(aux != NULL){
		return aux->id;
	}
}

Message* grabMessage(int pid){
	Clients* aux = clientsinfo;
	MessageList* data;
	while(aux != NULL && aux->pid != pid){
		aux=aux->next;
	}
	if(aux != NULL && aux->messages != NULL){
		data = aux->messages;
		aux->messages = aux->messages->next;
		return data->msg;
	}
	return NULL;

}