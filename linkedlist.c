#include "linkedlist.h"
#include <stdlib.h>

struct node *head;
struct node *last;

struct linked_list* createlist(void* val) {

	struct linked_list *list = (struct linked_list*) malloc(
			sizeof(struct linked_list));
	struct node *first_node = (struct node*) malloc(sizeof(struct node));

	first_node->val = val;
	first_node->next = NULL;

	list->head = first_node;
	list->last = first_node;
	list->length++;
	return list;
}

struct linked_list* addnode(linked_list* list, void* val, bool addtoend) {

	//Es el primero
	if (list->head == NULL) {
		//printf("Adding first element");
		return (createlist(val));
	}

	struct node* new_node = (struct node*) malloc(sizeof(struct node));

	new_node->val = val;
	new_node->next = NULL;

	if (addtoend) {
		//printf("Adding to end");
		(list->last)->next = new_node;
		list->last = new_node;
	} else {
		//printf("Adding to beggining");
		new_node->next = list->head;
		list->head = new_node;
	}
	list->length++;
	return list;
}

void removelastnode(linked_list* list) {

	struct node *current = (struct node*) malloc(sizeof(struct node));
	struct node *prev = (struct node*) malloc(sizeof(struct node));

	current = list->head;

	if (current != NULL && current->next != NULL) {
		prev = current;
		current = current->next;

		while (current->next != NULL) {
			prev = current;
			current = current->next;
		}
		prev->next = NULL;
		list->last = prev;
	} else {
		//Hay un solo elemento o ninguno
		list->head = list->last = NULL;
		free(prev);
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
			n = head->next;
		}
	} while (n != NULL);
	return n;
}

void printlist(linked_list* list) {

	struct node *current = (struct node*) malloc(sizeof(struct node));

	current = list->head;

	while (current != NULL) {
		printf("%d", current->val); //TODO edit for generic printout
		printf("->");
		if (current->next == NULL) {
			printf("NULL\n");
			return;
		} else {
			current = current->next;
		}
	}
	free(current);
}
