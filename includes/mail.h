#ifndef _MAIL_H_
#define _MAIL_H_
#endif

#include <time.h>

typedef struct mail {
	char* from;
	char* to;
	char* header;
	char* body;
	char* attachments;	//TODO Check attachment type
} mail;
