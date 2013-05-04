#ifndef _CLIENTS_H_
#define _CLIENTS_H_

#include "message.h"
#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct _messagelist{
	Message* msg;
	struct _messagelist* next;
}MessageList;

typedef struct _clients{
	int pid;
	int id;
	MessageList* messages;
	linked_list* mailList;
	struct _clients* next;
}Clients;

int getClientCond(int pid);
void addClientNode(int pid, int id);
Message* grabMessage(int pid);
linked_list* getMailList(int pid);
void addMailList(int pid, linked_list* mailList);
void initClients();

#endif