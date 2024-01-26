#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "manageUsers.h"
#include "validation.h"
#include "user.h"
#include "manageReservations.h"
#include "manageFlights.h"
#include "statistics.h"
#include "output.h"
#include <glib.h>
#include <locale.h>
#include <stdio.h>

struct user_booking_lists{
    GSList *flights;
    GSList *reservations;
    int is_sorted_flights;
    int is_sorted_reservations;
    GSList *user_flights_iter;
    GSList *user_reservations_iter;
};

struct user_list{
    GSList *users;
    int is_sorted_user;
    GSList *user_iter;
};

struct data_users{
    GHashTable *users_ids_hash;
    GHashTable *users_hash;
    GHashTable *inactive_users_hash;
    GHashTable *users_booking_hash;
    GHashTable *users_name_hash;
};

DATA_USERS *data_users_init (){
    DATA_USERS *data = malloc(sizeof(DATA_USERS));
    data->users_ids_hash = g_hash_table_new_full(g_str_hash, g_str_equal, free, NULL);
    data->users_hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeUser);
    data->inactive_users_hash = g_hash_table_new(g_str_hash, g_str_equal);
    data->users_booking_hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, freeBooking);
    data->users_name_hash = g_hash_table_new_full(g_int_hash, g_int_equal, free, freeUserList);
    return data;
}

//função que dá free aos dados da hash table dos utilizadores
void freeUsersData(DATA_USERS *data){
    g_hash_table_destroy(data->users_hash);
    g_hash_table_destroy(data->inactive_users_hash);
    g_hash_table_destroy(data->users_booking_hash);
    g_hash_table_destroy(data->users_ids_hash);
    g_hash_table_destroy(data->users_name_hash);
    free(data);
}

void freeBooking(void *list_pointer){
    BOOKING_LISTS *booking = (BOOKING_LISTS *)list_pointer;
    g_slist_free(g_steal_pointer(&(booking->flights)));
    g_slist_free(g_steal_pointer(&(booking->reservations)));

    free(booking);
}

void freeUserList(void *list_pointer){
    USER_LIST *user_list = (USER_LIST *)list_pointer;
    g_slist_free(g_steal_pointer(&(user_list->users)));
    free(user_list);
}

int insertReservationBookingHash(DATA_USERS *data, char *user_id, Reservation *reservation){
    BOOKING_LISTS *booking = g_hash_table_lookup(data->users_booking_hash, user_id);

    if (booking == NULL) {
        // O utilizador não tem nada guardado, criar uma nova entrada
        BOOKING_LISTS *new_booking = malloc(sizeof(BOOKING_LISTS));
        new_booking->flights = NULL;
        new_booking->reservations = NULL;
        new_booking->is_sorted_flights = 0;
        new_booking->is_sorted_reservations = 0;
        new_booking->user_flights_iter = NULL;
        new_booking->user_reservations_iter = NULL;
        new_booking->reservations = g_slist_prepend(new_booking->reservations, reservation);

        g_hash_table_insert(data->users_booking_hash, user_id, new_booking);
        return 0;
    }
    else {
        booking->reservations = g_slist_prepend(booking->reservations, reservation);
        return 1;
    }
}

int insertFlightBookingHash(DATA_USERS *data, char *user_id, Flight *flight){
    BOOKING_LISTS *booking = g_hash_table_lookup(data->users_booking_hash, user_id);

    if (booking == NULL) {
        // O utilizador não tem nada guardado, criar uma nova entrada
        BOOKING_LISTS *new_booking = malloc(sizeof(BOOKING_LISTS));
        new_booking->flights = NULL;
        new_booking->reservations = NULL;
        new_booking->is_sorted_flights = 0;
        new_booking->is_sorted_reservations = 0;
        new_booking->user_flights_iter = NULL;
        new_booking->user_reservations_iter = NULL;

        new_booking->flights = g_slist_prepend(new_booking->flights, flight);

        g_hash_table_insert(data->users_booking_hash, user_id, new_booking);
        return 0;
    }
    else {
        booking->flights= g_slist_prepend(booking->flights, flight);
        return 1;
    }
}

void insertNameHash(DATA_USERS *data, char *user_name, User *user){
    int *first_letter = malloc(sizeof(int));
    *first_letter = (int)user_name[0];
    
    USER_LIST *user_list = g_hash_table_lookup(data->users_name_hash, first_letter);

    if (user_list == NULL) {
        // O utilizador não tem nada guardado, criar uma nova entrada
        USER_LIST *new_user_list = malloc(sizeof(USER_LIST));
        new_user_list->users = NULL;
        new_user_list->is_sorted_user = 0;
        new_user_list->user_iter = NULL;

        new_user_list->users= g_slist_prepend(new_user_list->users, user);

        g_hash_table_insert(data->users_name_hash, first_letter, new_user_list);
    }
    else {
        user_list->users= g_slist_prepend(user_list->users, user);
        free(first_letter);
    }
}

//função que pesquisa na hash table dado parametro
User *user_hash_lookup(DATA_USERS *data, char* user_id){
    return (g_hash_table_lookup(data->users_hash, user_id));
}

char *getUserIdFromHash(DATA_USERS *data, char *user_id){
    char *id = g_hash_table_lookup(data->users_ids_hash, user_id);
    return id;
}

Reservation *getNextUserReservation(DATA_USERS *data_users, void *data_reservations, char *user_id){
    BOOKING_LISTS *booking = g_hash_table_lookup(data_users->users_booking_hash, user_id);
    if(booking == NULL){
        return NULL;
    }

    if(booking->is_sorted_reservations == 0){
        booking->reservations = g_slist_sort_with_data(booking->reservations, compare_reservations_id, data_reservations);
        booking->is_sorted_reservations = 1;
    }

    Reservation *reservation;
    if(booking->user_reservations_iter == NULL){
        booking->user_reservations_iter = booking->reservations;
        reservation = (Reservation *)booking->user_reservations_iter->data;
    }
    else{
        booking->user_reservations_iter = booking->user_reservations_iter->next;
        if(booking->user_reservations_iter == NULL){
            reservation = NULL;
        }
        else{
            reservation = (Reservation *)booking->user_reservations_iter->data;
        }
    }

    return reservation;
}

Flight *getNextUserFlight(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, char *user_id){
    BOOKING_LISTS *booking = g_hash_table_lookup(data_users->users_booking_hash, user_id);
    if(booking == NULL){
        return NULL;
    }

    if(booking->is_sorted_flights == 0){
        booking->flights = g_slist_sort_with_data(booking->flights, compare_flights_id, data_flights);
        booking->is_sorted_flights = 1;
    }

    Flight *flight;
    if(booking->user_flights_iter == NULL){
        booking->user_flights_iter = booking->flights;
        flight = (Flight *)booking->user_flights_iter->data;
    }
    else{
        booking->user_flights_iter = booking->user_flights_iter->next;
        if(booking->user_flights_iter == NULL){
            flight = NULL;
        }
        else{
            flight = (Flight *)booking->user_flights_iter->data;
        }
    }

    return flight;
}

User *getNextUser(DATA_USERS *data_users, char first_letter){
    int first_letter_int = (int)first_letter;
    USER_LIST *user_list = g_hash_table_lookup(data_users->users_name_hash, &first_letter_int);
    if(user_list == NULL){
        return NULL;
    }

    if(user_list->is_sorted_user == 0){
        user_list->users = g_slist_sort(user_list->users,compare_user_names);
        user_list->is_sorted_user = 1;
    }

    User *user;
    if(user_list->user_iter == NULL){
        user_list->user_iter = user_list->users;
        user = (User *)user_list->user_iter->data;
    }
    else{
        user_list->user_iter = user_list->user_iter->next;
        if(user_list->user_iter == NULL){
            user = NULL;
        }
        else{
            user = (User *)user_list->user_iter->data;
        }
    }
    
    return user;
}

void resetUserIter(DATA_USERS *data_users, char first_letter){
    int first_letter_int = (int)first_letter;
    USER_LIST *user_list = g_hash_table_lookup(data_users->users_name_hash, &first_letter_int);
    if(user_list != NULL){
        user_list->user_iter = NULL;
    }
}

//função que verifica se dado parametro se encontra na hash table
gboolean users_hash_contains(DATA_USERS *data, char* user_id){
    return g_hash_table_contains(data->users_hash, user_id);
}

gboolean inactive_users_hash_contains(DATA_USERS *data, char* user_id){
    return g_hash_table_contains(data->inactive_users_hash, user_id);
}

int compare_user_names(gconstpointer user_pointer1, gconstpointer user_pointer2){
    User *user1 = (User*)user_pointer1;
    User *user2 = (User*)user_pointer2;

    char *name1 = getUserName(user1);
    char *name2 = getUserName(user2);
    int name_compare = strcoll(name1, name2);

    free(name1);
    free(name2);
    if(name_compare == 0){
        char *user_id1 = getUserId(user1);
        char *user_id2 = getUserId(user2);
        int compare_ids = strcoll(user_id1, user_id2);
        free(user_id1);
        free(user_id2);
        return compare_ids;
    }
    else{
        return name_compare;
    }
}

void processUser(DATA_USERS *data_users, DATA_STATISTICS *data_statistics, User *user, int valid_parameter, int *header_written, char *original_line){
    if(valid_parameter){
        char status = getAccountStatus(user);

        if(status == 'A') {
            char *user_id = getUserId(user);
            char *user_name = getUserName(user);
            DATE *acc_creation = getAccountCreation(user);
            g_hash_table_add(data_users->users_ids_hash, user_id);
            g_hash_table_insert (data_users->users_hash, user_id, user);
            incYearStats(data_statistics, acc_creation, USER, NULL);
            insertNameHash(data_users, user_name, user);
            free(acc_creation);
            free(user_name);
        }
        else {
            char *user_id = getUserId(user);
            DATE *acc_creation = getAccountCreation(user);
            g_hash_table_add(data_users->users_ids_hash, user_id);
            g_hash_table_add(data_users->inactive_users_hash, user_id);
            incYearStats(data_statistics, acc_creation, USER, NULL);
            freeUser(user);
            free(acc_creation);
        }
    }
    else{
        int ret;
        ret = addToErrorsFile("./Resultados/users_errors.csv", "id;name;email;phone_number;birth_date;sex;passport;country_code;address;account_creation;pay_method;account_status\n" ,
                              original_line, *header_written);
        if(ret == 1) {
            fputs("Couldn't add the line to 'users_errors.csv'.", stderr);
            return;
        }
        if(!(*header_written)){
            *header_written = 1;
        }
        freeUser(user);
    }
}


//função para validar parametros de um dado utilizador
int validateAndSetUserParameter(User* user,char* parameter, int num_parameter){
    int valid = 0;

    switch(num_parameter){
        case 1:
            //userid
            valid = validatePositiveLen(parameter);
            if(valid) {
                setUserId(user, parameter);
            }
            break;
        case 2:
            //name
            valid = validatePositiveLen(parameter);
            if(valid) {
                setUserName(user, parameter);
            }
            break;
        case 3:
            //email
            valid = validateEmail(parameter);
            break;
        case 4:
            //phonenumber
            valid = validatePositiveLen(parameter);
            break;
        case 5:
            //birthdate
            valid = validateDate(parameter, 0);
            if(valid) {
                setBirthDate(user, parameter);
            }
            break;
        case 6:
            //sex
            valid = validatePositiveLen(parameter);
            if(valid) {
                setSex(user, parameter[0]);
            }
            break;
        case 7:
            //passport
            valid = validatePositiveLen(parameter);
            if(valid) {
                setPassport(user, parameter);
            }
            break;
        case 8:
            //countrycode
            valid = validateCountryCode(parameter);
            if(valid) {
                setCountryCode(user, parameter);
            }
            break;
        case 9:
            // address
            valid = validatePositiveLen(parameter);
            break;
        case 10:
            //account creation
            valid = validateDate(parameter, 1);
            if(valid) {
                setAccountCreation(user, parameter);
            }
            break;
        case 11:
            //paymethod
            valid = validatePositiveLen(parameter);
            break;
        case 12: {
            //acountstatus
            char status;
            valid = validateAccountStatus(parameter, &status);
            if (valid) {
                setAccountStatus(user, status);
            }
            break;
        }
    }

    return valid;
}