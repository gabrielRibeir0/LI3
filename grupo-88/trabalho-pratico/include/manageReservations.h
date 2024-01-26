#ifndef MANAGERESERVATIONS_H
#define MANAGERESERVATIONS_H

#include "reservation.h"
#include "manageUsers.h"

// Tipo de dados para conter as várias estruturas de dados relacionadas às reservas
typedef struct data_reservations DATA_RESERVATIONS;

// Tipo de dados que contém uma lista das reservas
typedef struct reservations_list RESERVATIONS_LIST;

// Função para inicializar uma reserva
DATA_RESERVATIONS *data_reservations_init();

// Função para obter uma reserva de uma hash table
Reservation *reservation_hash_lookup(DATA_RESERVATIONS *data, int reservation_id);

// Função para inserir uma reserva na lista de reservas de um hotel
void insertHotelsHash(DATA_RESERVATIONS *data, int *hotel_id, Reservation *reservation);

// Função para inserir o nome de um hotel numa hash table
void insertHotelName(DATA_RESERVATIONS *data, int hotel_id, char *hotel_name);

// Função para obter a próxima reserva numa lista
Reservation *getNextReservationsData(DATA_RESERVATIONS *data_reservations, int *hotel_id);

// Função que procura o nome de um hotel na hash table 
char *getHotelName(DATA_RESERVATIONS *data, int hotel_id);

// Função para dar free ao RESERVATIONS_LIST
void freeHotelList(void *list_pointer);

// Função para dar free ao DATA_RESERVATIONS
void freeReservationsData(DATA_RESERVATIONS *data);

// Função para comprar duas reservas através da Begin_Date, caso seja igual, compara através dos ids
int compare_reservations(gconstpointer reservation1_pointer, gconstpointer reservation2_pointer);

// Função auxiliar para a compare_reservations
int compare_reservations_id(gconstpointer id1, gconstpointer id2, gpointer data_flights);

// Função para processar uma reserva
void processReservation(DATA_RESERVATIONS *data_reservations, DATA_USERS *data_users, DATA_STATISTICS *data_statistics, Reservation *reservation, int valid_parameter, int inactive_user, int *header_written, char *original_line);

// Função para validar os parâmetros de uma reserva
int validateAndSetReservationParameter(Reservation* reservation,char* parameter, int num_parameter, DATA_USERS *data_user, DATA_RESERVATIONS *data_reservations);

#endif