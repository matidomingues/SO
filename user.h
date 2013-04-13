#ifndef _USER_H_
#define _USER_H_
#endif
#include <time.h>

typedef struct user {
	char* username;
	char* password;
	time_t registration_date;
	time_t modification_date;
	float tarifa;
} user;
