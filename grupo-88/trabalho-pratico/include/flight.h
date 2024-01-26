#ifndef FLIGHTS_H
#define FLIGHTS_H

#include "date.h"

// Tipo de dados para representar um voo
typedef struct flight Flight;

// Getters para os vários parâmetros de um voo
int getFlightId(Flight* flight);
char* getAirline(Flight* flight);
char* getPlaneModel(Flight* flight);
char* getOrigin(Flight* flight);
char* getDestination(Flight* flight);
DATE* getScheduleDeparture(Flight* flight);
DATE* getScheduleArrival(Flight* flight);
DATE* getRealDeparture(Flight* flight);
DATE* getRealArrival(Flight* flight);

// Setters para os vários parâmetros de um voo
void setFlightId(Flight* flight, char *id);
void setAirline(Flight* flight, char *id);
void setPlaneModel(Flight* flight, char *id);
void setOrigin(Flight* flight, char *id);
void setDestination(Flight* flight, char *id);
void setScheduleDeparture(Flight *flight, char *str);
void setScheduleArrival(Flight *flight, char *str);
void setRealDeparture(Flight *flight, char *str);
void setRealArrival(Flight *flight, char *str);

// Função para inicializar um voo
Flight *flight_init();

// Função para dar free a um voo
void freeFlight(void *flight);

#endif