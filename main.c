#include "includes\csv.h"
#include "includes\linkedlist.h"

int main() {

	/****************************************************/
	/*User initialization tests							*/
	/****************************************************/
	/*
	 linked_list* userlist = createList(NULL );
	 init_userlist("csv/users.csv", userlist);
	 printf("%s\n", ((user*) (userlist->head->val))->username);
	 printf("%s\n", ((user*) (userlist->head->val))->password);
	 printf("from: %s\n", ((mail*)((((user*) (userlist->head->val))->mail_list)->head->val))->from);
	 //printf("%s\n", ((user*) (userlist->head->val))->registration_date);
	 //printf("%s\n", ((user*) (userlist->head->val))->modification_date);
	 printf("%f\n", ((user*) (userlist->head->val))->fee);
	 printf("%s\n", ((user*) (userlist->head->next->val))->username);
	 printf("%s\n", ((user*) (userlist->head->next->val))->password);
	 //printf("%s\n", ((user*) (userlist->head->next->val))->registration_date);
	 //printf("%s\n", ((user*) (userlist->head->next->val))->modification_date);
	 printf("%f\n", ((user*) (userlist->head->next->val))->fee);
	 printf("%s\n", ((user*) (userlist->last->val))->username);
	 printf("%s\n", ((user*) (userlist->last->val))->password);
	 //printf("%s\n", ((user*) (userlist->last->val))->registration_date);
	 //printf("%s\n", ((user*) (userlist->last->val))->modification_date);
	 printf("%f\n", ((user*) (userlist->last->val))->fee);
	 */

	/****************************************************/
	/* File Handling Tests								*/
	/****************************************************/

	linked_list* userlist = createList(NULL );
	initUserList("csv/users.csv", userlist);

	//creo usuario
	user* u = (user*) malloc(sizeof(user));
	u->username = "username2";
	u->password = "password";
	u->registration_date = (time_t) 1365535381;
	u->modification_date = (time_t) 1365535381;
	u->fee = 2;

	//Agrego el usuario a users
	addNode(userlist, u, 1);

	//lo agrego al csv
	addUserToCSV(u, "csv/users.csv");

	dumpUsersToCSVFile(userlist);

	dumpMailsToCSVFile(((user*) (userlist->head->val))->mail_list,
			userlist->head->val);

	/*mail* m = (mail*) malloc(sizeof(mail));
	 m->from = "User1";
	 m->to = "User2";
	 m->header = "Title";
	 m->body = "Body";
	 m->attachments = "";
	 addMailToUser(u, m);
	 */

	/****************************************************/
	/* linked list tests								*/
	/****************************************************/
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
	return 0;
}
