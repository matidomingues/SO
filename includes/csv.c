#include "csv.h"

void init_userlist(const char* filename, linked_list* userlist) {
	char line[MAXLINELEN] = { 0x0 };
	int fieldcount = 0;
	char arr[MAXFLDS][MAXFLDLEN] = { { 0x0 } };
	int linecount = 0;

	const char* mode = "r"; /*r=read */

	FILE *fp = fopen(filename, mode); /* Open file */

	if (fp == NULL ) {
		printf("While opening file %s.\n", filename);
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	struct user* u;

	while (fgets(line, sizeof(line), fp) != 0) /* Read a line, place it in line*/
	{
		int i = 0;
		linecount++;
		//printf("Record #%d\n", linecount);
		parseline(line, ";", arr, &fieldcount); /* Split line into fields, save fields into arr */

		for (i = 0; i < fieldcount; i++) { /* Print field */
			//printf("\tField #%d=%s\n", i, arr[i]);
		}
		u = (struct user*) malloc(sizeof(struct user)); /*reserve space for user*/

		u->username = malloc(sizeof(arr[0]));
		strcpy(u->username, arr[0]);

		u->password = malloc(sizeof(arr[1]));
		strcpy(u->password, arr[1]);

		u->registration_date = (time_t) atoi(arr[2]);
		u->modification_date = (time_t) atoi(arr[3]);
		u->fee = atof(arr[4]);

		linked_list* maillist = createList(NULL );
		init_messagelist(u->username, maillist);

		u->mail_list = maillist;

		addNode(userlist, u, 1);
	}
	fclose(fp); /* Close file */
}

void init_messagelist(const char* username, linked_list* messagelist) {
	char line[MAXLINELEN] = { 0x0 };
	int fieldcount = 0;
	char arr[MAXFLDS][MAXFLDLEN] = { { 0x0 } };
	int linecount = 0;

	const char* mode = "r"; /*r=read */

	char filename[256];
	strcpy(filename, "csv/mails/");
	strcat(filename, username);
	strcat(filename, ".csv");

	FILE *fp = fopen(filename, mode); /* Open file */

	if (fp == NULL ) {
		printf("While opening file %s.\n", filename);
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	struct mail* m;

	while (fgets(line, sizeof(line), fp) != 0) /* Read a line, place it in line*/
	{
		int i = 0;
		linecount++;
		//printf("Record #%d\n", linecount);
		parseline(line, ";", arr, &fieldcount); /* Split line into fields, save fields into arr */

		for (i = 0; i < fieldcount; i++) { /* Print field */
			//printf("\tField #%d=%s\n", i, arr[i]);
		}
		m = (struct mail*) malloc(sizeof(struct mail)); /*reserve space for mail*/

		strcpy(m->from, arr[0]);

		strcpy(m->to, arr[1]);

		strcpy(m->header, arr[2]);

		strcpy(m->body, arr[3]);

		strcpy(m->attachments, arr[4]); //TODO: Si es un path, cambiar el tipo a char* y hacer strcpy

		addNode(messagelist, m, 1);
	}
	fclose(fp); /* Close file */
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

void addUserToCsv(user* u, char* filename) {

	const char* mode = "a"; /*a=append */

	FILE *fp = fopen(filename, mode); /* Open file */

	if (fp == NULL ) {
		printf("While opening file %s.\n", filename);
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	fprintf(fp, "\n%s;%s;%d;%d;%f", u->name, u->password, (int)u->registration_date,
			(int)u->modification_date, u->fee);
	fclose(fp);
}
