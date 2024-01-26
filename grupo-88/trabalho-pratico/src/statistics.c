#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "statistics.h"
#include "date.h"
#include <glib.h>
#include <stdio.h>
#include "manageFlights.h"

struct data_statistics{
    GHashTable *n_passengers_flights;
    GHashTable *user_stats_hash;
    GHashTable *hotel_rating_hash;
    GHashTable *year_stats_hash;
    GSList *years_list;
    GSList *median_list;
    GSList *airport_ids;
    int is_sorted_year;
    int is_sorted_median;
    GSList *years_iter;
    GSList *median_iter;
};

struct user_stats{
    int n_reservations;
    int n_flights;
    double total_spent;
};

struct airport_stats{
    char *name;
    int passengers;
};

struct airport_median{
    char *name;
    int median;
};

struct hotel_stats{
    int total_rating;
    int n_reservations;
};

struct day_stats{
    int users;
    int flights;
    int passengers;
    int unique_passengers;
    int reservations;
    GHashTable *passengers_day_hash;
};

struct month_stats{
    int users;
    int flights;
    int passengers;
    int unique_passengers;
    int reservations;
    DAY_STATS **dayStats;
    GHashTable *passengers_month_hash;
};

struct year_stats{
    int users;
    int flights;
    int passengers;
    int unique_passengers;
    int reservations;
    MONTH_STATS *monthStats[12];
    GHashTable *passengers_year_hash;
    GSList *n_passenger_sorted_list;
    int is_sorted;
    GSList *n_passenger_iter;
};

DATA_STATISTICS *statistics_init(){
    DATA_STATISTICS *data = malloc(sizeof(DATA_STATISTICS));

    data->n_passengers_flights = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, free);
    data->user_stats_hash = g_hash_table_new_full(g_str_hash, g_str_equal, NULL, free);
    data->hotel_rating_hash = g_hash_table_new_full(g_int_hash, g_int_equal, free, free);
    data->year_stats_hash = g_hash_table_new_full(g_int_hash, g_int_equal, free, freeYearStats);
    data->years_list = NULL;
    data->median_list = NULL;
    data->airport_ids = NULL;
    data->is_sorted_year = 0;
    data->is_sorted_median = 0;
    data->years_iter = NULL;
    data->median_iter = NULL;

    return data;
}

DAY_STATS *initDayStats(){
    DAY_STATS *stats = malloc(sizeof(DAY_STATS));
    stats->users = 0;
    stats->flights = 0;
    stats->passengers = 0;
    stats->unique_passengers = 0;
    stats->reservations = 0;
    stats->passengers_day_hash = g_hash_table_new(g_str_hash, g_str_equal);

    return stats;
}

MONTH_STATS *initMonthStats(int month){
    MONTH_STATS *stats = malloc(sizeof(MONTH_STATS));
    stats->users = 0;
    stats->flights = 0;
    stats->passengers = 0;
    stats->unique_passengers = 0;
    stats->reservations = 0;
    stats->passengers_month_hash = g_hash_table_new(g_str_hash, g_str_equal);
    int n_days;
    if(month == 1){
        n_days = 29;
    }
    else{
        n_days = 31;
    }
    stats->dayStats = malloc(sizeof(DAY_STATS *) * n_days);
    for(int i = 0; i < n_days; i++){
        stats->dayStats[i] = NULL;
    }

    return stats;
}

void freeAirportStats(gpointer airport_stats_pointer){
    AIRPORT_STATS *stats = (AIRPORT_STATS *)airport_stats_pointer;
    free(stats->name);
    free(stats);
}

void freeMonthStats(MONTH_STATS *month_stats, int month){
    int n_days;
    if(month == 1){
        n_days = 29;
    }
    else{
        n_days = 31;
    }

    for(int i = 0; i < n_days; i++){
        if(month_stats->dayStats[i] != NULL) {
            g_hash_table_destroy(month_stats->dayStats[i]->passengers_day_hash);
            free(month_stats->dayStats[i]);
        }
    }
    g_hash_table_destroy(month_stats->passengers_month_hash);
    free(month_stats->dayStats);
}

void freeYearStats(void *year_stats_pointer){
    YEAR_STATS *year_stats = (YEAR_STATS *)year_stats_pointer;
    g_slist_free_full(g_steal_pointer(&year_stats->n_passenger_sorted_list), freeAirportStats);
    for(int i = 0; i < 12; i++){
        if(year_stats->monthStats[i] != NULL) {
            freeMonthStats(year_stats->monthStats[i], i);
            free(year_stats->monthStats[i]);
        }
    }
    g_hash_table_destroy(year_stats->passengers_year_hash);
    free(year_stats);
}

void freeStatistics(DATA_STATISTICS *data){
    g_hash_table_destroy(data->n_passengers_flights);
    g_hash_table_destroy(data->user_stats_hash);
    g_hash_table_destroy(data->hotel_rating_hash);
    g_hash_table_destroy(data->year_stats_hash);
    g_slist_free(g_steal_pointer(&data->years_list));
    g_slist_free_full(g_steal_pointer(&data->median_list), free);
    g_slist_free(g_steal_pointer(&data->airport_ids));

    free(data);
}

void incNPassengers(DATA_STATISTICS *data, int *flight_id){
    int *current_value = g_hash_table_lookup(data->n_passengers_flights, flight_id);
    //aumentar o número de passageiros atual em 1 unidade
    if(current_value == NULL){
        int *value = malloc(sizeof(int));
        *value = 1;
        g_hash_table_insert(data->n_passengers_flights,  flight_id, value);
    }
    else{
        (*current_value)++;
    }
}

int incUserStats(DATA_STATISTICS *data, char *key, STAT_TO_UPDATE stat, double value){
    if (stat == FLIGHT){
        //aumentar o número de voos de um utilizador
        USER_STATS *current_stats = g_hash_table_lookup(data->user_stats_hash, key);
        if(current_stats == NULL){
            USER_STATS *new_stats = malloc(sizeof(USER_STATS));
            new_stats->n_flights = 1;
            new_stats->n_reservations = 0;
            new_stats->total_spent = 0;

            g_hash_table_insert(data->user_stats_hash, key, new_stats);
            return 0;
        }
        else{
            current_stats->n_flights++;
            return 1;
        }
    }
    else {
        //aumentar o número de reservas e o gasto de um utilizador
        USER_STATS *current_stats = g_hash_table_lookup(data->user_stats_hash, key);
        if(current_stats == NULL){
            USER_STATS *new_stats = malloc(sizeof(USER_STATS));
            new_stats->n_flights = 0;
            new_stats->n_reservations = 1;
            new_stats->total_spent = value;

            g_hash_table_insert(data->user_stats_hash, key, new_stats);
            return 0;
        }
        else{
            current_stats->n_reservations++;
            current_stats->total_spent += value;
            return 1;
        }
    }
}

void incHotelStats(DATA_STATISTICS *data, int *key, int rating){
    HOTEL_STATS *stats = g_hash_table_lookup(data->hotel_rating_hash, key);
    //aumentar o número de reservas e o rating de um hotel
    if(stats == NULL){
        HOTEL_STATS *new_stats = malloc(sizeof(HOTEL_STATS));
        new_stats->n_reservations = 1;
        new_stats->total_rating = rating;
        g_hash_table_insert(data->hotel_rating_hash, key, new_stats);
    }
    else{
        stats->n_reservations++;
        stats->total_rating += rating;
        free(key);
    }
}

void incAirportStats(DATA_STATISTICS *data, char *origin, char *destination, int year){
    YEAR_STATS *year_stats = g_hash_table_lookup(data->year_stats_hash, &year);
    //aumentar o núemro de passageiros de um aeroporto num dado ano
    if(year_stats == NULL){
        char *new_origin = strdup(origin);
        char *new_destination = strdup(destination);

        YEAR_STATS  *new_stats = malloc(sizeof(YEAR_STATS ));
        new_stats->n_passenger_sorted_list = NULL;
        new_stats->is_sorted = 0;
        new_stats->n_passenger_iter = NULL;

        AIRPORT_STATS *stats_origin = malloc(sizeof(AIRPORT_STATS));
        stats_origin->name = new_origin;
        stats_origin->passengers = 1;

        AIRPORT_STATS *stats_destination = malloc(sizeof(AIRPORT_STATS));
        stats_destination->name = new_destination;
        stats_destination->passengers = 1;

        new_stats->n_passenger_sorted_list = g_slist_prepend(new_stats->n_passenger_sorted_list,stats_origin);
        new_stats->n_passenger_sorted_list = g_slist_prepend(new_stats->n_passenger_sorted_list,stats_destination);

        int *new_year = malloc(sizeof(int));
        *new_year = year;
        g_hash_table_insert(data->year_stats_hash, new_year, new_stats);
    }
    else {
        int incOrigin = 0;
        int incDestination = 0;

        GSList *temp_list = year_stats->n_passenger_sorted_list;

        //percorrer a lista dos aeroportos até ao fim ou até ter aumentado o número de passageiros da origem e do destino
        while (temp_list != NULL) {
            AIRPORT_STATS *stats = (AIRPORT_STATS *) temp_list->data;
            if (strcmp(origin, stats->name) == 0) {
                stats->passengers++;
                incOrigin = 1;
            } else if (strcmp(destination, stats->name) == 0) {
                stats->passengers++;
                incDestination = 1;
            }

            if (incOrigin && incDestination) {
                break;
            }

            temp_list = temp_list->next;
        }

        //se a origem ou o destino não tiver sido aumentada, adicionar à lista
        if (!incOrigin) {
            char *new_origin = strdup(origin);
            AIRPORT_STATS *stats_origin = malloc(sizeof(AIRPORT_STATS));
            stats_origin->name = new_origin;
            stats_origin->passengers = 1;

            year_stats->n_passenger_sorted_list = g_slist_prepend(year_stats->n_passenger_sorted_list, stats_origin);
        }

        if (!incDestination) {
            char *new_destination = strdup(destination);
            AIRPORT_STATS *stats_destination = malloc(sizeof(AIRPORT_STATS));
            stats_destination->name = new_destination;
            stats_destination->passengers = 1;

            year_stats->n_passenger_sorted_list = g_slist_prepend(year_stats->n_passenger_sorted_list, stats_destination);
        }
    }
}

int incYearStats(DATA_STATISTICS *data_statistics, DATE *date, STAT_TO_UPDATE stat, char *passenger_id){
    int year = getYear(date);
    YEAR_STATS *year_stats = g_hash_table_lookup(data_statistics->year_stats_hash, &year);
    if(year_stats == NULL){
        //se ainda não existir registos para o dado ano, inicializar os dados
        YEAR_STATS *new_stats = malloc(sizeof(YEAR_STATS));
        new_stats->n_passenger_sorted_list = NULL;
        new_stats->is_sorted = 0;
        new_stats->users = 0;
        new_stats->flights = 0;
        new_stats->passengers = 0;
        new_stats->unique_passengers = 0;
        new_stats->reservations = 0;
        new_stats->passengers_year_hash = g_hash_table_new(g_str_hash, g_str_equal);

        //inicializar os dados so mês
        for(int i = 0; i < 12; i++){
            new_stats->monthStats[i] = NULL;
        }
        int month = getMonth(date) - 1;
        new_stats->monthStats[month] = initMonthStats(month);

        //inicializar os dados do dia
        int day = getDay(date) - 1;
        new_stats->monthStats[month]->dayStats[day] = initDayStats();

        //aumentar as estatísticas pretendidas no ano, mês e dia
        switch (stat) {
            case USER:
                new_stats->monthStats[month]->dayStats[day]->users = 1;
                new_stats->monthStats[month]->users = 1;
                new_stats->users = 1;
                break;
            case FLIGHT:
                new_stats->monthStats[month]->dayStats[day]->flights = 1;
                new_stats->monthStats[month]->flights = 1;
                new_stats->flights = 1;
                break;
            case PASSENGER:
                new_stats->monthStats[month]->dayStats[day]->passengers = 1;
                new_stats->monthStats[month]->dayStats[day]->unique_passengers = 1;
                new_stats->monthStats[month]->passengers = 1;
                new_stats->monthStats[month]->unique_passengers = 1;
                new_stats->passengers = 1;
                new_stats->unique_passengers = 1;

                g_hash_table_add(new_stats->passengers_year_hash, passenger_id);
                g_hash_table_add(new_stats->monthStats[month]->passengers_month_hash, passenger_id);
                g_hash_table_add(new_stats->monthStats[month]->dayStats[day]->passengers_day_hash, passenger_id);
                break;
            case RESERVATION:
                new_stats->monthStats[month]->dayStats[day]->reservations = 1;
                new_stats->monthStats[month]->reservations = 1;
                new_stats->reservations = 1;
                break;
        }


        int *key_year = malloc(sizeof(int));
        *key_year = year;
        g_hash_table_insert(data_statistics->year_stats_hash, key_year, new_stats);
        data_statistics->years_list = g_slist_prepend(data_statistics->years_list, key_year);
        return 0;
    }
    else{
        //se necessário inicializar os dados do mês e dia
        int month = getMonth(date) - 1;
        int day = getDay(date) - 1;
        if(year_stats->monthStats[month] == NULL){
            year_stats->monthStats[month] = initMonthStats(month);
        }

        if(year_stats->monthStats[month]->dayStats[day] == NULL){
            year_stats->monthStats[month]->dayStats[day] = initDayStats();
        }

        //aumentar as estatísticas pretendidas no ano, mês e dia
        switch (stat) {
            case USER:
                year_stats->monthStats[month]->dayStats[day]->users++;
                year_stats->monthStats[month]->users++;
                year_stats->users++;
                break;
            case FLIGHT:
                year_stats->monthStats[month]->dayStats[day]->flights++;
                year_stats->monthStats[month]->flights++;
                year_stats->flights++;
                break;
            case PASSENGER:
                year_stats->monthStats[month]->dayStats[day]->passengers++;
                year_stats->monthStats[month]->passengers++;
                year_stats->passengers++;

                //apenas aumentar o número de passsageiros únicos se o passageiro ainda não tiver sido adicionado
                gboolean exists_year = g_hash_table_contains(year_stats->passengers_year_hash, passenger_id);
                if(exists_year == FALSE) {
                    year_stats->monthStats[month]->dayStats[day]->unique_passengers++;
                    year_stats->monthStats[month]->unique_passengers++;
                    year_stats->unique_passengers++;
                    g_hash_table_add(year_stats->passengers_year_hash, passenger_id);
                    g_hash_table_add(year_stats->monthStats[month]->passengers_month_hash, passenger_id);
                    g_hash_table_add(year_stats->monthStats[month]->dayStats[day]->passengers_day_hash, passenger_id);
                }
                else{
                    gboolean exists_month = g_hash_table_contains(year_stats->monthStats[month]->passengers_month_hash, passenger_id);
                    if(exists_month == FALSE){
                        year_stats->monthStats[month]->dayStats[day]->unique_passengers++;
                        year_stats->monthStats[month]->unique_passengers++;
                        g_hash_table_add(year_stats->monthStats[month]->passengers_month_hash, passenger_id);
                        g_hash_table_add(year_stats->monthStats[month]->dayStats[day]->passengers_day_hash, passenger_id);
                    }
                    else{
                        gboolean exists_day = g_hash_table_contains(year_stats->monthStats[month]->dayStats[day]->passengers_day_hash, passenger_id);
                        if(exists_day == FALSE){
                            year_stats->monthStats[month]->dayStats[day]->unique_passengers++;
                            g_hash_table_add(year_stats->monthStats[month]->dayStats[day]->passengers_day_hash, passenger_id);
                        }
                    }
                }
                break;
            case RESERVATION:
                year_stats->monthStats[month]->dayStats[day]->reservations++;
                year_stats->monthStats[month]->reservations++;
                year_stats->reservations++;
                break;
        }
    }

    return 1;
}

void prependAirportId(DATA_STATISTICS *data, char *airport){
    data->airport_ids = g_slist_prepend(data->airport_ids, airport);
}

double getStatisticsValue(DATA_STATISTICS *data, char hash_table, char stat, void *key){
    if(hash_table == 'P'){
        //obter o número de passageiros de um voo
        int *flight_id = (int *)key;
        int *value = g_hash_table_lookup(data->n_passengers_flights, flight_id);

        if(value == NULL){
            return -1;
        }

        return *value;
    }
    else if(hash_table == 'U'){
        //obter uma estatística de um utilizador
        char *user_id = (char *)key;
        USER_STATS *stats = g_hash_table_lookup(data->user_stats_hash, user_id);
        if(stats == NULL){
            return -1;
        }

        //obter a estatístics pretendida
        switch (stat) {
            case 'F':
                return stats->n_flights;
            case 'R':
                return stats->n_reservations;
            case 'T':
                return stats->total_spent;
        }
    }
    return -1;
}

double getHotelRating(DATA_STATISTICS *data, int *hotel_id){
    HOTEL_STATS *hotel_stats = g_hash_table_lookup(data->hotel_rating_hash, hotel_id);
    if(hotel_stats == NULL){
        return -1;
    }

    //calcular o rating médio de um hotel
    double rating;
    rating = (double)hotel_stats->total_rating / (double)hotel_stats->n_reservations;

    return rating;
}

AIRPORT_STATS *getNextAirportStats(DATA_STATISTICS *data_statistics, int year){
    YEAR_STATS *year_stats = g_hash_table_lookup(data_statistics->year_stats_hash, &year);
    if(year_stats == NULL){
        return NULL;
    }

    if(year_stats->is_sorted == 0){
        year_stats->n_passenger_sorted_list = g_slist_sort(year_stats->n_passenger_sorted_list, compareAirportsByPassengers);
        year_stats->is_sorted = 1;
    }

    AIRPORT_STATS *stats;
    if(year_stats->n_passenger_iter == NULL){
        year_stats->n_passenger_iter = year_stats->n_passenger_sorted_list;
        stats = (AIRPORT_STATS *)year_stats->n_passenger_iter->data;
    }
    else{
        year_stats->n_passenger_iter = year_stats->n_passenger_iter->next;
        if(year_stats->n_passenger_iter == NULL){
            stats = NULL;
        }
        else{
            stats = (AIRPORT_STATS *)year_stats->n_passenger_iter->data;
        }
    }

    return stats;
}

void resetAirportStatsIter(DATA_STATISTICS *data_statistics, int year){
    YEAR_STATS *year_stats = g_hash_table_lookup(data_statistics->year_stats_hash, &year);
    if(year_stats != NULL){
        year_stats->n_passenger_iter = NULL;
    }
}

char *getAirportName(AIRPORT_STATS *stats){
    return strdup(stats->name);
}

int getNPassengers(AIRPORT_STATS *stats){
    return stats->passengers;
}

int getYearStats(DATA_STATISTICS *data, int year, int month, int day, int *users, int *flights, int *passengers, int *unique_passengers, int *reservations){
    YEAR_STATS *year_stats = g_hash_table_lookup(data->year_stats_hash, &year);

    if(year_stats == NULL){
        return 0;
    }

    if(month == -1 && day == -1){
        //só temos o ano, devolver os stats desse ano
        *users = year_stats->users;
        *flights = year_stats->flights;
        *passengers = year_stats->passengers;
        *unique_passengers = year_stats->unique_passengers;
        *reservations = year_stats->reservations;
    }
    else if(day == -1){
        //temos o mês, mas não o dia
        MONTH_STATS **month_stats = year_stats->monthStats;
        if(month_stats[month] == NULL){
            return 0;
        }

        *users = month_stats[month]->users;
        *flights = month_stats[month]->flights;
        *passengers = month_stats[month]->passengers;
        *unique_passengers = month_stats[month]->unique_passengers;
        *reservations = month_stats[month]->reservations;
    }
    else{
        //temos o dia
        MONTH_STATS **month_stats = year_stats->monthStats;
        if(month_stats[month] == NULL){
            return 0;
        }

        DAY_STATS **day_stats = month_stats[month]->dayStats;

        if(day_stats[day] == NULL){
            return 0;
        }

        *users = day_stats[day]->users;
        *flights = day_stats[day]->flights;
        *passengers = day_stats[day]->passengers;
        *unique_passengers = day_stats[day]->unique_passengers;
        *reservations = day_stats[day]->reservations;
    }

    return 1;
}

int getNextYear(DATA_STATISTICS *data){
    if(data->is_sorted_year == 0){
        data->years_list = g_slist_sort(data->years_list, compareYears);
        data->is_sorted_year = 1;
    }

    int year;
    if(data->years_iter == NULL){
        data->years_iter = data->years_list;
        year = *((int *)data->years_iter->data);
    }
    else{
        data->years_iter = data->years_iter->next;
        if(data->years_iter == NULL){
            return -1;
        }
        else{
            year = *((int *)data->years_iter->data);
        }
    }

    return year;
}

void resetMedianIter(DATA_STATISTICS *data_statistics){
    data_statistics->median_iter = NULL;
}

AIRPORT_MEDIAN *getNextMedian(DATA_STATISTICS *data_statistics, void *data_flights){
    if (data_statistics->is_sorted_median == 0){
        GSList *airport_list = data_statistics->airport_ids;
        while(airport_list != NULL) {
            AIRPORT_MEDIAN *new_median = malloc(sizeof(AIRPORT_MEDIAN));
            new_median->name = airport_list->data;

            int median = getMedianDelays(data_flights, (char *)airport_list->data);
            new_median->median = median;
            data_statistics->median_list = g_slist_prepend(data_statistics->median_list, new_median);
            airport_list = airport_list->next;
        }

        data_statistics->median_list = g_slist_sort(data_statistics->median_list, compareMedians);
        data_statistics->is_sorted_median = 1;
    }

    AIRPORT_MEDIAN *median;
    if(data_statistics->median_iter == NULL){
        data_statistics->median_iter = data_statistics->median_list;
        median = (AIRPORT_MEDIAN *)data_statistics->median_iter->data;
    }
    else{
        data_statistics->median_iter = data_statistics->median_iter->next;
        if(data_statistics->median_iter == NULL){
            median = NULL;
        }
        else{
            median = (AIRPORT_MEDIAN *)data_statistics->median_iter->data;
        }
    }

    return median;
}

char *getAirportNameMedian(AIRPORT_MEDIAN *airport_median){
    return strdup(airport_median->name);
}

int getMedian(AIRPORT_MEDIAN *airport_median){
    return airport_median->median;
}

int compareMedians(gconstpointer median1_pointer, gconstpointer median2_pointer){
    AIRPORT_MEDIAN *median1 = (AIRPORT_MEDIAN *)median1_pointer;
    AIRPORT_MEDIAN *median2 = (AIRPORT_MEDIAN *)median2_pointer;

    if(median1->median == median2->median){
        int compare_airport = strcmp(median1->name, median2->name);
        return compare_airport;
    }
    else if(median1->median > median2->median){
        return -1;
    }
    else{
        return 1;
    }
}

int compareYears(gconstpointer year1_pointer, gconstpointer year2_pointer){
    int *year1 = (int *)year1_pointer;
    int *year2 = (int *)year2_pointer;

    if(*year1 < *year2){
        return -1;
    }
    else {
        return *year1 != *year2;
    }
}

int compareAirportsByPassengers(gconstpointer airport1_pointer, gconstpointer airport2_pointer){
    AIRPORT_STATS *airportStats1 = (AIRPORT_STATS *)airport1_pointer;
    AIRPORT_STATS *airportStats2 = (AIRPORT_STATS *)airport2_pointer;

    if(airportStats1->passengers == airportStats2->passengers){
        return (strcmp(airportStats1->name, airportStats2->name));
    }
    else{
        return(airportStats2->passengers - airportStats1->passengers);
    }
}