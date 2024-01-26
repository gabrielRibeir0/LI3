#ifndef PASSENGERS_H
#define PASSENGERS_H

// Tipo de dados para representar um passageiro
typedef struct passenger Passenger;

// Getters para os parâmetros de um passageiro
int getPassengerFlightId(Passenger* passenger);
char* getPassengerId(Passenger* passenger);

// Setters para os parâmetros de um passageiro
void setPassengerFlightId(Passenger *passenger, char *id);
void setPassengerId(Passenger *passenger, char *id);

// Função para inicializar um passageiro
Passenger *init_passenger();

// Função para dar free a um passageiro
void freePassenger(void *passenger_pointer);

#endif