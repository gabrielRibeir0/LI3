#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "manageFlights.h"
#include "statistics.h"
#include "validation.h"
#include "output.h"
#include <stdio.h>
#include <glib.h>

struct data_flights{
    GHashTable *flights_hash;
    GHashTable *airports_hash;
    GHashTable *flights_ids_hash;
};

struct flights_and_delays{
    GSList *flights_list;
    GSList *delays_list;
    int length_delays;
    int is_sorted_flights;
    int is_sorted_delays;
    GSList *flights_iter;
};

DATA_FLIGHTS *data_flights_init(){
    DATA_FLIGHTS *data = malloc(sizeof(DATA_FLIGHTS));
    data->flights_hash = g_hash_table_new_full(g_int_hash, g_int_equal,NULL,freeFlight);
    data->airports_hash = g_hash_table_new_full(g_str_hash, g_str_equal, free, freeFlightsNDelays);
    data->flights_ids_hash = g_hash_table_new_full(g_int_hash, g_int_equal, free, NULL);
    return data;
}

void freeFlightsNDelays(void *fnd_pointer){
    FLIGHTS_N_DELAYS *flights_delays = (FLIGHTS_N_DELAYS *)fnd_pointer;
    g_slist_free(flights_delays->flights_list);
    g_slist_free_full(flights_delays->delays_list, free);
    free(flights_delays);
}

void freeFlightsData(DATA_FLIGHTS *data){
    g_hash_table_destroy(data->flights_hash);
    g_hash_table_destroy(data->airports_hash);
    g_hash_table_destroy(data->flights_ids_hash);
    free(data);
}

int insertAirportHash(DATA_FLIGHTS *data_flights, Flight *flight, char *airport, int delay){
    FLIGHTS_N_DELAYS *flights_delays = g_hash_table_lookup(data_flights->airports_hash, airport);
    int *value = malloc(sizeof(int));
    *value = delay;
    if(flights_delays == NULL){
        FLIGHTS_N_DELAYS *new_fnd = malloc(sizeof(FLIGHTS_N_DELAYS));
        new_fnd->is_sorted_flights = 0;
        new_fnd->is_sorted_delays = 0;
        new_fnd->flights_list = NULL;
        new_fnd->delays_list = NULL;
        new_fnd->flights_iter = NULL;
        new_fnd->flights_list = g_slist_prepend(new_fnd->flights_list, flight);
        new_fnd->delays_list = g_slist_prepend(new_fnd->delays_list, value);
        new_fnd->length_delays = 1;
        g_hash_table_insert(data_flights->airports_hash, airport, new_fnd);
        return 1;
    }
    else{
        flights_delays->flights_list = g_slist_prepend(flights_delays->flights_list, flight);
        flights_delays->delays_list = g_slist_prepend(flights_delays->delays_list, value);
        flights_delays->length_delays++;
        free(airport);
        return 0;
    }
}

Flight *flight_hash_lookup(DATA_FLIGHTS *data, int flight_id){
    return(g_hash_table_lookup(data->flights_hash,&flight_id));
}

int *getFlightIdFromHash(DATA_FLIGHTS *data, int flight_id){
    return g_hash_table_lookup(data->flights_ids_hash, &flight_id);
}

Flight *getNextAirportFlight(DATA_FLIGHTS *data_flights, char *airport_id){
    FLIGHTS_N_DELAYS *flights_delays = g_hash_table_lookup(data_flights->airports_hash, airport_id);
    if(flights_delays == NULL){
        return NULL;
    }

    if(flights_delays->is_sorted_flights == 0){
        flights_delays->flights_list = g_slist_sort(flights_delays->flights_list, compare_flights);
        flights_delays->is_sorted_flights = 1;
    }

    Flight *flight;
    if(flights_delays->flights_iter == NULL){
        flights_delays->flights_iter = flights_delays->flights_list;
        flight = (Flight *)flights_delays->flights_iter->data;
    }
    else{
        flights_delays->flights_iter = flights_delays->flights_iter->next;
        if(flights_delays->flights_iter == NULL){
            flight = NULL;
        }
        else{
            flight = (Flight *)flights_delays->flights_iter->data;
        }
    }

    return flight;
}

int getMedianDelays(DATA_FLIGHTS *data, char *airport){
    FLIGHTS_N_DELAYS *flights_delays = g_hash_table_lookup(data->airports_hash, airport);
    int median;
    if(flights_delays->is_sorted_delays == 0){
        flights_delays->delays_list = g_slist_sort(flights_delays->delays_list, compareDelays);
        flights_delays->is_sorted_delays = 1;
    }

    GSList *aux = flights_delays->delays_list;
    GSList *prev = NULL;
    for(int i = 0; i < flights_delays->length_delays / 2; i++){
        prev = aux;
        aux = aux->next;
    }

    if(flights_delays->length_delays % 2 == 0){
        median = (*(int *)aux->data + *(int *)prev->data) / 2;
    }
    else{
        median = *(int *)aux->data;
    }

    return median;
}

//função para verificar se uma hash contem um parametro
gboolean flights_hash_contains(DATA_FLIGHTS *data, char *parameter){
    int flight_id = (int)strtol(parameter, NULL, 10);
    return g_hash_table_contains(data->flights_hash, &flight_id);
}

int compare_flights(gconstpointer flight1_pointer, gconstpointer flight2_pointer){
    Flight *flight1 = (Flight *)flight1_pointer;
    Flight *flight2 = (Flight *)flight2_pointer;

    DATE *sch_dep_date1 = getScheduleDeparture(flight1);
    DATE *sch_dep_date2 = getScheduleDeparture(flight2);
    int compare_date = compareDate(sch_dep_date2, sch_dep_date1);
    free(sch_dep_date1);
    free(sch_dep_date2);
    if (compare_date == 0) {
        int id1 = getFlightId(flight1);
        int id2 = getFlightId(flight2);
        if (id1 >= id2) {
            return 1;
        } else {
            return -1;
        }
    }else {
        return compare_date;
    }
}

int compare_flights_id(gconstpointer id1, gconstpointer id2, gpointer data_flights){
    DATA_FLIGHTS *data = (DATA_FLIGHTS *)data_flights;
    Flight *flight1 = g_hash_table_lookup(data->flights_hash, id1);
    Flight *flight2 = g_hash_table_lookup(data->flights_hash, id2);

    return compare_flights(flight1, flight2);
}

int compareDelays(gconstpointer airport_delay1, gconstpointer airport_delay2){
    int *delay1 = (int *)airport_delay1;
    int *delay2 = (int *)airport_delay2;

    if(*delay1 < *delay2){
        return -1;
    }
    else{
        return *delay1 != *delay2;
    }
}

void processFlight(DATA_FLIGHTS *data_flights, DATA_STATISTICS *data_statistics, Flight *flight, int valid_parameter, int *header_written, char *original_line){
    if(valid_parameter){
        DATE *sch_departure = getScheduleDeparture(flight);
        DATE *sch_arrival = getScheduleArrival(flight);
        DATE *real_departure = getRealDeparture(flight);
        DATE *real_arrival = getRealArrival(flight);
        if(compareDate(sch_departure, sch_arrival) == -1 && compareDate(real_departure, real_arrival) == -1) {
            int new;
            int *flight_id = malloc(sizeof(int));
            *flight_id = getFlightId(flight);
            char *origin = getOrigin(flight);
            int delay = dateDifference(real_departure, sch_departure);
            g_hash_table_add(data_flights->flights_ids_hash, flight_id);
            g_hash_table_insert(data_flights->flights_hash,flight_id,flight);
            new = insertAirportHash(data_flights, flight, origin, delay);
            if(new){
                prependAirportId(data_statistics, origin);
            }
            incYearStats(data_statistics, sch_departure, FLIGHT, NULL);
        }
        else{
            int ret;
            ret = addToErrorsFile("./Resultados/flights_errors.csv", "id;airline;plane_model;total_seats;origin;destination;schedule_departure_date;schedule_arrival_date;real_departure_date;real_arrival_date;pilot;copilot;notes\n" ,
                                  original_line, *header_written);
            if(ret == 1) {
                fputs("Couldn't add the line to 'flights_errors.csv'.", stderr);
                return;
            }
            if(!(*header_written)){
                *header_written = 1;
            }

            freeFlight(flight);
        }
        free(sch_departure);
        free(sch_arrival);
        free(real_departure);
        free(real_arrival);
    }
    else{
        int ret;
        ret = addToErrorsFile("./Resultados/flights_errors.csv", "id;airline;plane_model;total_seats;origin;destination;schedule_departure_date;schedule_arrival_date;real_departure_date;real_arrival_date;pilot;copilot;notes\n" ,
                              original_line, *header_written);
        if(ret == 1) {
            fputs("Couldn't add the line to 'flights_errors.csv'.", stderr);
            return;
        }
        if(!(*header_written)){
            *header_written = 1;
        }

        freeFlight(flight);
    }
}

//função para validar e definir os parametros de um voo
int validateAndSetFlightParameter(Flight *flight, char* parameter, int num_parameter){
    int valid = 1;

    switch (num_parameter) {
        case 1:
            //flight_id
            valid = validatePositiveLen(parameter);
            if(valid) {
                setFlightId(flight, parameter);
            }
            break;
        case 2:
            //airline
            valid = validatePositiveLen(parameter);
            if(valid) {
                setAirline(flight, parameter);
            }
            break;
        case 3:
            //plane_model
            valid = validatePositiveLen(parameter);
            if(valid) {
                setPlaneModel(flight, parameter);
            }
            break;
        case 4:
            //total seats
            valid = validatePosInt(parameter);
            break;
        case 5:
            //origin
            valid = validateAirport(parameter);
            if(valid) {
                setOrigin(flight, parameter);
            }
            break;
        case 6:
            //destination
            valid = validateAirport(parameter);
            if(valid) {
                setDestination(flight, parameter);
            }
            break;
        case 7:
            //schedule_departure_date
            valid = validateDate(parameter, 1);
            if(valid) {
                setScheduleDeparture(flight, parameter);
            }
            break;
        case 8:
            //schedule arrival date
            valid = validateDate(parameter, 1);
            if(valid) {
                setScheduleArrival(flight, parameter);
            }
            break;
        case 9:
            //real departure date
            valid = validateDate(parameter, 1);
            if(valid) {
                setRealDeparture(flight, parameter);
            }
            break;
        case 10:
            //real arrival date
            valid = validateDate(parameter, 1);
            if(valid) {
                setRealArrival(flight, parameter);
            }
            break;
        case 11:
            //pilot
            valid = validatePositiveLen(parameter);
            break;
        case 12:
            //copilot
            valid = validatePositiveLen(parameter);
            break;
        default:
            //notes
            break;
    }

    return valid;
}