#include <stdio.h>
#include "interpretertestes.h"
#include "parser.h"
#include "manageUsers.h"
#include "manageFlights.h"
#include "manageReservations.h"
#include "statistics.h"
#include "output_test.h"
#include <sys/resource.h>
#include <locale.h>

int main(int argc, char** argv){
    struct rusage r_usage;
    clock_t t_start, t_end;
    setlocale(LC_COLLATE, "en_US.UTF-8");
    t_start = clock();

    //batch mode only has 2 arguments (argc == 3)
    if(argc != 4){
        fputs("Wrong number of arguments!\n", stderr);
        return 1;
    }

    DATA_USERS *data_users = data_users_init();
    DATA_FLIGHTS *data_flights = data_flights_init();
    DATA_RESERVATIONS *data_reservations = data_reservations_init();
    DATA_STATISTICS  *data_statistics = statistics_init();

    const char* datasets_filepath = argv[1];
    const char* commands_filepath = argv[2];
    const char* outputs_filepath = argv[3];

    openDirectory(datasets_filepath, data_flights, data_reservations, data_users, data_statistics);
    process_commandsT(commands_filepath, data_users, data_flights, data_reservations, data_statistics);

    freeUsersData(data_users);
    freeFlightsData(data_flights);
    freeReservationsData(data_reservations);
    freeStatistics(data_statistics);
    t_end = clock();
    getrusage(RUSAGE_SELF, &r_usage);

    outputResources(((double)(t_end - t_start))/CLOCKS_PER_SEC, r_usage.ru_maxrss);
    compareOutputFiles(outputs_filepath, commands_filepath);
    return 0;
}