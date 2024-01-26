#ifndef INTERPRETERTESTES_H
#define INTERPRETERTESTES_H

#include "manageUsers.h"
#include "manageFlights.h"
#include "manageReservations.h"
#include "statistics.h"

typedef struct command Cmd;
int get_n_argsT(char* line);
Cmd* parse_commandT(char* line);

// Função para executar um comando de testes 
void execute_commandT(Cmd* command, int command_number, DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, double *times);

// Função para dar free a um comando de testes
void free_commandT(Cmd* cmd);

// Função para processar os comandos de um teste
void process_commandsT(const char *filepath, DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics);

#endif