#include "includes\csv.h"
#include "includes\linkedlist.h"
int main() {

	linked_list* userlist = createList(NULL );
	init_userlist("csv/users.csv", userlist);

	linked_list* maillist = createList(NULL );
	init_messagelist("csv/Mails/username1.csv", maillist);
	//printlist(userlist);

	printf("%s\n", ((user*) (userlist->head->val))->username);
	printf("%s\n", ((user*) (userlist->head->val))->password);
	printf("%s\n", ((user*) (userlist->head->val))->registration_date);
	printf("%s\n", ((user*) (userlist->head->val))->modification_date);
	printf("%s\n", ((user*) (userlist->head->val))->fee);
	printf("%s\n", ((user*) (userlist->head->next->val))->username);
	printf("%s\n", ((user*) (userlist->head->next->val))->password);
	printf("%s\n", ((user*) (userlist->head->next->val))->registration_date);
	printf("%s\n", ((user*) (userlist->head->next->val))->modification_date);
	printf("%f\n", ((user*) (userlist->head->next->val))->fee);
	printf("%s\n", ((user*) (userlist->last->val))->username);
	printf("%s\n", ((user*) (userlist->last->val))->password);
	printf("%s\n", ((user*) (userlist->last->val))->registration_date);
	printf("%s\n", ((user*) (userlist->last->val))->modification_date);
	printf("%f\n", ((user*) (userlist->last->val))->fee);

	printf("%s\n", ((mail*) (maillist->head->val))->to);
	printf("%s\n", ((mail*) (maillist->head->val))->from);
	printf("%s\n", ((mail*) (maillist->head->val))->header);
	printf("%s\n", ((mail*) (maillist->head->val))->body);
	printf("%s\n", ((mail*) (maillist->head->val))->attachments);
	printf("%s\n", ((mail*) (maillist->head->next->val))->to);
	printf("%s\n", ((mail*) (maillist->head->next->val))->from);
	printf("%s\n", ((mail*) (maillist->head->next->val))->header);
	printf("%s\n", ((mail*) (maillist->head->next->val))->body);
	printf("%f\n", ((mail*) (maillist->head->next->val))->attachments);

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
	return 0;
}
