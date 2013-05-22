#ifndef _USER_H_
#define _USER_H_

#include "linkedlist.h"
#include <time.h>

typedef struct user {
	char* username;
	char* password;
	time_t registration_date;
	time_t modification_date;
	float fee;
	linked_list* mail_list;
} user;

#endif
