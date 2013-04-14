#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_

#include <stdio.h>
#include <stdbool.h>

typedef struct _node {
	void* val;
	struct _node * next;
}node;

typedef struct _linked_list {
	struct _node * head;
	struct _node * last;
	int length;
}linked_list;

/*Crea una lista*/
linked_list* createList(void* val);

/*Agrega un elemento*/
void addNode(linked_list* list, void* val, bool addtoend);

/*Remueve el primer elemento*/
void removefirstnode(linked_list* list);

/*Remueve el ultimo elemento*/
void removelastnode(linked_list* list);

/*Remueve un elemento*/
void removenode(linked_list* list, node* n);

/*Busca un elemento y lo devuelve*/
node* search(linked_list* list, void* val);

/*Indica si la lista se encuentra vacia*/
bool isempty(linked_list* list);

/*Imprime la lista en la salida estandar*/
void printlist(linked_list* list);

/*Devuelve la longitud de la lista*/
int length(linked_list* list);

bool hasNext(node* node);

node* next(node* node);

#endif


