#include <stdio.h>
#include "interpreter.h"
#include "interactive.h"
#include "parser.h"
#include "manageUsers.h"
#include "manageFlights.h"
#include "manageReservations.h"
#include "statistics.h"
#include "utility.h"
#include <locale.h>

int main(int argc, char** argv){
    setlocale(LC_COLLATE, "en_US.UTF-8");

    PROGRAM_MODE mode;
    if(argc == 1){
        //interactive mode tem 0 argumentos
        mode = INTERACTIVE;
    }
    else if(argc == 3){
        //batch mode tem 2 argumentos
        mode = BATCH;
    }
    else{
        fputs("Wrong number of arguments!\n", stderr);
        return 1;
    }

    DATA_USERS *data_users = data_users_init();
    DATA_FLIGHTS *data_flights = data_flights_init();
    DATA_RESERVATIONS *data_reservations = data_reservations_init();
    DATA_STATISTICS  *data_statistics = statistics_init();

    if(mode == BATCH) {
        const char* datasets_filepath = argv[1];
        const char* commands_filepath = argv[2];
        openDirectory(datasets_filepath, data_flights, data_reservations, data_users, data_statistics);
        process_commands(commands_filepath, data_users, data_flights, data_reservations, data_statistics);
    }
    else{
        initInteractiveMode(data_users, data_flights, data_reservations, data_statistics);
    }

    freeUsersData(data_users);
    freeFlightsData(data_flights);
    freeReservationsData(data_reservations);
    freeStatistics(data_statistics);
    return 0;
}