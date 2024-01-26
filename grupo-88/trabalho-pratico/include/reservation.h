#ifndef RESERVATIONS_H
#define RESERVATIONS_H

#include "date.h"

// Tipo de dados para representar uma reserva
typedef struct reservation Reservation;

// Getters para os vários parâmetros de uma reserva
int getReservationId(Reservation* reservation);
char* getReservationUserId(Reservation* reservation);
int getHotelId(Reservation* reservation);
int getHotelStars(Reservation* reservation);
int getCityTax(Reservation* reservation);
int getPricePerNight(Reservation* reservation);
char getIncludesBreakfast(Reservation* reservation);
int getRating(Reservation* reservation);
DATE* getBeginDate(Reservation* reservation);
DATE* getEndDate(Reservation* reservation);

// Setters para os vários parâmetros de uma reserva
void setReservationId(Reservation *reservation, char *id);
void setReservationUserId(Reservation *reservation, char *id);
void setHotelId(Reservation *reservation, char *id);
void setHotelStars(Reservation *reservation, char* stars);
void setCityTax(Reservation *reservation, char* tax);
void setPricePerNight(Reservation *reservation, char* price);
void setIncludesBreakfast(Reservation *reservation, char has_breakfast);
void setRating(Reservation *reservation, char* rating);
void setBeginDate(Reservation *reservation, char *str);
void setEndDate(Reservation *reservation, char *str);

// Função para inicializar uma reserva
Reservation *reservation_init();

// Função para dar free a uma reserva
void freeReservation(void *reservation_pointer);

#endif