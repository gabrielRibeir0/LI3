#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "string.h"
#include "stdio.h"
#include "stdlib.h"
#include "user.h"
#include "flight.h"
#include "reservation.h"
#include "passenger.h"
#include "manageFlights.h"
#include "managePassengers.h"
#include "manageUsers.h"
#include "manageReservations.h"

void parseVoos(const char *filePath, DATA_FLIGHTS *data_flights, DATA_STATISTICS *data_statistics) {
    FILE *arquivo = fopen(filePath, "r"); //r significa que abre no modo leitura
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo 'flights.csv'.\n");
        exit(1);
    }

    char *linha = NULL;
    size_t tlinha;
    int line_counter = 0;
    int valid_parameter, num_parameter;
    int header_written = 0;

    while ((getline(&linha, &tlinha, arquivo)) != -1) {
        if(line_counter == 0){
            line_counter++;
            continue;
        }

        Flight *flight = flight_init();
        valid_parameter = num_parameter = 1;
        char *original_line = linha;
        char *token = strsep(&linha, ";");
        while (token != NULL) {
            valid_parameter = validateAndSetFlightParameter(flight, token, num_parameter);

            if(linha != NULL) {
                *(linha - 1) = ';';
            }

            if(valid_parameter){
                token = strsep(&linha, ";");
            }
            else{
                token = NULL;
                linha = NULL;
            }
            num_parameter++;
        }

        processFlight(data_flights, data_statistics, flight, valid_parameter, &header_written, original_line);

        free(original_line);
    }

    if (linha != NULL) {
        free(linha);
    }

    fclose(arquivo);
}

void parsePassageiros(const char *filePath, DATA_FLIGHTS *data_flights, DATA_USERS *data_users, DATA_STATISTICS *data_statistics) {
    FILE *arquivo = fopen(filePath, "r");
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo 'passengers.csv'.\n");
        exit(1);
    }

    char *linha = NULL;
    size_t tlinha;
    int valid_parameter, num_parameter, inactive_user;
    int header_written = 0;
    int line_counter = 0;
    while ((getline(&linha, &tlinha, arquivo)) != -1) {
        //ignorar primeira linha do ficheiro
        if(line_counter == 0){
            line_counter++;
            continue;
        }

        Passenger *passenger = init_passenger();
        valid_parameter = num_parameter = 1;
        inactive_user = 0;
        char *original_line = linha;
        char *token = strsep(&linha, ";");
        while (token != NULL) {
            valid_parameter = validateAndSetPassengerParameter(passenger, token, num_parameter, data_flights, data_users);

            if(linha != NULL) {
                *(linha - 1) = ';';
            }

            if(valid_parameter == 1){
                token = strsep(&linha, ";");
            }
            else if(valid_parameter == 2){
                token = strsep(&linha, ";");
                inactive_user = 1;
            }
            else{
                token = NULL;
                linha = NULL;
            }
            num_parameter++;
        }

        processPassenger(data_users, data_flights, data_statistics, passenger, valid_parameter, inactive_user, &header_written, original_line);

        free(original_line);
        freePassenger(passenger);
    }

    if (linha != NULL) {
        free(linha);
    }

    fclose(arquivo);
}

void parseReservas(const char *filePath, DATA_RESERVATIONS *data_reservations, DATA_USERS *data_users, DATA_STATISTICS *data_statistics) {
    FILE *arquivo = fopen(filePath, "r"); //r significa que abre no modo leitura
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo 'reservations.csv'.\n");
        exit(1);
    }

    char *linha = NULL;
    size_t tlinha;
    int line_counter = 0;
    int valid_parameter, num_parameter, inactive_user;
    int header_written = 0;

    while ((getline(&linha, &tlinha, arquivo)) != -1) {
        if(line_counter == 0){
            line_counter++;
            continue;
        }

        Reservation *reservation = reservation_init();
        valid_parameter = num_parameter = 1;
        inactive_user = 0;
        char *original_line = linha;
        char *token = strsep(&linha, ";");
        while (token != NULL) {
            valid_parameter = validateAndSetReservationParameter(reservation, token, num_parameter, data_users, data_reservations);

            if(linha != NULL) {
                *(linha - 1) = ';';
            }

            if(valid_parameter == 1){
                token = strsep(&linha, ";");
            }
            else if (valid_parameter == 2){
                token = strsep(&linha, ";");
                inactive_user = 1;
            }
            else{
                token = NULL;
                linha = NULL;
            }

            num_parameter++;
        }

        processReservation(data_reservations, data_users, data_statistics, reservation, valid_parameter, inactive_user, &header_written, original_line);

        free(original_line);
    }

    if (linha != NULL) {
        free(linha);
    }

    fclose(arquivo);
}

void parseUtilizadores(const char *filePath, DATA_USERS *data_users, DATA_STATISTICS *data_statistics) {
    FILE *arquivo = fopen(filePath, "r"); //r significa que abre no modo leitura
    if (arquivo == NULL) {
        fprintf(stderr, "Erro ao abrir o arquivo 'users.csv'.\n");
        exit(1);
    }

    char *linha = NULL;
    size_t tlinha;
    int line_counter = 0;
    int valid_parameter, num_parameter;
    int header_written = 0;
    while ((getline(&linha, &tlinha, arquivo)) != -1) {
        if(line_counter == 0){
            line_counter++;
            continue;
        }

        User * user = user_init();
        valid_parameter = num_parameter = 1;
        char *original_line = linha;
        char *token = strsep(&linha, ";");

        while (token != NULL) {
            valid_parameter = validateAndSetUserParameter(user, token, num_parameter);
            if(linha != NULL) {
                *(linha - 1) = ';';
            }

            if(valid_parameter){
                token = strsep(&linha, ";");
            }
            else{
                token = NULL;
                linha = NULL;
            }
            num_parameter++;
        }

        processUser(data_users, data_statistics, user, valid_parameter, &header_written, original_line);

        free(original_line);
    }

    if (linha != NULL) {
        free(linha);
    }

    fclose(arquivo);
}

void openDirectory(const char *folderPath, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_USERS *data_users, DATA_STATISTICS *data_statistics) {
    char *filepath;
    int size;

    size = asprintf(&filepath, "%s/%s", folderPath, "users.csv");
    if(size == -1 || access(filepath, F_OK) == -1){
        fputs("Error: The file 'users.csv' doesn't exist.\n", stderr);
        exit(1);
    }
    parseUtilizadores(filepath, data_users, data_statistics);
    free(filepath);

    size = asprintf(&filepath, "%s/%s", folderPath, "reservations.csv");
    if(size == -1 || access(filepath, F_OK) == -1){
        fputs("Error: The file 'reservations.csv' doesn't exist.\n", stderr);
        exit(1);
    }
    parseReservas(filepath, data_reservations, data_users, data_statistics);
    free(filepath);

    size = asprintf(&filepath, "%s/%s", folderPath, "flights.csv");
    if(size == -1 || access(filepath, F_OK) == -1){
        fputs("Error: The file 'flights.csv' doesn't exist.\n", stderr);
        exit(1);
    }
    parseVoos(filepath, data_flights, data_statistics);
    free(filepath);

    size = asprintf(&filepath, "%s/%s", folderPath, "passengers.csv");
    if(size == -1 || access(filepath, F_OK) == -1){
        fputs("Error: The file 'passengers.csv' doesn't exist.\n", stderr);
        exit(1);
    }
    parsePassageiros(filepath, data_flights,data_users, data_statistics);
    free(filepath);
}