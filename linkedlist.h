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

struct linked_list* createlist(void*);

struct linked_list* addnode(struct linked_list*, void*, bool);

void removelastnode(struct linked_list*);

void removenode(struct linked_list*, struct node*);

struct node* search(struct linked_list*, void*);

void printlist(struct linked_list*);
