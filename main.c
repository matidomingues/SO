#include "linkedlist.h"
#include "message.h"
#include <unistd.h>

int main() {

	struct linked_list* list = createlist(10);
	addnode(list, 2, true);
	printlist(list);
	removelastnode(list);
	printlist(list);
	return 1;
}
