#ifndef _MAIL_H_
#define _MAIL_H_

typedef struct mail {
	char from[15];
	char to[15];
	char header[30];
	char body[100];
	char attachments[30];	//TODO Check attachment type
	int read;
} mail;

#endif