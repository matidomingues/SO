#include "linkedlist.h"
#include <stdlib.h>

struct linked_list* createlist(void* val) {

	struct linked_list *list = (struct linked_list*) malloc(
			sizeof(struct linked_list));

	if (val != NULL ) {

		struct node *first_node = (struct node*) malloc(sizeof(struct node));

		printf("Creating list.\n");

		first_node->val = val;
		first_node->next = NULL;

		list->head = first_node;
		list->last = first_node;
		list->length = 1;
		return list;
	} else {
		printf("Creating NULL list.\n");

		list->head = NULL;
		list->last = NULL;
		list->length = 0;
		return list;
	}
}

void addnode(linked_list* list, void* val, bool addtoend) {

	struct node* new_node = (struct node*) malloc(sizeof(struct node));

	new_node->val = val;
	new_node->next = NULL;

	if (isempty(list)) { //Primer elemento
		printf("Adding first element.\n");
		list->head = list->last = new_node;
	} else {
		if (addtoend) {
			printf("Adding node to end.\n");
			(list->last)->next = new_node;
			list->last = new_node;
		} else {
			printf("Adding to beggining.\n");
			new_node->next = list->head;
			list->head = new_node;
		}
	}
	(list->length)++;
}

void removefirstnode(struct linked_list* list) {
	if (isempty(list)) {
		printf("Cannot remove first node, list is empty.\n");
		return;
	}

	struct node *temp = list->head;

	if ((list->head)->next != NULL ) {
		printf("Removing first element.\n");
		list->head = (list->head)->next;
		free(temp);
	} else { //Hay un solo elemento
		printf("Removing final element.\n");
		list->head = NULL;
		free(temp);
	}
	list->length--;
}

void removelastnode(linked_list* list) {

	if (isempty(list)) {
		printf("Cannot remove last node, list is empty.\n");
		return;
	}

	struct node *current = list->head;

	if (current != NULL && current->next != NULL ) {
		struct node *prev;

		prev = current;
		current = current->next;

		while (current->next != NULL ) {
			prev = current;
			current = current->next;
		}
		printf("Removing last element.\n");
		prev->next = NULL;
		list->last = prev;
	} else {
		//Hay un solo elemento o ninguno
		printf("Removing final element.\n");
		list->head = list->last = NULL;
	}
	list->length--;
	free(current);
}

void removenode(linked_list* list, struct node* n) {

}

struct node* search(linked_list* list, void* val) {

	struct node *n = (struct node*) malloc(sizeof(struct node));

	n = list->head;

	do {
		if (n->val == val) {
			return n;
		} else {
			n = list->head->next;
		}
	} while (n != NULL );
	return n;
}

bool isempty(struct linked_list* list) {
	return list->length == 0;
}

void printlist(linked_list* list) {

	if (isempty(list)) {
		printf("List is empty.\n");
		return;
	}

	struct node *current = (struct node*) malloc(sizeof(struct node));

	current = list->head;

	while (current != NULL ) {
		printf("%d", (int) current->val); //TODO edit for generic printout
		printf("->");
		if (current->next == NULL ) {
			printf("NULL\n");
			return;
		} else {
			current = current->next;
		}
	}
	free(current);
}

int length(struct linked_list* list) {
	return list->length;
}
