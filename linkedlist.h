#ifndef _LINKEDLIST_H_
#define _LINKEDLIST_H_
extern struct node *head;
extern struct node *last;
#endif

#include <stdio.h>
#include <stdbool.h>

typedef struct node {
	void* val;
	struct node* next;
} node;

struct node* createlist(void*);

struct node* addnode(void*, bool);

void removelastnode();

void removenode(struct node*);

struct node* search(void*);

void printlist();
