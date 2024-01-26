#ifndef MANAGEUSERS_H
#define MANAGEUSERS_H

#include "user.h"
#include "manageFlights.h"
#include "statistics.h"
#include "reservation.h"

// Tipo de dados para conter as várias estruturas de dados relacionadas aos users
typedef struct data_users DATA_USERS;

// Tipo de dados que contém uma lista de reservas e voos de users 
typedef struct user_booking_lists BOOKING_LISTS;

//Tipo de dados que contém uma lista de users 
typedef struct user_list USER_LIST;

// Função para incializar um user
DATA_USERS *data_users_init ();

// Função para dar free ao DATA_USERS
void freeUsersData(DATA_USERS *data_users);

// Função para dar free ao BOOKING_LISTS
void freeBooking(void *list_pointer);

// Função para dar free ao USER_LIST
void freeUserList(void *list_pointer);

// Função para inserir uma reserva numa hash table de bookings
int insertReservationBookingHash(DATA_USERS *data, char *user_id, Reservation *reservation);

// Função para inserir um voo numa hash table de bookings
int insertFlightBookingHash(DATA_USERS *data, char *user_id, Flight *flight);

// Função para inserir um nome numa lista de hash tables (a chave é a primeira letra do nome)
void insertNameHash(DATA_USERS *data, char *user_name, User *user);

// Função para obter um user de uma hash table
User *user_hash_lookup(DATA_USERS *data, char* user_id);

// Função para obter um id de uma hash table
char *getUserIdFromHash(DATA_USERS *data, char *user_id);

// Função para obter a próxima reserva de um utilizador de uma lista de bookings
Reservation *getNextUserReservation(DATA_USERS *data_users, void *data_reservations, char *user_id);

// Função para obter o próximo voo de um utilizador de uma lista de bookings
Flight *getNextUserFlight(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, char *user_id);

// Função para obter o próximo user de uma lista de hash tables (a chave é a primeira letra do nome)
User *getNextUser(DATA_USERS *data_users, char first_letter);

// Função auxiliar para a getNextUser (dá reset ao iterador)
void resetUserIter(DATA_USERS *data_users, char first_letter);

// Função para comparar dois nomes de utilizadores (caso sejam iguais, compara pelo id)
int compare_user_names(gconstpointer id1, gconstpointer id2);

// Função para verificar se um user existe na hash table
gboolean users_hash_contains(DATA_USERS *data, char* user_id);

// Função para verificar se um user inativo existe na hash table
gboolean inactive_users_hash_contains(DATA_USERS *data, char* user_id);

// Função para processar um user
void processUser(DATA_USERS *data_users, DATA_STATISTICS *data_statistics, User *user, int valid_parameter, int *header_written, char *original_line);

// Função para validar os parâmetros de um user
int validateAndSetUserParameter(User* user,char* parameter, int num_parameter);

#endif