#ifndef MANAGEFLIGHTS_H
#define MANAGEFLIGHTS_H

#include "flight.h"
#include "statistics.h"

// Tipo de dados para conter as várias estruturas de dados relacionadas aos voos
typedef struct data_flights DATA_FLIGHTS;

// Tipo de dados que contém os voos e atrasos de um aeroporto
typedef struct flights_and_delays FLIGHTS_N_DELAYS;

// Função para inicializar um voo
DATA_FLIGHTS *data_flights_init();

// Função para dar free ao DATA_FLIGHTS
void freeFlightsData(DATA_FLIGHTS *data);

// Função para dar free ao FLIGHTS_N_DELAYS
void freeFlightsNDelays(void *list_pointer);

// Função para inserir um voo na lista de voos de um aeroporto na hash table
int insertAirportHash(DATA_FLIGHTS *data_flights, Flight *flight, char *airport_id, int delay);

// Função para obter um voo da hash table
Flight *flight_hash_lookup(DATA_FLIGHTS *data, int flight_id);

// Função para obter um id de um voo da hash table
int *getFlightIdFromHash(DATA_FLIGHTS *data, int flight_id);

// Função para obter o próximo voo numa lista
Flight *getNextAirportFlight(DATA_FLIGHTS *data_flights, char *airport_id);

// Função para calcular a mediana dos atrasos de um aeroporto
int getMedianDelays(DATA_FLIGHTS *data, char *airport);

// Função para verificar se um voo existe na hash table
gboolean flights_hash_contains(DATA_FLIGHTS *data, char* parameter);

// Função para comparar 2 voos, usada na ordenação de listas
int compare_flights(gconstpointer flight1_pointer, gconstpointer flight2_pointer);

// Função para comparar 2 voos representados pelos seus ids, usada na ordenação de listas
int compare_flights_id(gconstpointer id1, gconstpointer id2, gpointer data_flights);

// Função para comparar 2 atrasos
int compareDelays(gconstpointer airport_delay1, gconstpointer airport_delay2);

// Função para processar um voo
void processFlight(DATA_FLIGHTS *data_flights, DATA_STATISTICS *data_statistics, Flight *flight, int valid_parameter, int *header_written, char *original_line);

// Função para validar os parâmetros de um voo
int validateAndSetFlightParameter(Flight *flight, char* parameter, int num_parameter);

#endif