#ifndef DATE_H
#define DATE_H

// Tipo de dados 'DATE' que contém o ano, mês, dia, hora, minutos e segundos de uma data
typedef struct date DATE;

// Função 'createDate' que inicializa uma nova data
// Recebe os vários parâmetros para a criação da data
DATE *createDate(int year, int month, int day, int hour, int minutes, int seconds);

// Função 'compareDate' que compara 2 datas
// Devolve -1 se a date1 for a mais recente, 0 se forem iguais e 1 se a date2 for a mais recente
int compareDate(DATE *date1, DATE *date2);

// Função 'dateToStr' que formata uma data numa string
// Formatos "ano/mês/dia" ou "ano/mês/dia horas/minutos/segundos", dependendo do caso
char *dateToStr(DATE *date, int has_time);

// Função 'strToDATE' que converte uma string para uma data
// Formatos com horas ou sem horas
DATE *strToDate(char *str, int has_time);

// Função 'dateDifference' que calcula a diferença entre 2 datas em segundos
int dateDifference(DATE *end_date, DATE *begin_date);

// Função 'calculateUserAge' que calcula a idade de um utilizador em anos.
int calculateUserAge(DATE *birth_date);

// Getters para os vários parâmetros de uma data
int getYear(DATE *date);
int getMonth(DATE *date);
int getDay(DATE *date);
int getHour(DATE *date);
int getMinutes(DATE *date);
int getSeconds(DATE *date);

#endif