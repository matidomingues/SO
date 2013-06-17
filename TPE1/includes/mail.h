#ifndef _MAIL_H_
#define _MAIL_H_

#include <time.h>

typedef struct mail {
	char from[15];
	char to[15];
	char header[30];
	char body[100];
	char attachments[30];
	int read;
	time_t senttime;
} mail;

typedef struct _listmail{
	mail* msg;
	struct _listmail* next;
}ListMail;

#endif
