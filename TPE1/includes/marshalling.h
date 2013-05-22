#ifndef _MARSHALLING_H_
#define _MARSHALLING_H_

#include "mail.h"
#include "message.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void* unmarshall(char* data);
char* marshall(void* msg, size_t length);


#endif