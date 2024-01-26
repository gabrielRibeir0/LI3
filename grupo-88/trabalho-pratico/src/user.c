#include "user.h"
#include <stdlib.h>
#include <string.h>

struct user{
    char* id;
    char* name;
    DATE* birth_date;
    char sex;
    char passport[9];
    char country_code[3];
    DATE* account_creation;
    char account_status;
};

//GETTERS
char* getUserId(User* user){
    return strdup(user->id);
}

char* getUserName(User* user){
    return strdup(user->name);
}

char getSex(User* user){
    return user->sex;
}

char* getPassport(User* user){
    return strdup(user->passport);
}

char* getCountryCode(User* user){
    return strdup(user->country_code);
}

char getAccountStatus(User* user){
    return user->account_status;
}

DATE* getAccountCreation(User* user){
    DATE *new_date = createDate(getYear(user->account_creation), getMonth(user->account_creation), getDay(user->account_creation),
                                getHour(user->account_creation), getMinutes(user->account_creation), getSeconds(user->account_creation));
    
    return new_date;
}

DATE* getBirthDate(User* user){
    DATE *new_date = createDate(getYear(user->birth_date), getMonth(user->birth_date), getDay(user->birth_date), 0, 0, 0);
    
    return new_date;
}

//SETTERS
void setUserId(User *user, char *user_id){
    user->id = malloc(sizeof(char) * (strlen(user_id) + 1));
    strcpy(user->id, user_id);
}

void setUserName(User *user, char *user_name){
    user->name = malloc(sizeof(char) * (strlen(user_name) + 1));
    strcpy(user->name, user_name);
}

void setSex(User *user, char sex){
    user->sex = sex;
}

void setPassport(User *user, char *passport){
    strcpy(user->passport, passport);
}

void setCountryCode(User *user, char *code){
    strcpy(user->country_code, code);
}

void setAccountStatus(User *user, char status){
    user->account_status = status;
}

void setAccountCreation(User* user, char *str){
    DATE *new_date = strToDate(str, 1);
    user->account_creation = new_date; 
}

void setBirthDate(User* user, char *str){
    DATE *new_date = strToDate(str, 0);
    user->birth_date = new_date; 
}

//INIT / FREE
User *user_init(){
    User *user = malloc(sizeof(User));

    user->id = NULL;
    user->name = NULL;
    user->birth_date = NULL;
    user->account_creation = NULL;

    return user;
}

void freeUser(void *user_pointer){
    User *user = (User *)user_pointer;
    if(user != NULL) {
        free(user->id);
        free(user->name);
        free(user->birth_date);
        free(user->account_creation);
    }

    free(user);
}