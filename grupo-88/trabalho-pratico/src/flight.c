#include "flight.h"
#include "utility.h"
#include <stdlib.h>
#include <string.h>

struct flight{
    int id;
    char* airline;
    char* plane_model;
    char origin[4];
    char destination[4];
    DATE* schedule_departure_date;
    DATE* schedule_arrival_date;
    DATE* real_departure_date;
    DATE* real_arrival_date;
};

//GETTERS
int getFlightId(Flight* flight){
    return flight->id;
}

char* getAirline(Flight* flight){
    return strdup(flight->airline);
}

char* getPlaneModel(Flight* flight){
    return strdup(flight->plane_model);
}

char* getOrigin(Flight* flight){
    return strdup(flight->origin);
}

char* getDestination(Flight* flight){
    return strdup(flight->destination);
}

DATE* getScheduleDeparture(Flight* flight){
    DATE *new_date = createDate(getYear(flight->schedule_departure_date), getMonth(flight->schedule_departure_date),
                                getDay(flight->schedule_departure_date), getHour(flight->schedule_departure_date),
                                getMinutes(flight->schedule_departure_date), getSeconds(flight->schedule_departure_date));

    return new_date;
}

DATE* getScheduleArrival(Flight* flight){
    DATE *new_date = createDate(getYear(flight->schedule_arrival_date), getMonth(flight->schedule_arrival_date),
                                getDay(flight->schedule_arrival_date), getHour(flight->schedule_arrival_date),
                                getMinutes(flight->schedule_arrival_date), getSeconds(flight->schedule_arrival_date));

    return new_date;
}

DATE* getRealDeparture(Flight* flight){
    DATE *new_date = createDate(getYear(flight->real_departure_date), getMonth(flight->real_departure_date),
                                getDay(flight->real_departure_date), getHour(flight->real_departure_date),
                                getMinutes(flight->real_departure_date), getSeconds(flight->real_departure_date));

    return new_date;
}

DATE* getRealArrival(Flight* flight){
    DATE *new_date = createDate(getYear(flight->real_arrival_date), getMonth(flight->real_arrival_date),
                                getDay(flight->real_arrival_date), getHour(flight->real_arrival_date),
                                getMinutes(flight->real_arrival_date), getSeconds(flight->real_arrival_date));

    return new_date;
}

//SETTERS
void setFlightId(Flight* flight, char *id){
    int value = (int) strtol(id, NULL, 10);
    flight->id = value;
}

void setAirline(Flight* flight, char *id){
    flight->airline = malloc (sizeof (char) * (strlen(id) +1));
    strcpy(flight->airline, id);
}

void setPlaneModel(Flight* flight, char *id){
    flight->plane_model = malloc (sizeof(char) * (strlen(id) + 1));
    strcpy(flight->plane_model, id);
}

void setOrigin(Flight* flight, char *id){
    strcpy(flight->origin, id);
    str_upper(flight->origin);
}

void setDestination(Flight* flight, char *id){
    strcpy(flight->destination, id);
    str_upper(flight->destination);
}

void setScheduleDeparture(Flight *flight, char *str){
    DATE *new_date = strToDate(str, 1);
    flight->schedule_departure_date = new_date;
}

void setScheduleArrival(Flight *flight, char *str){
    DATE *new_date = strToDate(str, 1);
    flight->schedule_arrival_date = new_date;
}

void setRealDeparture(Flight *flight, char *str){
    DATE *new_date = strToDate(str, 1);
    flight->real_departure_date = new_date;
}

void setRealArrival(Flight *flight, char *str){
    DATE *new_date = strToDate(str, 1);
    flight->real_arrival_date = new_date;
}

//INIT / FREE
Flight* flight_init(){
    Flight *flight = malloc(sizeof(Flight));

    flight->airline = NULL;
    flight->plane_model = NULL;
    flight->schedule_arrival_date = NULL;
    flight->schedule_departure_date = NULL;
    flight->real_arrival_date = NULL;
    flight->real_departure_date = NULL;

    return flight;
}

void freeFlight(void *flight_pointer){
    Flight *flight = (Flight *)flight_pointer;
    free(flight->airline);
    free(flight->plane_model);
    free(flight->schedule_departure_date);
    free(flight->schedule_arrival_date);
    free(flight->real_departure_date);
    free(flight->real_arrival_date);

    free(flight);
}