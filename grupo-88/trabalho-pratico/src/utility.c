#include "utility.h"
#include <ctype.h>
#include <stdio.h>

void str_upper(char *str){
    if (str == NULL) { // se a string for NULL indica o erro
        fputs("Error: NULL string on 'str_upper'\n", stderr);
        return;
    }

    for(int i = 0; str[i]; i++){ // percorre a string e põe os seus caracteres em maiúscula
        str[i] = toupper(str[i]);
    }
}

int str_isAlpha(char *str, int len){
    for(int i = 0; i < len; i++){ // percorre a string
        if(isalpha(str[i]) == 0){ // se houver um caracter que não é uma letra, interrompe o ciclo e devolve 0
            return 0;
        }
    }

    return 1;
}