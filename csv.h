#ifndef _CSV_H_
#define _CSV_H_
#endif

#define MAXLINELEN 1024	/* Maximum line length */
#define MAXFLDS 20		/* Maximum number of fields */
#define MAXFLDLEN 24	/* Longest field + 1 = 31 byte field */

/****************************************************/
/* void parseline()										*/
/* Splits line into fields, as delimited by 'delim 	*/
/****************************************************/
void parseline(char *line, char *delim, char arr[][MAXFLDLEN], int *fldcnt);


int interpret();
