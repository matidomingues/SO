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
#define MAXFLDLEN 30	/* Longest field + 1 = 31 byte field */	//TODO:Estirar esto si no alcanza!
/****************************************************/
/* void parseline()									*/
/* Splits line into fields, as delimited by 'delim'	*/
/****************************************************/
void parseline(char *line, const char *delim, char arr[][MAXFLDLEN],
		int *fieldcount);

/****************************************************/
/* void init_userlist()								*/
/* Initializes user structure					 	*/
/****************************************************/

void initUserList(const char* filename, linked_list* userlist);

/****************************************************/
/* void init_messagelist()								*/
/* Initializes mail structure					 	*/
/****************************************************/

void initMailList(const char* username, linked_list* messagelist);

/****************************************************/
/* void addUserToCsv()								*/
/* Loads single user into users.csv					 	*/
/****************************************************/

void addUserToCSV(const user* u, const char* filename);

/****************************************************/
/* void addMailToUser()								*/
/* Adds user's emails to it's .csv file			 	*/
/****************************************************/

void addMailToUser(char* filename, mail* m);

/****************************************************/
/* void dumpUserstoCSVFile()						*/
/* Dumps all users into users.csv file			 	*/
/****************************************************/

void dumpUsersToCSVFile(linked_list* users);

/****************************************************/
/* void dumpMailsToCSVFile()						*/
/* Dumps all mail into users' .csv file			 	*/
/****************************************************/

void dumpMailsToCSVFile(linked_list* mails, user* u);
