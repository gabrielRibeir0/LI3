#ifndef MANAGEPASSENGERS_H
#define MANAGEPASSENGERS_H

#include "passenger.h"
#include "manageFlights.h"
#include "manageUsers.h"
#include "statistics.h"

// Função para processar um passageiro
void processPassenger(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_STATISTICS *data_statistics, Passenger *passenger, int valid_parameter, int inactive_user, int *header_written, char *original_line);

// Função para validar os parâmetros de um passageiro
int validateAndSetPassengerParameter(Passenger *passenger, char *field, int num_parameter, DATA_FLIGHTS *data_flights, DATA_USERS *data_users);

#endif