#ifndef INTERPRETER_H
#define INTERPRETER_H

#include "manageUsers.h"
#include "manageFlights.h"
#include "manageReservations.h"
#include "statistics.h"
#include "utility.h"

// Tipo de dados para representar um comando
typedef struct command Cmd;

// Função para contar o número de argumentos de um comando
int get_n_args(char* line);

// Função para fazer parsing de um comando
Cmd* parse_command(char* line);

// Função para executar um comando
void execute_command(Cmd* command, int command_number, DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, PROGRAM_MODE mode);

// Função para dar free a um comando
void free_command(Cmd* cmd);

// Função para processar um comando (parsing e execução)
void process_commands(const char *filepath, DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics);
#endif