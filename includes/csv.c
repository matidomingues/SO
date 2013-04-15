#include "csv.h"

void initUserList(const char* filename, linked_list* userlist) {
	char line[MAXLINELEN] = { 0x0 };
	int fieldcount = 0;
	char arr[MAXFLDS][MAXFLDLEN] = { { 0x0 } };
	int linecount = 0;

	FILE *fp = fopen(filename, "r");

	if (fp == NULL ) {
		printf("While opening file %s.\n", filename);
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	user* u;

	while (fgets(line, sizeof(line), fp) != 0) /* Read a line, place it in line*/
	{
		//int i = 0;
		linecount++;
		//printf("Record #%d\n", linecount);
		parseline(line, ";", arr, &fieldcount); /* Split line into fields, save fields into arr */

		//for (i = 0; i < fieldcount; i++) { /* Print field */
		//	printf("\tField #%d=%s\n", i, arr[i]);
		//}
		u = (user*) malloc(sizeof(user)); /*reserve space for user*/

		u->username = malloc(sizeof(arr[0]));
		strcpy(u->username, arr[0]);

		u->password = malloc(sizeof(arr[1]));
		strcpy(u->password, arr[1]);

		u->registration_date = (time_t) atoi(arr[2]);
		u->modification_date = (time_t) atoi(arr[3]);
		u->fee = atof(arr[4]);

		linked_list* maillist = createList(NULL );
		initMailList(u->username, maillist);

		u->mail_list = maillist;

		addNode(userlist, u, 1);
	}
	fclose(fp);
}

void initMailList(const char* username, linked_list* maillist) {
	char line[MAXLINELEN] = { 0x0 };
	int fieldcount = 0;
	char arr[MAXFLDS][MAXFLDLEN] = { { 0x0 } };
	int linecount = 0;

	char filename[128];
	strcpy(filename, "csv/mails/");
	strcat(filename, username);
	strcat(filename, ".csv");

	FILE *fp = fopen(filename, "r");

	if (fp == NULL ) {
		printf("While opening file %s.\n", filename);
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	mail* m;

	while (fgets(line, sizeof(line), fp) != 0) /* Read a line, place it in line*/
	{
		//int i = 0;
		linecount++;
		//printf("Record #%d\n", linecount);
		parseline(line, ";", arr, &fieldcount); /* Split line into fields, save fields into arr */

		//for (i = 0; i < fieldcount; i++) { /* Print field */
		//printf("\tField #%d=%s\n", i, arr[i]);
		//}
		m = (mail*) malloc(sizeof(mail)); /*reserve space for mail*/

		m->from = malloc(sizeof(arr[0]));
		strcpy(m->from, arr[0]);

		m->to = malloc(sizeof(arr[1]));
		strcpy(m->to, arr[1]);

		m->header = malloc(sizeof(arr[2]));
		strcpy(m->header, arr[2]);

		m->body = malloc(sizeof(arr[3]));
		strcpy(m->body, arr[3]);

		m->attachments = malloc(sizeof(arr[4]));
		strcpy(m->attachments, arr[4]);

		addNode(maillist, m, 1);
	}
	fclose(fp);
}

void parseline(char *line, const char *delim, char arr[][MAXFLDLEN],
		int *fieldcount) {
	char*p = strtok(line, delim);
	int field = 0;

	while (p) {
		strcpy(arr[field], p);
		field++;
		p = strtok('\0', delim);
	}
	*fieldcount = field;
}

void addUserToCSV(const user* u, const char* filename) {
	FILE *fp = fopen(filename, "a");

	if (fp == NULL ) {
		printf("While opening file %s.\n", filename);
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	fprintf(fp, "%s;%s;%d;%d;%f\n", u->username, u->password,
			(int) u->registration_date, (int) u->modification_date, u->fee);
	fclose(fp);
}

void addMailToUser(char* filename, mail* m) {
	FILE *fp = fopen(filename, "a");

	if (fp == NULL ) {
		printf("While opening file %s.\n", filename);
		perror("File open error");
		exit(EXIT_FAILURE);
	}
	fprintf(fp, "%s;%s;%s;%s;%s", m->from, m->to, m->header, m->body,
			m->attachments); //TODO: Por que aca no hace falta \n???
	fclose(fp);
}

void dumpUsersToCSVFile(linked_list* users) {
	FILE* fp = fopen("csv/users.csv", "w");
	fclose(fp);
	node* current = users->head;
	int i;
	for (i = 0; i < length(users); i++) {
		addUserToCSV(current->val, "csv/users.csv");
		current = current->next;
	}
	fclose(fp);
}

void dumpMailsToCSVFile(linked_list* mails, user* u) {
	char filename[128];
	strcpy(filename, "csv/mails/");
	strcat(filename, u->username);
	strcat(filename, ".csv");

	FILE *fp = fopen(filename, "w");
	fclose(fp);
	node* current = mails->head;
	int i;
	for (i = 0; i < length(mails); i++) {
		addMailToUser(filename, current->val);
		current = current->next;
	}
	fclose(fp);
}
