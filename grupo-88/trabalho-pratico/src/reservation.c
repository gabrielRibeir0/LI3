#include "reservation.h"
#include <stdlib.h>
#include <string.h>

struct reservation{
    int id;
    char* user_id;
    int hotel_id;
    int hotel_stars; 
    int city_tax;
    DATE* begin_date;
    DATE* end_date;
    int price_per_night;
    char includes_breakfast;
    int rating;
};

//GETTERS
int getReservationId(Reservation* reservation){
    return reservation->id;
}

char* getReservationUserId(Reservation* reservation){
    return strdup(reservation->user_id);
}

int getHotelId(Reservation* reservation){
    return reservation->hotel_id;
}

int getHotelStars(Reservation* reservation){
    return reservation->hotel_stars;
}

int getCityTax(Reservation* reservation){
    return reservation->city_tax;
}

int getPricePerNight(Reservation* reservation){
    return reservation->price_per_night;
}

char getIncludesBreakfast(Reservation* reservation){
    return reservation->includes_breakfast;
}

int getRating(Reservation* reservation){
    return reservation->rating;
}

DATE* getBeginDate(Reservation* reservation){
    DATE *new_date = createDate(getYear(reservation->begin_date), getMonth(reservation->begin_date), getDay(reservation->begin_date), 0, 0, 0);

    return new_date;
}

DATE* getEndDate(Reservation* reservation){
    DATE *new_date = createDate(getYear(reservation->end_date), getMonth(reservation->end_date), getDay(reservation->end_date), 0, 0, 0);

    return new_date;
}

//SETTERS
void setReservationId(Reservation *reservation, char *id){
    int value = (int) strtol(id+4, NULL, 10);
    reservation->id = value;
}

void setReservationUserId(Reservation *reservation, char *id){
    reservation->user_id = malloc(sizeof(char) * (strlen(id) + 1));
    strcpy(reservation->user_id, id);
}

void setHotelId(Reservation *reservation, char *id){
    int value = (int) strtol(id+3, NULL, 10);
    reservation->hotel_id = value;
}

void setHotelStars(Reservation *reservation, char* stars){
    int value = (int) strtol(stars, NULL, 10);
    reservation->hotel_stars = value;
}

void setCityTax(Reservation *reservation, char* tax){
    int value = (int) strtol(tax, NULL, 10);
    reservation->city_tax = value;
}

void setPricePerNight(Reservation *reservation, char* price){
    int value = (int) strtol(price, NULL, 10);
    reservation->price_per_night = value;
}

void setIncludesBreakfast(Reservation *reservation, char has_breakfast){
    reservation->includes_breakfast = has_breakfast;
}

void setRating(Reservation *reservation, char* rating){
    int value = (int) strtol(rating, NULL, 10);
    reservation->rating = value;
}

void setBeginDate(Reservation *reservation, char *str){
    DATE *new_date = strToDate(str, 0);

    reservation->begin_date = new_date;
}

void setEndDate(Reservation *reservation, char *str){
    DATE *new_date = strToDate(str, 0);

    reservation->end_date = new_date;
}

//INIT / FREE
Reservation *reservation_init(){
    Reservation *reservation = malloc(sizeof(Reservation));

    reservation->user_id = NULL;
    reservation->begin_date = NULL;
    reservation->end_date = NULL;

    return reservation;
}

void freeReservation(void *reservation_pointer){
    Reservation *reservation = (Reservation *)reservation_pointer;
    free(reservation->user_id);
    free(reservation->begin_date);
    free(reservation->end_date);

    free(reservation);
}