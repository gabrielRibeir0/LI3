#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include <glib.h>
#include <stdio.h>
#include "managePassengers.h"
#include "validation.h"
#include "statistics.h"
#include "output.h"

void processPassenger(DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_STATISTICS *data_statistics, Passenger *passenger, int valid_parameter, int inactive_user, int *header_written, char *original_line){
    if (valid_parameter && inactive_user == 0) {
        //passageiros válidos e inativos
        //ir buscar os dados necessários
        int flight_id = getPassengerFlightId(passenger);
        int *flight_id_key = getFlightIdFromHash(data_flights, flight_id);
        char *passenger_id = getPassengerId(passenger);
        char *user_id = getUserIdFromHash(data_users, passenger_id);
        Flight *flight = flight_hash_lookup(data_flights, flight_id);
        char *origin = getOrigin(flight);
        char *destination = getDestination(flight);
        DATE *sch_departure = getScheduleDeparture(flight);
        int year = getYear(sch_departure);

        //inserir dados e aumentar estatísticas
        incNPassengers(data_statistics, flight_id_key);
        incUserStats(data_statistics, user_id, FLIGHT, 1);
        incAirportStats(data_statistics, origin, destination, year);
        insertFlightBookingHash(data_users, user_id, flight);
        incYearStats(data_statistics, sch_departure, PASSENGER, user_id);

        //libertar os dados
        free(passenger_id);
        free(origin);
        free(destination);
        free(sch_departure);
    }
    else if (valid_parameter){
        //passageiros válidos e ativos
        int flight_id = getPassengerFlightId(passenger);
        int *flight_id_key = getFlightIdFromHash(data_flights, flight_id);
        char *passenger_id = getPassengerId(passenger);
        char *user_id = getUserIdFromHash(data_users, passenger_id);
        Flight *flight = flight_hash_lookup(data_flights, flight_id);
        char *origin = getOrigin(flight);
        char *destination = getDestination(flight);
        DATE *sch_departure = getScheduleDeparture(flight);
        int year = getYear(sch_departure);

        incNPassengers(data_statistics, flight_id_key);
        incAirportStats(data_statistics, origin, destination, year);
        incYearStats(data_statistics, sch_departure, PASSENGER, user_id);

        free(passenger_id);
        free(origin);
        free(destination);
        free(sch_departure);
    }
    else{
        //passageiros inválidos
        int ret;
        ret = addToErrorsFile("./Resultados/passengers_errors.csv", "flight_id;user_id\n", original_line, *header_written);

        if(ret == 1){
            fputs("Couldn't add the line to 'passengers_errors.csv'", stderr);
            return;
        }
        if (!(*header_written)) {
            *header_written = 1;
        }
    }
}

int validateAndSetPassengerParameter(Passenger *passenger, char *field, int num_parameter, DATA_FLIGHTS *data_flights, DATA_USERS *data_users){
    int valid;

    if(num_parameter == 1) {
        //flight_id
        valid = validatePositiveLen(field);

        if(valid){
            gboolean contains = flights_hash_contains(data_flights, field);
            if(contains == FALSE){
                valid = 0;
            }
            else{
                setPassengerFlightId(passenger, field);
            }
        }
    }
    else{
        //user_id
        int len = strlen(field);
        field[len - 1] = '\0';
        valid = validatePositiveLen(field);
        if(valid){
            gboolean contains_active = users_hash_contains(data_users, field);
            if (contains_active == FALSE) {
                gboolean contains_inactive = inactive_users_hash_contains(data_users, field);
                if(contains_inactive == TRUE){
                    setPassengerId(passenger, field);
                    valid = 2;
                }
                else {
                    valid = 0;
                }
            }
            else {
                setPassengerId(passenger, field);
            }
        }

        field[len - 1] = '\n';
    }

    return valid;
}