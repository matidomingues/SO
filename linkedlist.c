#include "linkedlist.h"
#include <stdlib.h>

struct node *head;
struct node *last;

struct node* createlist(void* val) {
	struct node *n = (struct node*) malloc(sizeof(struct node));

	n->val = val;
	n->next = NULL;

	head = n;
	last = n;

	return n;
}

struct node* addnode(void* val, bool addtoend) {

	//Es el primero
	if (NULL == head) {
		return (createlist(val));
	}

	struct node *n = (struct node*) malloc(sizeof(struct node));

	n->val = val;
	n->next = NULL;

	if (addtoend) {
		last->next = n;
		last = n;
	} else {
		n->next = head;
		head = n;
	}
	return n;
}

void removelastnode() {

	struct node *current = (struct node*) malloc(sizeof(struct node));
	struct node *prev = (struct node*) malloc(sizeof(struct node));

	current = head;

	if (current != NULL && current->next != NULL ) {
		prev = current;
		current = current->next;

		while (current->next != NULL ) {
			prev = current;
			current = current->next;
		}
	} else {
		//hay un solo elemento o ninguno
		head = last = NULL;
		free(current);
		free(prev);
		return;
	}
	prev->next = NULL;
	last = prev;
	free(current);
	//free(prev);
}

void removenode(struct node* n) {

}

struct node* search(void* val) {

	struct node *n = (struct node*) malloc(sizeof(struct node));

	n = head;

	do {
		if (n->val == val) {
			return n;
		} else {
			n = head->next;
		}
	} while (n != NULL );
	return n;
}

void printlist() {

	struct node *current = (struct node*) malloc(sizeof(struct node));

	current = head;

	while (current != NULL ) {
		printf("%d", current->val);
		printf("->");
		if (current->next == NULL ) {
			printf("NULL");
			return;
		} else {
			current = current->next;
		}
	}
	free(current);
}
