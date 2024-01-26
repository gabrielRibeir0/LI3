#ifndef INTERACTIVE_H
#define INTERACTIVE_H

#include "manageUsers.h"
#include "manageFlights.h"
#include "manageReservations.h"
#include "statistics.h"

//função para processar um clique no backspace
void backspace();

//menu que pergunta se o utilizador que fazer mais uma query ou não
void continue_menu(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, char *caminho);

//responsavel pelo terceiro meno, onde é requirido o comando
void third_menu(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, char *caminho, int n_Query);

//responsavel pelo menu das intruções face ás especifidades das queries
void instructions_menu (DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, char *caminho);

//responsavel pelo segundo menu, dando ao utilizador a possibilidade de escolher uma das queries para realizar
void second_menu(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, char *caminho);

//responsavel pelo primerio menu, onde é requirido o caminho do dataset
void initInteractiveMode(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics);

#endif