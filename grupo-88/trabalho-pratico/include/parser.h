#ifndef PARSER_H
#define PARSER_H

#include "statistics.h"
#include "managePassengers.h"

// Função para dar parse aos voos
void parseVoos(const char *filePath, DATA_FLIGHTS *data_flights, DATA_STATISTICS *data_statistics);

// Função para dar parse aos passageiros
void parsePassageiros(const char *filePath, DATA_FLIGHTS *data_flights, DATA_USERS *data_users, DATA_STATISTICS *data_statistics);

// Função para dar parse ás reservas
void parseReservas(const char *filePath, DATA_RESERVATIONS *data_reservations, DATA_USERS *data_users, DATA_STATISTICS *data_statistics);

// Função para dar parse aos utilizadores
void parseUtilizadores(const char *filePath, DATA_USERS *data_users);

// Função para abrir uma diretoria 
void openDirectory(const char *folderPath, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_USERS *data_users, DATA_STATISTICS *data_statistics);

#endif