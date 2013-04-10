#include "csv.h"

int main() {

	struct linked_list* userlist = createlist(NULL );
	init_userlist("test.csv", userlist);
	//printlist(userlist);
	printf("%s\n", ((user*) (userlist->head->val))->username);
	printf("%s\n", ((user*) (userlist->head->val))->password);
	printf("%s\n", ((user*) (userlist->head->val))->modification_date);
	printf("%s\n", ((user*) (userlist->head->val))->registration_date);
	printf("%s\n", ((user*) (userlist->head->val))->tarifa);
	printf("%s\n", ((user*) (userlist->head->next->val))->username);
	printf("%s\n", ((user*) (userlist->head->next->val))->password);
	printf("%s\n", ((user*) (userlist->head->next->val))->modification_date);
	printf("%s\n", ((user*) (userlist->head->next->val))->registration_date);
	printf("%f\n", ((user*) (userlist->head->next->val))->tarifa);
	printf("%s\n", ((user*) (userlist->last->val))->username);
	printf("%s\n", ((user*) (userlist->last->val))->password);
	printf("%s\n", ((user*) (userlist->last->val))->modification_date);
	printf("%s\n", ((user*) (userlist->last->val))->registration_date);
	printf("%f\n", ((user*) (userlist->last->val))->tarifa);

	/*struct linked_list* list = createlist(5);
	 addnode(list, 1, 1);
	 printf("%d", (list->head)->val);
	 printf("%d", (list->last)->val);
	 addnode(list, 2, 1);
	 printf("%d", (list->head)->val);
	 printf("%d", (list->last)->val);
	 addnode(list, 3, 1);
	 printf("%d", (list->head)->val);
	 printf("%d", (list->last)->val);*/

	/*char m[10][10];
	 strcpy(m[0], "hola");
	 char* a;
	 strcpy(a,m[0]);

	 printf(m[0]);
	 printf(a);
	 strcpy(m[0], "chau");
	 printf(m[0]);
	 printf(a);*/

}
