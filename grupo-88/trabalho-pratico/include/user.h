#ifndef USERS_H
#define USERS_H

#include "date.h"

// Tipo de dados para representar um utilizador
typedef struct user User;

// Getters para os vários parâmetros de um user
char* getUserId(User* user);
char* getUserName(User* user);
char getSex(User* user);
char* getPassport(User* user);
char* getCountryCode(User* user);
char getAccountStatus(User* user);
DATE* getAccountCreation(User* user);
DATE* getBirthDate(User* user);

// Setters para os vários parâmetros de um user
void setUserId(User *user, char *user_id);
void setUserName(User *user, char *user_name);
void setSex(User *user, char sex);
void setPassport(User *user, char *passport);
void setCountryCode(User *user, char *code);
void setAccountStatus(User *user, char status);
void setAccountCreation(User* user, char *str);
void setBirthDate(User* user, char *str);

// Função para inicializar um utilizador
User *user_init();

// Função para dar free a um utilizador
void freeUser(void *user);

#endif