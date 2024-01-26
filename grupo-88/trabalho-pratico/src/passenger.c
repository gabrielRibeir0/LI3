#include "passenger.h"
#include <stdlib.h>
#include <string.h>

struct passenger{
    int flight_id;
    char* user_id;
};

//GETTERS
int getPassengerFlightId(Passenger* passenger){
    return passenger->flight_id;
}

char* getPassengerId(Passenger* passenger){
    return strdup(passenger->user_id);
}

//SETTERS
void setPassengerFlightId(Passenger *passenger, char *id){
    int value = (int) strtol(id, NULL, 10);
    passenger->flight_id = value;
}

void setPassengerId(Passenger *passenger, char *id){
    passenger->user_id = malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(passenger->user_id, id);
}

//INIT / FREE
Passenger *init_passenger(){
    Passenger *passenger = malloc(sizeof(Passenger));
    passenger->user_id = NULL;

    return passenger;
}

void freePassenger(void *passenger_pointer){
    Passenger *passenger = (Passenger *)passenger_pointer;
    free(passenger->user_id);

    free(passenger);
}