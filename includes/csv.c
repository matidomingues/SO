#include "csv.h"

void init_userlist(const char* filename, struct linked_list* userlist) {
	char line[MAXLINELEN] = { 0x0 };
	int fieldcount = 0;
	char arr[MAXFLDS][MAXFLDLEN] = { 0x0 };
	int linecount = 0;

	const char* mode = "r"; /*r=read, w=write, b=binary */

	FILE *fp = fopen(filename, mode); /* Open file */

	if (fp == NULL ) {
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	struct user* u;

	while (fgets(line, sizeof(line), fp) != 0) /* Read a line, place it in line*/
	{
		int i = 0;
		linecount++;
		printf("Record #%d\n", linecount);
		parseline(line, ";", arr, &fieldcount); /* Split line into fields, save fields into arr */

		for (i = 0; i < fieldcount; i++) { /* Print field */
			printf("\tField #%d=%s\n", i, arr[i]);
		}
		u = (struct user*) malloc(sizeof(struct user)); /*reserve space for user*/

		u->username = malloc(sizeof(arr[0]));
		strcpy(u->username, arr[0]);

		u->password = malloc(sizeof(arr[1]));
		strcpy(u->password, arr[1]);

		u->registration_date = (int) arr[2]; //TODO Cast to time_t
		u->modification_date = (int) arr[3]; //TODO Cast to time_t
		u->tarifa = atof(arr[4]);

		addnode(userlist, u, 1);
	}
	fclose(fp); /* Close file */
}

void init_messagelist(const char* filename, struct linked_list* messagelist) {
	char line[MAXLINELEN] = { 0x0 };
	int fieldcount = 0;
	char arr[MAXFLDS][MAXFLDLEN] = { 0x0 };
	int linecount = 0;

	const char* mode = "r"; /*r=read, w=write, b=binary */

	FILE *fp = fopen(filename, mode); /* Open file */

	if (fp == NULL ) {
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	struct mail* m;

	while (fgets(line, sizeof(line), fp) != 0) /* Read a line, place it in line*/
	{
		int i = 0;
		linecount++;
		printf("Record #%d\n", linecount);
		parseline(line, ";", arr, &fieldcount); /* Split line into fields, save fields into arr */

		for (i = 0; i < fieldcount; i++) { /* Print field */
			printf("\tField #%d=%s\n", i, arr[i]);
		}
		m = (struct mail*) malloc(sizeof(struct mail)); /*reserve space for mail*/

		m->from = malloc(sizeof(arr[0]));
		strcpy(m->from, arr[0]);

		m->to = malloc(sizeof(arr[1]));
		strcpy(m->to, arr[1]);

		m->header = malloc(sizeof(arr[2]));
		strcpy(m->header, arr[2]);

		m->body = malloc(sizeof(arr[3]));
		strcpy(m->body, arr[3]);

		m->attachments = arr[4];

		addnode(messagelist, m, 1);
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
