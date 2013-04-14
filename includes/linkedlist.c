#include "linkedlist.h"
#include <stdlib.h>

linked_list* createList(void* val) {

	linked_list *list = (linked_list*) malloc(
			sizeof(linked_list));

	if (val != NULL ) {

		node *first_node = (node*) malloc(sizeof(node));

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

void addNode(linked_list* list, void* val, bool addtoend) {

	node* new_node = (node*) malloc(sizeof(node));

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

void removefirstnode(linked_list* list) {
	if (isempty(list)) {
		printf("Cannot remove first node, list is empty.\n");
		return;
	}

	node *temp = list->head;

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

	node *current = list->head;

	if (current != NULL && current->next != NULL ) {
		node *prev;

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

void removenode(linked_list* list, node* n) {

}

node* search(linked_list* list, void* val) {

	node *n = (node*) malloc(sizeof(node));

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

bool isempty(linked_list* list) {
	return list->length == 0;
}

void printlist(linked_list* list) {

	if (isempty(list)) {
		printf("List is empty.\n");
		return;
	}

	node *current = (node*) malloc(sizeof(node));

	current = list->head;

	while (current != NULL ) {
		//printf("%d", (int) current->val); //TODO edit for generic printout
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

int length(linked_list* list) {
	return list->length;
}
