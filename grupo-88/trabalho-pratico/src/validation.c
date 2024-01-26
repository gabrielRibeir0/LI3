#include "utility.h"
#include "validation.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

int checkFieldsLen(char *field, char *delims, int *lengths, int n_fields) {
    char *start_field = field;
    char *chr_pos;

    //procura a próxima ocorrência do delimitador seguinte e verifica se o comprimento está correto
    for(int i = 0; i < n_fields; i++) {
        chr_pos = strchr(start_field, delims[i]);
        if(chr_pos - start_field != lengths[i]) {
            return 0;
        }
        start_field = chr_pos + 1;
    }

    return 1;
}

//DATES
int validateDate(char *field, int has_time){
    int year, month, day, read, expected_read;

    if(has_time){
        int lengths[6] = {4,2,2,2,2,2};
        int valid_lengths = checkFieldsLen(field, "// ::", lengths, 6);
        if(!valid_lengths){
            return 0;
        }

        expected_read = 6;
        int hour, minute, seconds;
        read = sscanf(field, "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minute, &seconds);
        return (read == expected_read && year >= 0 && year <= 9999 && month >= 1 && month <= 12 && day >= 1 && day <= 32 &&
                hour >= 0 && hour <= 23 && minute >= 0 && minute <= 59 && seconds >= 0 && seconds <= 59);
    }
    else{
        int lengths[6] = {4,2,2};
        int valid_lengths = checkFieldsLen(field, "//", lengths, 3);
        if(!valid_lengths){
            return 0;
        }

        expected_read = 3;
        read = sscanf(field, "%d/%d/%d", &year, &month, &day);
        return (read == expected_read && year >= 0 && year <= 9999 && month >= 1 && month <= 12 && day >= 1 && day <= 32);
    }
}

//RESERVATIONS
int validatePositiveLen(char *field){ //serve para várias entidades
    return (strlen(field) > 0);
}

//hotel stars
int validateInt1to5(char *field){
    char *temp;
    int value = (int) strtol(field, &temp, 10);

    if(temp == field || *temp != '\0')
        return 0;

    if(value < 1 || value > 5)
        return 0;

    return 1;
}

//hotel stars
int validateRating(char *field){
    if(strcmp(field, "") == 0){
        return 1;
    }
    else{
        return(validateInt1to5(field));
    }
}
//>= 0
int validatePosInt(char *field){
    char *temp;
    int value = (int) strtol(field, &temp, 10);

    if(temp == field || *temp != '\0')
        return 0;

    if(value < 0)
        return 0;

    return 1;
}
// includes_breakfast
int validateInclBreakfast(char *field, char *inc_breakfast){
    if(strcasecmp(field, "false") == 0 || strcasecmp(field, "f") == 0 || strcmp(field, "0") == 0 || strcmp(field, "") == 0){
        (*inc_breakfast) = 'F';
        return 1;
    }
    else if(strcasecmp(field, "true") == 0 || strcasecmp(field, "t") == 0 || strcmp(field, "1") == 0){
        (*inc_breakfast) = 'T';
        return 1;
    }
    else{
        return 0;
    }
}

//FLIGHTS
int validateAirport(char *field){
    return ((strlen(field) == 3) && str_isAlpha(field, 3));
}

//USERS
int validateEmail(char *field){
    //verifica se username < 1
    char *start_field;
    char *chr_pos = strchr(field, '@');

    if(chr_pos - field < 1){
        return 0;
    }

    //verifica se domain < 1
    start_field = chr_pos + 1;
    chr_pos = strchr(start_field, '.');
    if(chr_pos - start_field < 1){
        return 0;
    }

    //verifica se TLD < 2
    start_field = chr_pos + 1;
    chr_pos = strchr(start_field, '\0');
    if(chr_pos - start_field < 2){
        return 0;
    }

    return 1;
}

int validateCountryCode(char *field){
    return((strlen(field) == 2) && str_isAlpha(field, 2));
}

int validateAccountStatus(char *field, char *status){
    if(strcasecmp(field, "active\n") == 0){
        (*status) = 'A';
        return 1;
    }
    else if(strcasecmp(field, "inactive\n") == 0){
        (*status) = 'I';
        return 1;
    }
    else{
        return 0;
    }
}
