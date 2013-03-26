#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
#endif

#include <stdio.h>
#include <stdbool.h>

typedef struct linked_list {
	struct node* head;
	struct node* last;
	int length;
} linked_list;

typedef struct node {
	void* val;
	struct node* next;
} node;

/*Crea una lista*/
struct linked_list* createlist(void*);

/*Agrega un elemento*/
void addnode(struct linked_list*, void*, bool);

/*Remueve el primer elemento*/
void removefirstnode(struct linked_list*);

/*Remueve el ultimo elemento*/
void removelastnode(struct linked_list*);

/*Remueve un elemento*/
void removenode(struct linked_list*, struct node*);

/*Busca un elemento y lo devuelve*/
struct node* search(struct linked_list*, void*);

/*Indica si la lista se encuentra vacia*/
bool isempty(struct linked_list*);

/*Imprime la lista en la salida estandar*/
void printlist(struct linked_list*);

/*Devuelve la longitud de la lista*/
int length(struct linked_list*);
