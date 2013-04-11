#ifndef _CSV_H_
#define _CSV_H_
#endif

#include "user.h"
#include "mail.h"
#include "linkedlist.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLINELEN 1024	/* Maximum line length */
#define MAXFLDS 5	/* Maximum number of fields */
#define MAXFLDLEN 30	/* Longest field + 1 = 31 byte field */

/****************************************************/
/* void parseline()									*/
/* Splits line into fields, as delimited by 'delim 	*/
/****************************************************/
void parseline(char *line, const char *delim, char arr[][MAXFLDLEN],
		int *fieldcount);

void init_userlist(const char* filename, struct linked_list* userlist);

void init_messagelist(const char* filename, struct linked_list* messagelist);
