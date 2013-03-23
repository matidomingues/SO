#include "linkedlist.h"

int main() {
	struct node* list = createlist(3);
	addnode(4, true);
	removelastnode();
	printlist();
	return 1;
}
