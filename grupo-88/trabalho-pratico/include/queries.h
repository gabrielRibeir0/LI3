#ifndef QUERIES_H
#define QUERIES_H
#include <glib.h>
#include "manageFlights.h"
#include "managePassengers.h"
#include "manageReservations.h"
#include "manageUsers.h"
#include "statistics.h"
#include "utility.h"

// Função para listar o resumo de uma reserva
void Query1_reservation (int size, char* id ,int flag, int command_number, DATA_RESERVATIONS *data_reservations);

// Função para listar o resumo de um voo
void Query1_flight (int size, char* id ,int flag, int command_number, DATA_FLIGHTS *data_flights, DATA_STATISTICS *data_statistics);

// Função para listar o resumo de um user
void Query1_users (int size, char* id, int flag, int command_number, DATA_USERS *data_users, DATA_STATISTICS *data_statistics);

// Função para listar o resumo de uma reserva, voo ou user
void Query1(char* id , int flag, int command_number,DATA_RESERVATIONS *data_reservations, DATA_FLIGHTS *data_flights, DATA_USERS *data_users, DATA_STATISTICS *data_statistics);

// Função para listar os voos ou reservas de um utilizador
void Query2(char* user_id, char* type, int flag, int command_number,DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations);

// Função para apresentar a classificação média de um hotel
void Query3(char *hotel_id, int flag, int command_number, DATA_STATISTICS *data_statistics);

// Função para listar as reservas de um hotel
void Query4 (char *hotel_id, int flag, int command_number, DATA_RESERVATIONS *data_reservations);

// Função para listar os voos com origem num dado aeroporto, entre duas datas
void Query5( char* origin, char* begin_date_str, char* end_date_str, int flag, int command_number, DATA_FLIGHTS *data_flights);

// Função para listar o top N aeroportos com mais passageiros, para um dado ano
void Query6 (int year, int N, int flag, int command_number, DATA_STATISTICS *data_statistics);

// Função para listar o top N aeroportos com a maior mediana de atrasos
void Query7 (int N, int flag, int command_number, DATA_STATISTICS *data_statistics, DATA_FLIGHTS *data_flights);

// Função para apresentar a receita total de um hotel entre duas datas
void Query8 (char *reservation_id, char *begin_date_str, char *end_date_srt, int flag, int command_number, DATA_RESERVATIONS * data_reservations);

// Função para listar todos os utilizadores que começam por um prefixo
void Query9(char *prefix, int flag, int command_number, DATA_USERS *data_users);

// Função para apresentar várias métricas gerias da aplicação
void Query10(int year, int month, int flag, int command_number, DATA_STATISTICS *data_statistics);

// Função responsável por identificar qual querie a executar com base no id recebido e preparar os argumentos para passar às queries
void query_manager(DATA_FLIGHTS *data_flights,DATA_RESERVATIONS * data_reservations,DATA_USERS *data_users,DATA_STATISTICS *data_statistics,int query_id, int nargs, char **args, int flag, int command_number, PROGRAM_MODE mode);

#endif 