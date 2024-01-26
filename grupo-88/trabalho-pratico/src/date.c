#include "date.h"
#include <stdio.h>
#include <stdlib.h>

#define CURRENT_YEAR 2023
#define CURRENT_MONTH 10
#define CURRENT_DAY 1
#define SEC_PER_DAY 86400
#define SEC_PER_HOUR 3600

struct date{
    int year;
    int month;
    int day;
    int hour;
    int minutes;
    int seconds;
};

DATE *createDate(int year, int month, int day, int hour, int minutes, int seconds){
    DATE *new_date = malloc(sizeof(DATE));
    new_date->year = year;
    new_date->month = month;
    new_date->day = day;
    new_date->hour = hour;
    new_date->minutes = minutes;
    new_date->seconds = seconds;

    return new_date;
}

int compareDate(DATE *date1, DATE *date2){
    //Comparar anos
    if (date1->year < date2->year) {
        return -1;
    }
    else if (date1->year > date2->year) {
        return 1;
    }

    //Comparar meses
    if (date1->month < date2->month) {
        return -1;
    }
    else if (date1->month > date2->month) {
        return 1;
    }

    //Comparar dias
    if (date1->day < date2->day) {
        return -1;
    }
    else if (date1->day > date2->day) {
        return 1;
    }

    //Comparar horas
    if (date1->hour < date2->hour) {
        return -1;
    }
    else if (date1->hour > date2->hour) {
        return 1;
    }

    //Comparar minutos
    if (date1->minutes < date2->minutes) {
        return -1;
    }
    else if (date1->minutes > date2->minutes) {
        return 1;
    }

    //Comparar segundos
    if (date1->seconds < date2->seconds) {
        return -1;
    }
    else if (date1->seconds > date2->seconds) {
        return 1;
    }

    return 0;
}

char *dateToStr(DATE *date, int has_time){
    char *date_str;
    //decide em qual formato é para criar a string
    if(has_time) {
        date_str = malloc(sizeof(char) * 20);
        sprintf(date_str, "%d/%02d/%02d %02d:%02d:%02d", date->year, date->month, date->day, date->hour, date->minutes, date->seconds);
    }
    else{
        date_str = malloc(sizeof(char) * 11);
        sprintf(date_str, "%d/%02d/%02d", date->year, date->month, date->day);
    }

    return date_str;
}

DATE *strToDate(char *str, int has_time){
    if(str == NULL) {
        return NULL;
    }

    DATE *new_date;
    int year, month, day;
    //decide em qual formato é para criar a data
    if(has_time){
        int hour, minutes, seconds;
        sscanf(str, "%d/%d/%d %d:%d:%d", &year, &month, &day, &hour, &minutes, &seconds);
        new_date = createDate(year, month, day, hour, minutes, seconds);
    }
    else{
        sscanf(str, "%d/%d/%d", &year, &month, &day);
        new_date = createDate(year, month, day, 0, 0, 0);
    }

    return new_date;
}

int dateDifference(DATE *end_date, DATE *begin_date){
    return (getDay(end_date) - getDay(begin_date)) * SEC_PER_DAY + (getHour(end_date) - getHour(begin_date)) * SEC_PER_HOUR +
           (getMinutes(end_date) - getMinutes(begin_date)) * 60 + getSeconds(end_date) - getSeconds(begin_date);
}

int calculateUserAge(DATE *birth_date) {
    int years = CURRENT_YEAR - birth_date->year;

    //se ainda não passamos o dia de aniversário do utilizador, retirar 1 ano à idade
    if ((CURRENT_MONTH < birth_date->month) ||
        (CURRENT_MONTH == birth_date->month && CURRENT_DAY < birth_date->day)) {
        years--;
    }

    return years;
}

//GETTERS
int getYear(DATE *date){
    return date->year;
}

int getMonth(DATE *date){
    return date->month;
}

int getDay(DATE *date){
    return date->day;
}

int getHour(DATE *date){
    return date->hour;
}

int getMinutes(DATE *date){
    return date->minutes;
}

int getSeconds(DATE *date){
    return date->seconds;
}