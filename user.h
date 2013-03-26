#ifndef _USER_H_
#define _USER_H_
#endif

typedef struct user {
	int id; 			//Id unico
	char* username;	//Nombre
	char* password;	//Password
	float registration_date;	//Registration Date
	float modification_date;	//Modification Date
	float tarifa;				//??
	struct node* mail_list; 	//Lista de mails
	struct node* friend_list;	//Lista de contactos
} user;
