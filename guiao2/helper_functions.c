#include "helper_functions.h"
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>

void printInt(void* i) {
    int* i_ = i;
    printf("%d", *i_);
}

int* randomInt(){
    int* data = malloc(sizeof(int));
    *data = rand() % 100;
    return data;
}

void toUpperStr(char* str){
    int i = 0;
    while(str[i]){
        str[i] = toupper(str[i]);
        i++;
    } 
}