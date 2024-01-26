#ifndef STATISTICS_H
#define STATISTICS_H

#include <glib.h>
#include "date.h"

// Estrutura de dados para dar update às estatísticas de um utilizador, voo, passageiro e reserva
typedef enum stat_update{
    USER,
    FLIGHT,
    PASSENGER,
    RESERVATION
}STAT_TO_UPDATE;

// Tipo de dados para conter as várias estruturas de dados relacionadas às estatísticas
typedef struct data_statistics DATA_STATISTICS;

// Tipo de dados que contém as estatísticas de um utilizador
typedef struct user_stats USER_STATS;

// Tipo de dados que contém as estatísticas de um aeroporto (passageiros)
typedef struct airport_stats AIRPORT_STATS;

// Tipo de dados que contém as estatísticas de um hotel
typedef struct hotel_stats HOTEL_STATS;

// Tipo de dados que contém as estatísticas de um aeroporto (medianas)
typedef struct airport_median AIRPORT_MEDIAN;

// Tipo de dados que contém as estatísticas de um ano
typedef struct year_stats YEAR_STATS;

// Tipo de dados que contém as estatísticas de um mês
typedef struct month_stats MONTH_STATS;

// Tipo de dados que contém as estatísticas de um dia
typedef struct day_stats DAY_STATS;

// Função para inicializar uma estatística
DATA_STATISTICS *statistics_init();

// Função para inicializar as estatísicas de um dia
DAY_STATS *initDayStats();

// Função para inicializar as estatísticas de um mês
MONTH_STATS *initMonthStats(int month);

// Função para dar free às estatísticas do aeroporto
void freeAirportStats(gpointer airport_stats_pointer);

// Função para dar free às estatísticas do ano
void freeYearStats(void *year_stats_pointer);

// Função para dar free às listas e hash tables das estatísticas
void freeStatistics(DATA_STATISTICS *data);

// Função para aumentar o número de passageiros em 1 unidade
void incNPassengers(DATA_STATISTICS *data, int *flight_id);

// Função para aumentar as estatísticas de um utilizador (voos e reservas)
int incUserStats(DATA_STATISTICS *data, char *key, STAT_TO_UPDATE stat, double value);

// Função para aumentar as estatísticas de um hotel (reservas e rating)
void incHotelStats(DATA_STATISTICS *data, int *key, int rating);

// Função para aumentar o número de passageitos de um aeroporto num dado ano
void incAirportStats(DATA_STATISTICS *data, char *origin, char *destination, int year);

// Função para aumentar/adicionar as estatísticas de um ano
int incYearStats(DATA_STATISTICS *data_statistics, DATE *date, STAT_TO_UPDATE stat, char *passenger_id);

// Função para adicionar o id de um aeroporto no inicio de uma lista de ids de aeroportos 
void prependAirportId(DATA_STATISTICS *data, char *airport);

// Função para obter as estatísticas de um voo ou utilizador
double getStatisticsValue(DATA_STATISTICS *data, char hash_table, char stat, void *key);

// Função para obter um rating de um hotel de uma hash table de estatísticas de um hotel
double getHotelRating(DATA_STATISTICS *data, int *hotel_id);

// Função para obter as próximas estatísticas de um aeroporto
AIRPORT_STATS *getNextAirportStats(DATA_STATISTICS *data_statistics, int year);

// Função para dar reset ao iterador do nº passageiros de uma hash de estatísticas
void resetAirportStatsIter(DATA_STATISTICS *data_statistics, int year);

// Função para obter o nome de um aeroporto
char *getAirportName(AIRPORT_STATS *stats);

// Função para obter o número de passageiros 
int getNPassengers(AIRPORT_STATS *stats);

// Função para obter o próximo ano de uma hash table de estatísticas 
int getYearStats(DATA_STATISTICS *data, int year, int month, int day, int *users, int *flights, int *passengers, int *unique_passengers, int *reservations);

// Função para obter o próximo ano de uma lista de anos
int getNextYear(DATA_STATISTICS *data);

// Função para resetar um iterador de uma mediana 
void resetMedianIter(DATA_STATISTICS *data_statistics);

// Função para obter a próxima mediana de uma lista de aeroportos 
AIRPORT_MEDIAN *getNextMedian(DATA_STATISTICS *data_statistics, void *data_flights);

// Função para obter a mediana de um aeroporto (string)
char *getAirportNameMedian(AIRPORT_MEDIAN *airport_median);

// Função para obter a mediana de um aeroporto 
int getMedian(AIRPORT_MEDIAN *airport_median);

// Função para comparar duas medianas de aeroportos
int compareMedians(gconstpointer median1_pointer, gconstpointer median2_pointer);

// Função para comparar dois anos 
int compareYears(gconstpointer year1_pointer, gconstpointer year2_pointer);

// Função para comparar aeroportos pelo nº passageiros, caso seja igual compara pelo nome
int compareAirportsByPassengers(gconstpointer airport1_pointer, gconstpointer airport2_pointer);

#endif