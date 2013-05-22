#include "marshalling.h"

void* unmarshall(char* data){
	char* tokens;
	void* msg;
	tokens = strtok(data,";");
	if(strcmp(tokens, "message") == 0){
		msg = malloc(sizeof(Message));
		tokens = strtok(NULL,";");
		strcpy(((Message*)msg)->protocol, tokens);
		tokens = strtok(NULL,";");
		strcpy(((Message*)msg)->method, tokens);
		tokens = strtok(NULL,";");
		strcpy(((Message*)msg)->resource, tokens);
		tokens = strtok(NULL,";");
		((Message*)msg)->time = atoi(tokens);
		tokens = strtok(NULL,";");
		((Message*)msg)->referer = atoi(tokens);
		tokens = strtok(NULL,";");
		if(tokens == NULL){
			strcpy(((Message*)msg)->body, "");
		}else{
			strcpy(((Message*)msg)->body, tokens);
		}
	}else if(strcmp(tokens, "mail") == 0){
		msg = malloc(sizeof(mail));
		tokens = strtok(NULL,";");
		strcpy(((mail*)msg)->from, tokens);
		tokens = strtok(NULL,";");
		strcpy(((mail*)msg)->to, tokens);
		tokens = strtok(NULL,";");
		strcpy(((mail*)msg)->body, tokens);
		tokens = strtok(NULL,";");
		strcpy(((mail*)msg)->attachments, tokens);
		tokens = strtok(NULL,";");
		((mail*)msg)->read = atoi(tokens);
		tokens = strtok(NULL,";");
		((mail*)msg)->senttime = atoi(tokens);
	}
	return (void*)msg;
}

char* marshall(void* msg, size_t length){
	char* data = malloc(150*sizeof(char));
	if(length == sizeof(Message)){
		sprintf(data,"%s;%s;%s;%s;%d;%d;%s", "message", ((Message*)msg)->protocol, 
			((Message*)msg)->method, ((Message*)msg)->resource, (int)((Message*)msg)->time, 
			((Message*)msg)->referer, ((Message*)msg)->body);
	}else if(length == sizeof(mail)){
		sprintf(data,"%s;%s;%s;%s;%s;%d;%d", "mail", ((mail*)msg)->from, ((mail*)msg)->to, 
			((mail*)msg)->body, ((mail*)msg)->attachments, ((mail*)msg)->read, 
			(int)((mail*)msg)->senttime);
	}
	return data;
}