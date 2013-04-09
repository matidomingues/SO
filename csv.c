#include "csv.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int interpret() {
	char tmp[MAXLINELEN];
	int fieldcount = 0;
	char arr[MAXFLDS][MAXFLDLEN];
	int linecount = 0;

	const char* filename = "test.csv";
	const char* mode = "r"; /*r=read, w=write, b=binary */

	FILE *fp = fopen(filename, mode); /* Open file */

	if (fp == NULL ) {
		perror("File open error");
		exit(EXIT_FAILURE);
	}

	while (fgets(tmp, sizeof(tmp), fp) != 0) /* Read a line */
	{
		int i = 0;
		linecount++;
		printf("Record #%d\n", linecount);
		parseline(tmp, ";", arr, &fieldcount); /* Split line into fields */

		for (i = 0; i < fieldcount; i++) { /* Print field */
			/****************************************************/
			/*ACAAA!!! en vez de imprimir, hacer lo que queramos*/
			/****************************************************/
			printf("\tField #%d=%s\n", i, arr[i]);
		}
	}

	fclose(fp); /* Close file */
	return 0;
}

void parseline(char *line, char *delim, char arr[][MAXFLDLEN], int *fldcnt) {
	char*p = strtok(line, delim);
	int fld = 0;

	while (p) {
		strcpy(arr[fld], p);
		fld++;
		p = strtok('\0', delim);
	}
	*fldcnt = fld;
}
