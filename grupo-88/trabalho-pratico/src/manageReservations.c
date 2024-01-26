#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "manageReservations.h"
#include "manageUsers.h"
#include "reservation.h"
#include "validation.h"
#include "output.h"
#include <glib.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

struct reservations_list{
    GSList *reservations_list;
    int is_sorted;
    GSList *iter_reservations;
};

//struct das reservas
struct data_reservations{
    GHashTable *reservations_hash;
    GHashTable *hotels_hash;
    GHashTable *hotel_id_to_name;
};

DATA_RESERVATIONS *data_reservations_init(){
    DATA_RESERVATIONS *data = malloc(sizeof(DATA_RESERVATIONS));
    data->reservations_hash = g_hash_table_new_full(g_int_hash, g_int_equal, free, NULL);
    data->hotels_hash = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, freeHotelList);
    data->hotel_id_to_name = g_hash_table_new_full(g_int_hash, g_int_equal, free, free);
    return data;
}

void freeHotelList(void *list_pointer){
    RESERVATIONS_LIST *rl = (RESERVATIONS_LIST *)list_pointer;
    g_slist_free_full(g_steal_pointer(&(rl->reservations_list)), freeReservation);
    free(rl);
}

//função para dar free aos dados da hashtable das reservas
void freeReservationsData(DATA_RESERVATIONS *data){
    g_hash_table_destroy(data->reservations_hash);
    g_hash_table_destroy(data->hotels_hash);
    g_hash_table_destroy(data->hotel_id_to_name);

    free(data);
}

//função para inserir na hashtable das reservas
void insertReservationHash(DATA_RESERVATIONS *data, Reservation *reservation, int *reservation_id){
    g_hash_table_insert(data->reservations_hash, reservation_id, reservation);
}

void insertHotelsHash(DATA_RESERVATIONS *data, int *hotel_id, Reservation *reservation){
    RESERVATIONS_LIST *rl = g_hash_table_lookup(data->hotels_hash, hotel_id);
    if(rl == NULL){
        RESERVATIONS_LIST *new_rl = malloc(sizeof(RESERVATIONS_LIST));
        new_rl->reservations_list = NULL;
        new_rl->is_sorted = 0;
        new_rl->reservations_list = g_slist_prepend(new_rl->reservations_list, reservation);
        new_rl->iter_reservations = NULL;
        g_hash_table_insert(data->hotels_hash, hotel_id, new_rl);
    }
    else{
        rl->reservations_list = g_slist_prepend(rl->reservations_list, reservation);
    }
}

void insertHotelName(DATA_RESERVATIONS *data, int hotel_id, char *hotel_name){
    gboolean name_exists = g_hash_table_contains(data->hotel_id_to_name, &hotel_id);
    if(name_exists == FALSE){
        char *name = strdup(hotel_name);
        int *id = malloc(sizeof(int));
        *id = hotel_id;
        g_hash_table_insert(data->hotel_id_to_name, id, name);
    }
}

//função para procurar uma reserva na hashtable através do id
Reservation *reservation_hash_lookup(DATA_RESERVATIONS *data, int reservation_id){
    return (g_hash_table_lookup(data->reservations_hash, &reservation_id));
}

Reservation *getNextReservationsData(DATA_RESERVATIONS *data_reservations, int *hotel_id){
    RESERVATIONS_LIST *rl = g_hash_table_lookup(data_reservations->hotels_hash, hotel_id);
    if(rl == NULL){
        return NULL;
    }

    if(rl->is_sorted == 0){
        rl->reservations_list = g_slist_sort(rl->reservations_list, compare_reservations);
        rl->is_sorted = 1;
    }
    
    Reservation *reservation;
    if(rl->iter_reservations == NULL){
        rl->iter_reservations = rl->reservations_list;
        reservation = (Reservation *)rl->iter_reservations->data;
    }
    else{
        rl->iter_reservations = rl->iter_reservations->next;
        if(rl->iter_reservations == NULL){
            reservation = NULL;
        }
        else{
            reservation = (Reservation *)rl->iter_reservations->data;
        }
    }

    return reservation;
}

char *getHotelName(DATA_RESERVATIONS *data, int hotel_id){
    char *hotel_name = g_hash_table_lookup(data->hotel_id_to_name, &hotel_id);
    if(hotel_name == NULL){
        return NULL;
    }
    return strdup(hotel_name);
}

int compare_reservations(gconstpointer reservation1_pointer, gconstpointer reservation2_pointer){
    Reservation *reservation1 = (Reservation *)reservation1_pointer;
    Reservation *reservation2 = (Reservation *)reservation2_pointer;

    DATE *begin_date1 = getBeginDate(reservation1);
    DATE *begin_date2 = getBeginDate(reservation2);
    int compare_date = compareDate(begin_date2, begin_date1);
    free(begin_date1);
    free(begin_date2);
    if (compare_date == 0) {
        int id1 = getReservationId(reservation1);
        int id2 = getReservationId(reservation2);
        if (id1 >= id2) {
            return 1;
        } else {
            return -1;
        }
    } else {
        return compare_date;
    }
}

int compare_reservations_id(gconstpointer id1, gconstpointer id2, gpointer data_flights){
    DATA_RESERVATIONS *data = (DATA_RESERVATIONS *)data_flights;
    Reservation *reservation1 = g_hash_table_lookup(data->reservations_hash, id1);
    Reservation *reservation2 = g_hash_table_lookup(data->reservations_hash, id2);

    return compare_reservations(reservation1, reservation2);
}

void processReservation(DATA_RESERVATIONS *data_reservations, DATA_USERS *data_users, DATA_STATISTICS *data_statistics, Reservation *reservation, int valid_parameter, int inactive_user, int *header_written, char *original_line){
    if(valid_parameter){
        DATE *begin_date = getBeginDate(reservation);
        DATE *end_date = getEndDate(reservation);
        if (compareDate(begin_date, end_date) != -1) {
            valid_parameter = 0;
        }
        free(begin_date);
        free(end_date);
    }

    if (valid_parameter && inactive_user == 0) {
        DATE *begin_date = getBeginDate(reservation);
        DATE *end_date = getEndDate(reservation);
        int *hotel_id = malloc(sizeof(int));
        (*hotel_id) = getHotelId(reservation);
        int rating = getRating(reservation);
        char *user_id = getReservationUserId(reservation);
        char *user_id_key = getUserIdFromHash(data_users, user_id);
        int *reservation_id = malloc(sizeof(int));
        *reservation_id = getReservationId(reservation);
        int hotelNights = getDay(end_date) - getDay(begin_date);
        int price_per_night = getPricePerNight(reservation);
        double total_price = price_per_night * hotelNights +
                             ((double) (price_per_night * hotelNights) / (double) 100) *
                             getCityTax(reservation);

        g_hash_table_insert(data_reservations->reservations_hash, reservation_id, reservation);
        insertHotelsHash(data_reservations, hotel_id, reservation);
        incUserStats(data_statistics, user_id_key, RESERVATION, total_price);
        incHotelStats(data_statistics, hotel_id, rating);
        insertReservationBookingHash(data_users, user_id_key, reservation);
        incYearStats(data_statistics, begin_date, RESERVATION, NULL);

        free(user_id);
        free(begin_date);
        free(end_date);
    }
    else if(valid_parameter){
        DATE *begin_date = getBeginDate(reservation);
        int *hotel_id = malloc(sizeof(int));
        (*hotel_id) = getHotelId(reservation);
        int rating = getRating(reservation);
        int *reservation_id = malloc(sizeof(int));
        *reservation_id = getReservationId(reservation);
        g_hash_table_insert(data_reservations->reservations_hash, reservation_id, reservation);
        insertHotelsHash(data_reservations, hotel_id, reservation);
        incHotelStats(data_statistics, hotel_id, rating);
        incYearStats(data_statistics, begin_date, RESERVATION, NULL);

        free(begin_date);
    }
    else{
        int ret;
        ret = addToErrorsFile("./Resultados/reservations_errors.csv",
                              "id;user_id;hotel_id;hotel_name;hotel_stars;city_tax;address;begin_date;end_date;price_per_night;includes_breakfast;room_details;rating;comment\n",
                              original_line, *header_written);
        if (ret == 1) {
            fputs("Couldn't add the line to 'reservations_errors.csv'.", stderr);
            return;
        }
        if (!(*header_written)) {
            *header_written = 1;
        }

        freeReservation(reservation);
    }
}

//função para validar e definir os parametros de uma reserva
int validateAndSetReservationParameter(Reservation* reservation, char* parameter, int num_parameter, DATA_USERS *data_user, DATA_RESERVATIONS *data_reservations){
    int valid = 1;
    switch(num_parameter){
        case 1:
            //id
            valid = validatePositiveLen(parameter);
            if(valid) {
                setReservationId(reservation, parameter);
            }
            break;
        case 2:
            //user id
            valid = validatePositiveLen(parameter);

            if(valid) {
                gboolean contains_active = users_hash_contains(data_user, parameter);
                if (contains_active == FALSE) {
                    gboolean contains_inactive = inactive_users_hash_contains(data_user, parameter);
                    if(contains_inactive == TRUE){
                        setReservationUserId(reservation, parameter);
                        valid = 2;
                    }
                    else {
                        valid = 0;
                    }
                }
                else{
                    setReservationUserId(reservation, parameter);
                }
            }
            break;
        case 3:
            //hotel id
            valid = validatePositiveLen(parameter);
            if(valid){
                setHotelId(reservation, parameter);
            }
            break;
        case 4:
            //hotel name
            valid = validatePositiveLen(parameter);
            if(valid) {
                int hotel_id = getHotelId(reservation);
                insertHotelName(data_reservations, hotel_id, parameter);
            }
            break;
        case 5:
            //hotel stars
            valid = validateInt1to5(parameter);
            if(valid) {
                setHotelStars(reservation, parameter);
            }
            break;
        case 6:
            //city tax
            valid = validatePosInt(parameter);
            if(valid){
                setCityTax(reservation, parameter);
            }
            break;
        case 7:
            //address
            valid = validatePositiveLen(parameter);
            break;
        case 8:
            //begin_date
            valid = validateDate(parameter, 0);
            if(valid) {
                setBeginDate(reservation, parameter);
            }
            break;
        case 9:
            //end date
            valid = validateDate(parameter, 0);
            if(valid) {
                setEndDate(reservation, parameter);
            }
            break;
        case 10:
            //price per night
            valid = validatePosInt(parameter);
            if(valid){
                setPricePerNight(reservation, parameter);
            }
            break;
        case 11: {
            //includes breakfast
            char inc_breakfast;
            valid = validateInclBreakfast(parameter, &inc_breakfast);
            if (valid) {
                setIncludesBreakfast(reservation, inc_breakfast);
            }
            break;
        }
        case 13:
        //rating
            valid = validateRating(parameter);
            if(valid){
                setRating(reservation, parameter);
            }
            break;
        default:
            //room details & comment
            break;
    }

    return valid;
}