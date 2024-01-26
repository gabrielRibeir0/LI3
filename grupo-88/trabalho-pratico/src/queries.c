#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "queries.h"
#include "reservation.h"
#include "flight.h"
#include "user.h"
#include "stdio.h"
#include <glib.h>
#include "statistics.h"
#include "output.h"
#include "utility.h"
#include "manageFlights.h"
#include "manageReservations.h"
#include "manageUsers.h"
#include "interactive.h"
#include "output_interactive.h"

typedef enum id_type{
    USER_ID,
    RESERVATION_ID,
    FLIGHT_ID
} ID_TYPE;

typedef enum q2_output{
    FLIGHTS,
    RESERVATIONS,
    EMPTY
} Q2_OUTPUT;

void Query1_reservation (int size, char* id ,int flag, int command_number, DATA_RESERVATIONS *data_reservations){
    int reservation_id = (int)strtol(id+4, NULL, 10);
    Reservation *reserva  = reservation_hash_lookup(data_reservations, reservation_id);

    if(reserva == NULL){
        addToOutputFile(NULL, 0, 'R', 1, flag, command_number, 1);
    }
    else {
        int hotel_id = getHotelId(reserva);
        char hotelId[8];
        snprintf(hotelId, 8, "HTL%d", hotel_id);
        char *hotelName = getHotelName(data_reservations, hotel_id);

        char hotelStars[2];
        snprintf(hotelStars, 2,"%d", getHotelStars(reserva));

        DATE *hotelBegin = getBeginDate(reserva);
        DATE *hotelEnd = getEndDate(reserva);
        char *hotelBeginDate = dateToStr(hotelBegin, 0);
        char *hotelEndDate = dateToStr(hotelEnd, 0);

        char hotelBreakfastStr[6];
        char hotelBreakfast = getIncludesBreakfast(reserva);
        if(hotelBreakfast == 'F'){
            strcpy(hotelBreakfastStr, "False");
        }
        else if(hotelBreakfast == 'T'){
            strcpy(hotelBreakfastStr, "True");
        }

        int hotelNights = getDay(hotelEnd) - getDay(hotelBegin);
        char *strHotelNights;
        size = asprintf(&strHotelNights, "%d", hotelNights);
        if(size == -1){
            return;
        }

        int price_per_night = getPricePerNight(reserva);
        double hotel_totalprice = price_per_night * hotelNights +
                                ((double)(price_per_night * hotelNights) / (double)100) * getCityTax(reserva);
        char* str_hotel_totalprice;
        size = asprintf(&str_hotel_totalprice, "%.3f", hotel_totalprice);
        if(size == -1){
            return;
        }

        char *fields[8] = {hotelId, hotelName, hotelStars, hotelBeginDate, hotelEndDate, hotelBreakfastStr, strHotelNights,str_hotel_totalprice};
        
        addToOutputFile(fields, 8, 'R', 1, flag, command_number, 1);

        free(str_hotel_totalprice);
        free(hotelBeginDate);
        free(hotelEndDate);
        free(strHotelNights);
        free(hotelName);
        free(hotelBegin);
        free(hotelEnd);
    }
}

void Query1_flight (int size, char* id ,int flag, int command_number, DATA_FLIGHTS *data_flights, DATA_STATISTICS *data_statistics){
    int flight_id = (int)strtol(id, NULL, 10);
    Flight * flight = flight_hash_lookup(data_flights,flight_id);
        
    if(flight == NULL){
        addToOutputFile(NULL, 0, 'F', 1, flag, command_number, 1);
    }
    else {
        char* flightAirline = getAirline(flight);
        char* flightPlane = getPlaneModel (flight);
        char* flightOrigin = getOrigin (flight);
        char* flightDestination = getDestination (flight);
        DATE *flightDeparture = getScheduleDeparture (flight);
        char* flightDepartureDate = dateToStr(flightDeparture, 1);
        DATE *flightArrival = getScheduleArrival(flight);
        char* flightArrivalDate = dateToStr(flightArrival, 1);
        
        int flightPassengers = (int)getStatisticsValue(data_statistics, 'P', 'F',&flight_id);
        char *strFlightPassengers;
        size = asprintf(&strFlightPassengers, "%d",flightPassengers);
        if(size == -1){
            return;
        }

        DATE *real_departure = getRealDeparture(flight);
        int flightDelay = dateDifference(real_departure, flightDeparture);
        char* strFlightDelaysec;
        size = asprintf(&strFlightDelaysec,"%d", flightDelay);
        if(size == -1){
            return;
        }

        char *fields[8] = {flightAirline, flightPlane, flightOrigin,flightDestination, flightDepartureDate, flightArrivalDate, strFlightPassengers, strFlightDelaysec};
        
        addToOutputFile(fields, 8, 'F', 1, flag, command_number, 1);

        free(flightDeparture);
        free(flightArrival);
        free(real_departure);
        free(flightAirline);
        free(flightPlane);
        free(flightOrigin);
        free(flightDestination);
        free(flightArrivalDate);
        free(flightDepartureDate);
        free(strFlightPassengers);
        free(strFlightDelaysec);
    }
}

void Query1_users (int size, char* id ,int flag, int command_number, DATA_USERS *data_users, DATA_STATISTICS *data_statistics){
    User *user = user_hash_lookup(data_users, id);

    if(user == NULL){
        addToOutputFile(NULL, 0, 'U', 1, flag, command_number, 1);
        return;
    }

    char status = getAccountStatus(user);
    if(status == 'A'){
        char* userName = getUserName (user);
        char userSex[2];
        userSex[0] = getSex(user);
        userSex[1] = '\0';
        
        DATE *birth_date = getBirthDate(user);
        
        int UserAge = calculateUserAge(birth_date);
        char* strUserAge;
        size = asprintf(&strUserAge,"%d", UserAge);
        if(size == -1){
            return;
        }
        char* userCountry = getCountryCode (user);
        char* userPassport = getPassport (user);
        
        double userNumberFlights = getStatisticsValue(data_statistics, 'U','F',id);
        char* strUserNumberFlights;
        size = asprintf(&strUserNumberFlights, "%.f", userNumberFlights);
        if(size == -1){
            return;
        }
        double userNumberReservations = getStatisticsValue(data_statistics, 'U','R',id);
        char* strUserNumberReservations;
        size = asprintf(&strUserNumberReservations,"%.f", userNumberReservations);
        if(size == -1){
            return;
        }
        double userTotalSpent = getStatisticsValue(data_statistics, 'U','T',id);
        char *strUserTotalSpent;
        size = asprintf(&strUserTotalSpent, "%.3f", userTotalSpent);
        if(size == -1){
            return;
        }
        char *fields[8] = {userName, userSex, strUserAge, userCountry, userPassport,strUserNumberFlights, strUserNumberReservations, strUserTotalSpent};
        
        addToOutputFile(fields, 8, 'U', 1, flag, command_number, 1);
            
        free(birth_date);
        free(userName);
        free(strUserAge);
        free(userCountry);
        free(userPassport);
        free(strUserNumberFlights);
        free(strUserNumberReservations);
        free(strUserTotalSpent);
    }
    else if(status == 'I'){
        addToOutputFile(NULL, 0, 'U', 1, flag, command_number, 1);
    }
}
/*
Função da query 1, recebe como argumentos o id que é argumento do comando, a flag e o número do commando para uso na apresentação do output e ainda as estruturas de dados
*/
void Query1(char* id ,int flag, int command_number,DATA_RESERVATIONS *data_reservations, DATA_FLIGHTS *data_flights, DATA_USERS *data_users, DATA_STATISTICS *data_statistics){
    int size = 0;
    ID_TYPE id_type;
    if(id[0] == 'B' && id[1] == 'o' && id[2] == 'o' && id[3] == 'k') {
        id_type = RESERVATION_ID;
    }
    else if(id[0] == '0' && id[1] == '0' && id[2] == '0' && id[3] == '0') {
        id_type = FLIGHT_ID;
    }
    else{
        id_type = USER_ID;
    }

    if (id_type == RESERVATION_ID){
        Query1_reservation (size, id, flag, command_number, data_reservations);
    }
    else if (id_type == FLIGHT_ID){
        Query1_flight (size, id, flag, command_number, data_flights, data_statistics);
    }    
    else{
        Query1_users (size, id, flag, command_number, data_users, data_statistics);
    }
}

void Query2(char* user_id, char* type, int flag, int command_number,DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations) {
    Flight *flight;
    Reservation *reservation;
    Q2_OUTPUT output_type;
    char *fields[3] = {NULL, NULL, "\0"};
    if(strcmp(type, "flights") == 0){
       flight = getNextUserFlight(data_users, data_flights, user_id);
       reservation = NULL;
       output_type = FLIGHTS;

    }
    else if(strcmp(type, "reservations") == 0){
        reservation = getNextUserReservation(data_users, data_reservations, user_id);
        flight = NULL;
        output_type = RESERVATIONS;
    }
    else{
        flight = getNextUserFlight(data_users, data_flights, user_id);
        reservation = getNextUserReservation(data_users, data_reservations, user_id);
        output_type = EMPTY;
    }

    if(flight == NULL && reservation == NULL){
        addToOutputFile(NULL, 0 , 'R', 2, flag, command_number, 0);
        return;
    }

    int flight_id, reservation_id;
    char reservation_id_str[15], flight_id_str[11];
    int line_number = 1;
    while(flight != NULL && reservation != NULL){
        DATE *flight_date = getScheduleDeparture(flight);
        DATE *reservation_date = getBeginDate(reservation);
        int compare_date = compareDate(flight_date, reservation_date);
        if(compare_date == -1){
            reservation_id = getReservationId(reservation);
            sprintf(reservation_id_str, "Book%010d", reservation_id);
            char *reservation_date_str = dateToStr(reservation_date, 0);
            fields[0] = reservation_id_str;
            fields[1] = reservation_date_str;
            if(output_type == EMPTY){
                char id_type[12];
                strcpy(id_type, "reservation");
                fields[2] = id_type;
                
                addToOutputFile(fields, 3, 'R', 2, flag, command_number, line_number);
            }
            else{
                char id_type[1];
                strcpy(id_type, "");
                fields[2] = id_type;
                
                addToOutputFile(fields, 2, 'R', 2, flag, command_number, line_number);
            }

            reservation = getNextUserReservation(data_users, data_reservations, user_id);
        }
        else{
            flight_id = getFlightId(flight);
            sprintf(flight_id_str, "%010d", flight_id);
            char *flight_date_str = dateToStr(flight_date, 0);
            fields[0] = flight_id_str;
            fields[1] = flight_date_str;
            if(output_type == EMPTY){
                char id_type[7];
                strcpy(id_type, "flight");
                fields[2] = id_type;
                
                addToOutputFile(fields, 3, 'F', 2, flag, command_number, line_number);
            }
            else{
                char id_type[1];
                strcpy(id_type, "");
                fields[2] = id_type;
                
                addToOutputFile(fields, 2, 'F', 2, flag, command_number, line_number);
            }

            flight = getNextUserFlight(data_users, data_flights, user_id);
        }
        free(reservation_date);
        free(flight_date);
        free(fields[1]);
        line_number++;
    }

    if(flight == NULL){
        while(reservation != NULL){
            //vai precorrendo a lista das reservas
            reservation_id = getReservationId(reservation);
            sprintf(reservation_id_str,"Book%010d", reservation_id);
            DATE *reservation_date = getBeginDate(reservation);
            char *reservation_date_str = dateToStr(reservation_date, 0);
            fields[0] = reservation_id_str;
            fields[1] = reservation_date_str;

            if(output_type == EMPTY){
                char id_type[12];
                strcpy(id_type, "reservation");
                fields[2] = id_type;
                
                addToOutputFile(fields, 3, 'R', 2, flag, command_number, line_number);
            }
            else{
                char id_type[1];
                strcpy(id_type, "");
                fields[2] = id_type;
                
                addToOutputFile(fields, 2, 'R', 2, flag, command_number, line_number);
            }
            free(reservation_date);
            free(fields[1]);
            reservation = getNextUserReservation(data_users, data_reservations, user_id);
            line_number++;
        }
    }
    else if(reservation == NULL){
        while(flight != NULL){
            //vai precorrendo a lista dos voos
            flight_id = getFlightId(flight);
            sprintf(flight_id_str, "%010d", flight_id);
            DATE *flight_date = getScheduleDeparture(flight);
            char *flight_date_str = dateToStr(flight_date, 0);
            fields[0] = flight_id_str;
            fields[1] = flight_date_str;
            if(output_type == EMPTY){
                char id_type[7];
                strcpy(id_type, "flight");
                fields[2] = id_type;
                
                addToOutputFile(fields, 3, 'F', 2, flag, command_number, line_number);
            }
            else{
                char id_type[1];
                strcpy(id_type, "");
                fields[2] = id_type;
                
                addToOutputFile(fields, 2, 'F', 2, flag, command_number, line_number);
            }
            free(flight_date);
            free(fields[1]);
            flight = getNextUserFlight(data_users, data_flights, user_id);
            line_number++;
        }
    }
}

/*
Função da query 3 que recebe o id do hotel, a flag, o número do comando e as estruturas de dados das reservas
*/
void Query3(char *hotel_id, int flag, int command_number, DATA_STATISTICS *data_statistics){
    double rating;
    int hotelId = (int) strtol(hotel_id+3, NULL, 10);
    rating = getHotelRating(data_statistics, &hotelId);
    if(rating == -1){
        addToOutputFile(NULL, 0, 'R', 3, flag, command_number, 0);
        return;
    }

    char average_rating[6];
    snprintf(average_rating, 6, "%.3f", rating);
    char *fields[1] = {average_rating};
    
    addToOutputFile(fields, 1, 'R', 3, flag, command_number, -1);
}

/*
Função da query 4, recebe como argumentos o id do hotel, a flag, o número do comando e os dados as reservas
*/
void Query4 (char *hotel_id, int flag, int command_number, DATA_RESERVATIONS *data_reservations){
    int hotelId = (int) strtol(hotel_id+3, NULL, 10);
    //GSList *reservations_list = getReservationsList(data_reservations, &hotelId);
    Reservation *reservation = getNextReservationsData(data_reservations, &hotelId);
    if(reservation == NULL){
        addToOutputFile(NULL, 0 , 'R', 4, flag, command_number, 0);
        return;
    }

    //Reservation *temp_reservation;
    char *reservation_fields[6];
    int i = 1;
    while(reservation != NULL) {
        int size;
        int id = getReservationId(reservation);
        char reservation_id[15];
        snprintf(reservation_id, 15, "Book%010d", id);
        DATE *begin_date = getBeginDate(reservation);
        DATE *end_date = getEndDate(reservation);
        char *begin_date_str = dateToStr(begin_date, 0);
        char *end_date_str = dateToStr(end_date, 0);
        char *user_id = getReservationUserId(reservation);
        int rating = getRating(reservation);
        char *rating_str;
        size = asprintf(&rating_str, "%d", rating);
        if (size == -1) {
            return;
        }
        int HotelNights = getDay(end_date) - getDay(begin_date);
        int price_per_night = getPricePerNight(reservation);
        double total_price = price_per_night * HotelNights +
                             ((double) (price_per_night * HotelNights) / (double) 100) *
                             getCityTax(reservation);
        char *total_str;
        size = asprintf(&total_str, "%.3f", total_price);
        if (size == -1) {
            return;
        }

        reservation_fields[0] = reservation_id;
        reservation_fields[1] = begin_date_str;
        reservation_fields[2] = end_date_str;
        reservation_fields[3] = user_id;
        reservation_fields[4] = rating_str;
        reservation_fields[5] = total_str;

        
        addToOutputFile(reservation_fields, 6, 'R', 4, flag, command_number, i);
        i++;

        free(reservation_fields[1]);
        free(reservation_fields[2]);
        free(reservation_fields[3]);
        free(reservation_fields[4]);
        free(reservation_fields[5]);
        free(begin_date);
        free(end_date);

        reservation = getNextReservationsData(data_reservations, &hotelId);
    }
}

/*
Função da query 5 que recebe o aeroporto de origem alvo, as datas de início e fim limites, a flag, o número do comando e os dados dos voos
*/
void Query5(char* origin, char* begin_date_str, char* end_date_str, int flag, int command_number, DATA_FLIGHTS *data_flights){//lista de ids de aeroportos, ordenada por data prevista de saida
    DATE *begin_date_limit = strToDate(begin_date_str, 1);
    DATE *end_date_limit = strToDate(end_date_str, 1);

    if(begin_date_limit == NULL || end_date_limit == NULL || compareDate(begin_date_limit, end_date_limit) != -1){
        addToOutputFile(NULL, 0 , 'F', 5, flag, command_number, 0);
        free(begin_date_limit);
        free(end_date_limit);
        return;
    }
    
    Flight *flight = getNextAirportFlight(data_flights, origin);

    if(flight == NULL){
        addToOutputFile(NULL, 0 , 'F', 5, flag, command_number, 0);
    }

    char *flight_fields[5];
    int i = 1;
    while(flight != NULL){
        DATE *temp_begin_date = getScheduleDeparture(flight);
        int compare_begin_date = compareDate(begin_date_limit, temp_begin_date);
        int compare_end_date = compareDate(end_date_limit, temp_begin_date);
        if(compare_begin_date <= 0 && compare_end_date >= 0) {
            int id = getFlightId(flight);
            char flight_id[11];
            sprintf(flight_id, "%010d", id);
            DATE *sch_date = getScheduleDeparture(flight);
            char *sch_date_str = dateToStr(sch_date, 1);

            char *destination = getDestination(flight);

            char *airline = getAirline(flight);
            char *plane_model = getPlaneModel(flight);

            flight_fields[0] = flight_id;
            flight_fields[1] = sch_date_str;
            flight_fields[2] = destination;
            flight_fields[3] = airline;
            flight_fields[4] = plane_model;

            
            addToOutputFile(flight_fields, 5, 'F', 5, flag, command_number, i);
            i++;

            free(flight_fields[1]);
            free(flight_fields[2]);
            free(flight_fields[3]);
            free(flight_fields[4]);
            free(sch_date);
        }

        free(temp_begin_date);
        flight = getNextAirportFlight(data_flights, origin);
    }

    if(i == 1){
        addToOutputFile(NULL, 0 , 'F', 5, flag, command_number, 0);
    }

    free(begin_date_limit);
    free(end_date_limit);
}

/*
Função da query 6 que recebe o ano alvo, o número do top pretendido, a flag, o númmero do comando e os dados das estatísticas
*/
void Query6(int year, int N, int flag, int command_number, DATA_STATISTICS *data_statistics){
    resetAirportStatsIter(data_statistics, year);
    AIRPORT_STATS *stats = getNextAirportStats(data_statistics, year);

    if(stats == NULL || N < 1){
        addToOutputFile(NULL, 0 , 'F', 6, flag, command_number, 0);
        return;
    }

    int size;
    char *airport_fields[2];
    for(int i = 0; stats != NULL && i < N; i++){
        airport_fields[0] = getAirportName(stats);
        int n_passengers = getNPassengers(stats);
        size = asprintf(&airport_fields[1], "%d", n_passengers);
        if(size == -1){
            return;
        }

        addToOutputFile(airport_fields, 2, 'F', 6, flag, command_number, i + 1);

        free(airport_fields[0]);
        free(airport_fields[1]);
        stats = getNextAirportStats(data_statistics, year);
    }
}

void Query7 (int N, int flag, int command_number, DATA_STATISTICS *data_statistics, DATA_FLIGHTS *data_flights){
    resetMedianIter(data_statistics);
    AIRPORT_MEDIAN *airport_median = getNextMedian(data_statistics, data_flights);

    if(airport_median == NULL || N < 1){
        addToOutputFile(NULL, 0, 'F', 7, flag, command_number, 0);
    }

    int size;
    char *airport_fields[2];
    for(int i = 0; airport_median != NULL && i < N; i++){
        airport_fields[0] = getAirportNameMedian(airport_median);
        int median = getMedian(airport_median);
        size = asprintf (&airport_fields[1], "%d", median);
        if(size == -1)
            return;

        addToOutputFile(airport_fields, 2, 'F', 7, flag, command_number, i + 1);

        free(airport_fields[0]);
        free(airport_fields[1]);
        airport_median = getNextMedian(data_statistics, data_flights);
    }
}

/*
Função da query 8 que recebe o id do hotel alvo, as datas de início e fim limites, a flag, o número do comando e os dados dos voos
*/
void Query8 (char *hotel_id, char *begin_date_str, char *end_date_str, int flag, int command_number, DATA_RESERVATIONS *data_reservations){
    int size;
    int revenue = 0;
    int hotelId = (int) strtol(hotel_id+3, NULL, 10);
    DATE *begin_date_limit = strToDate(begin_date_str, 0);
    DATE *end_date_limit = strToDate(end_date_str, 0);
    Reservation *reservation = getNextReservationsData(data_reservations, &hotelId);
    while(reservation != NULL) {
        DATE *BeginDate = getBeginDate(reservation);
        DATE *EndDate = getEndDate(reservation);

        if(compareDate(begin_date_limit ,EndDate) <= 0 && compareDate(end_date_limit ,BeginDate) >= 0){
            if(compareDate(begin_date_limit, BeginDate) <= 0 && compareDate(end_date_limit, EndDate) >= 0){
                int price_per_night = getPricePerNight(reservation);
                int days_begin_date = getDay(BeginDate);
                int days_end_date = getDay(EndDate);
                int date_difference = days_end_date - days_begin_date;
                revenue += price_per_night*date_difference;
            }
            else if (compareDate(begin_date_limit, BeginDate) <= 0 && compareDate(end_date_limit, EndDate) <= 0){
                int price_per_night = getPricePerNight(reservation);
                int days_begin_date = getDay(BeginDate);
                int days_end_date = getDay(end_date_limit);
                int date_difference = days_end_date - days_begin_date + 1;
                revenue += price_per_night*date_difference;
            }
            else if (compareDate(begin_date_limit, BeginDate) >= 0 && compareDate(end_date_limit, EndDate) >= 0){ //B fora E dentro
                int price_per_night = getPricePerNight(reservation);
                int days_begin_date = getDay(begin_date_limit);
                int days_end_date = getDay(EndDate);
                int date_difference = days_end_date - days_begin_date;
                revenue += price_per_night*date_difference;
            }
            else if (compareDate(begin_date_limit, BeginDate) >= 0 && compareDate(end_date_limit, EndDate) <= 0){ //ambas as datas fora
                int price_per_night = getPricePerNight(reservation);
                int days_begin_date = getDay(begin_date_limit);
                int days_end_date = getDay(end_date_limit);
                int date_difference = days_end_date - days_begin_date + 1;
                revenue += price_per_night*date_difference;
            }
        }

        free(BeginDate);
        free(EndDate);
        reservation = getNextReservationsData(data_reservations, &hotelId);
    }
    char *strRevenue;
    size = asprintf(&strRevenue,"%d", revenue);
    if(size == -1){
        return;
    }
    char *fields[1] = {strRevenue};
    
    addToOutputFile(fields, 1, 'R', 8, flag, command_number, -1);

    free(strRevenue);
    free(begin_date_limit);
    free(end_date_limit);
}

void Query9(char *prefix, int flag, int command_number, DATA_USERS *data_users) {
    if(command_number == 458){

    }
    if(prefix == NULL){
        addToOutputFile(NULL, 0, 'U', 9, flag, command_number, 0);
        return;
    }

    char first_letter = prefix[0];
    resetUserIter(data_users, first_letter);
    User *user = getNextUser(data_users, first_letter);
    if(user == NULL){
        addToOutputFile(NULL, 0, 'U', 9, flag, command_number, 0);
        return;
    }

    char *user_fields[2];
    int i = 1;
    while(user != NULL){
        char *user_name = getUserName(user);
        int has_prefix = strncmp(user_name, prefix, strlen (prefix));
        if(has_prefix == 0){
            user_fields[0] = getUserId(user);
            user_fields[1] = user_name;

            addToOutputFile(user_fields, 2, 'U', 9, flag, command_number, i);
            i++;
            free(user_fields[0]);
        }

        free(user_name);
        user = getNextUser(data_users, first_letter);
    }

    if(i == 1){
        addToOutputFile(NULL, 0, 'U', 9, flag, command_number, 0);
    }
}

void Query10(int year, int month, int flag, int command_number, DATA_STATISTICS *data_statistics){
    int users = 0, flights = 0, passengers = 0, unique_passengers = 0, reservations = 0, exists, size;
    char *fields[6];
    char *users_str = NULL, *flights_str = NULL, *passengers_str = NULL, *unique_passengers_str = NULL, *reservations_str = NULL;
    if(year != 0 && month != 0){
        //temos o ano e o mês nos argumentos -> agrupar por dia
        int n_days;
        if(month == 2){
            n_days = 29;
        }
        else{
            n_days = 31;
        }

        int line = 1;
        for(int i = 0; i < n_days; i++) {
            exists = getYearStats(data_statistics, year, month-1, i, &users, &flights, &passengers, &unique_passengers, &reservations);

            if (exists == 1) {
                char day_str[3];
                sprintf(day_str, "%d", i+1);
                size = asprintf(&users_str, "%d", users);
                if(size == -1)
                    return;
                size = asprintf(&flights_str, "%d", flights);
                if(size == -1)
                    return;
                size = asprintf(&passengers_str, "%d", passengers);
                if(size == -1)
                    return;
                size = asprintf(&unique_passengers_str, "%d", unique_passengers);
                if(size == -1)
                    return;
                size = asprintf(&reservations_str, "%d", reservations);
                if(size == -1)
                    return;

                fields[0] = day_str;
                fields[1] = users_str;
                fields[2] = flights_str;
                fields[3] = passengers_str;
                fields[4] = unique_passengers_str;
                fields[5] = reservations_str;

                addToOutputFile(fields, 6, 'D', 10, flag, command_number, line);

                free(users_str);
                free(flights_str);
                free(passengers_str);
                free(unique_passengers_str);
                free(reservations_str);
                line++;
            }
        }
    }
    else if(year != 0){
        //temos só o ano -> agrupar por mês
        int line = 1;
        for(int i = 0; i < 12; i++) {
            exists = getYearStats(data_statistics, year, i, -1, &users, &flights, &passengers, &unique_passengers, &reservations);

            if (exists == 1) {
                char month_str[3];
                sprintf(month_str, "%d", i+1);
                size = asprintf(&users_str, "%d", users);
                if(size == -1)
                    return;
                size = asprintf(&flights_str, "%d", flights);
                if(size == -1)
                    return;
                size = asprintf(&passengers_str, "%d", passengers);
                if(size == -1)
                    return;
                size = asprintf(&unique_passengers_str, "%d", unique_passengers);
                if(size == -1)
                    return;
                size = asprintf(&reservations_str, "%d", reservations);
                if(size == -1)
                    return;

                fields[0] = month_str;
                fields[1] = users_str;
                fields[2] = flights_str;
                fields[3] = passengers_str;
                fields[4] = unique_passengers_str;
                fields[5] = reservations_str;

                addToOutputFile(fields, 6, 'M', 10, flag, command_number, line);

                free(users_str);
                free(flights_str);
                free(passengers_str);
                free(unique_passengers_str);
                free(reservations_str);
                line++;
            }
        }
    }
    else{
        //não temos nenhum -> agrupar por ano
        int y = getNextYear(data_statistics);
        int line = 1;
        while(y != -1){
            exists = getYearStats(data_statistics, y, -1, -1, &users, &flights, &passengers, &unique_passengers, &reservations);
            if(exists == 1) {
                char year_str[5];
                sprintf(year_str, "%d", y);
                size = asprintf(&users_str, "%d", users);
                if (size == -1)
                    return;
                size = asprintf(&flights_str, "%d", flights);
                if (size == -1)
                    return;
                size = asprintf(&passengers_str, "%d", passengers);
                if (size == -1)
                    return;
                size = asprintf(&unique_passengers_str, "%d", unique_passengers);
                if (size == -1)
                    return;
                size = asprintf(&reservations_str, "%d", reservations);
                if (size == -1)
                    return;

                fields[0] = year_str;
                fields[1] = users_str;
                fields[2] = flights_str;
                fields[3] = passengers_str;
                fields[4] = unique_passengers_str;
                fields[5] = reservations_str;

                addToOutputFile(fields, 6, 'Y', 10, flag, command_number, line);

                free(users_str);
                free(flights_str);
                free(passengers_str);
                free(unique_passengers_str);
                free(reservations_str);
                line++;
            }

            y = getNextYear(data_statistics);
        }
    }
}


/*
Função 'query_manager' responsável por identificar qual querie a executar com base no id recebido e preparar os argumentos para passar às queries
*/
void query_manager(DATA_FLIGHTS *data_flights,DATA_RESERVATIONS * data_reservations,DATA_USERS *data_users,DATA_STATISTICS *data_statistics,int query_id, int nargs, char **args, int flag, int command_number, PROGRAM_MODE mode){
    int ret;
    switch(query_id) {
        case 1:
            if(nargs == 1){
                Query1(args[0],flag,command_number, data_reservations, data_flights, data_users, data_statistics);
            }
            break;
        case 2:
            if(nargs == 1){
                Query2(args[0], "", flag, command_number, data_users, data_flights, data_reservations);
            }
            else if(nargs == 2){
                Query2(args[0], args[1], flag, command_number, data_users, data_flights, data_reservations);
            }
            break;
        case 3:
             if(nargs == 1){
                 Query3(args[0],flag, command_number, data_statistics);
             }
            break;
        case 4:
            if(nargs == 1){
                Query4(args[0], flag, command_number, data_reservations);
            }
            break;
        case 5:
            if(nargs == 5){
                char *arg_begin_date;
                ret = asprintf(&arg_begin_date, "%s %s", args[1], args[2]);
                if(ret == -1){
                    return;
                }
                char *arg_end_date;
                ret = asprintf(&arg_end_date, "%s %s", args[3], args[4]);
                if(ret == -1){
                    return;
                }
                Query5(args[0], arg_begin_date, arg_end_date, flag, command_number, data_flights);
                if(ret == 1){
                    return;
                }

                free(arg_begin_date);
                free(arg_end_date);
            }
            break;
        case 6:
            if(nargs == 2){
                int year = (int)strtol(args[0], NULL, 10);
                int N = (int)strtol(args[1], NULL, 10);
                Query6(year, N, flag, command_number, data_statistics);
            }
            break;
        case 7:
            if(nargs == 1){
                int N = (int)strtol(args[0], NULL, 10);
                Query7(N, flag, command_number, data_statistics, data_flights);
            }
            break;
        case 8:
            Query8(args[0], args[1], args[2], flag, command_number, data_reservations);
            break;
        case 9:
            if(nargs == 1){
                Query9(args[0], flag, command_number, data_users);
            }
            else if(nargs > 1){
                int total_len = 0;
                for(int i = 0; i < nargs; i++){
                    total_len += strlen(args[i]) + 1;
                }

                char prefix[total_len];
                strcpy(prefix, args[0]);
                for(int i = 1; i < nargs; i++){
                    strcat(prefix, " ");
                    strcat(prefix, args[i]);
                }

                Query9(prefix, flag, command_number, data_users);
            }
            break;
        case 10:
            if(nargs == 0){
                Query10(0, 0, flag, command_number, data_statistics);
            }
            else if(nargs == 1){
                int year = (int)strtol(args[0], NULL, 10);
                Query10(year, 0, flag, command_number, data_statistics);
            }
            else if(nargs == 2){
                int year = (int)strtol(args[0], NULL, 10);
                int month = (int)strtol(args[1], NULL, 10);
                Query10(year, month, flag, command_number, data_statistics);
            }
            break;
        default:
            addToOutputFile(NULL, 0, 'U', query_id, flag, command_number, 1);
            break;
    }

    if(mode == INTERACTIVE) {
        output_menu(command_number);
    }
}