#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "output.h"
#include <stdio.h>
#include <stdlib.h>

int addToErrorsFile(char *filepath, char *header, char *line, int header_written){
    FILE*  errors_file = fopen(filepath, "a");

    if(errors_file == NULL){
        perror("Error");
        return 1;
    }
    int write, write2;
    write = 1;

    if(!header_written){
        write = fprintf(errors_file,"%s", header);
    }
    write2 = fprintf(errors_file, "%s", line);

    fclose(errors_file);

    if(write < 0 || write2 < 0){
        perror("Error");
        return 1;
    }

    return 0;
}

int addToOutputFile(char **fields, int fields_num, char input_type, int query_id, int flag, int cmd_number, int line_number){
    char *filepath;
    int size;
    size = asprintf(&filepath, "./Resultados/command%d_output.txt", cmd_number);

    if(size == -1){
        perror("Error");
        return 1;
    }

    FILE *output_file = fopen(filepath, "a");

    if(output_file == NULL){
        free(filepath);
        perror("Error");
        return 1;
    }

    //se não há campos para escrever fica só o ficheiro vazio
    if(fields_num == 0){
        free(filepath);
        fclose(output_file);
        return 0;
    }

    //modo de output com flag
    if(flag){
        if(line_number > 1){
            fprintf(output_file,"\n");
        }

        //imprimir os campos no formato em função da query que for
        switch(query_id) {
            case 1:
                switch (input_type) {
                    case 'U':
                        fprintf(output_file, "--- 1 ---\nname: %s\nsex: %s\nage: %s\ncountry_code: %s\npassport: %s\nnumber_of_flights: %s\nnumber_of_reservations: %s\ntotal_spent: %s\n",
                                fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], fields[7]);
                        break;
                    case 'R':
                        fprintf(output_file, "--- 1 ---\nhotel_id: %s\nhotel_name: %s\nhotel_stars: %s\nbegin_date: %s\nend_date: %s\nincludes_breakfast: %s\nnights: %s\ntotal_price: %s\n",
                                fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], fields[7]);
                        break;
                    case 'F':
                        fprintf(output_file, "--- 1 ---\nairline: %s\nplane_model: %s\norigin: %s\ndestination: %s\nschedule_departure_date: %s\nschedule_arrival_date: %s\npassengers: %s\ndelay: %s\n",
                                fields[0], fields[1], fields[2], fields[3], fields[4], fields[5], fields[6], fields[7]);
                        break;
                }
                break;
            case 2:
                if(fields_num == 2) {
                    fprintf(output_file, "--- %d ---\nid: %s\ndate: %s\n", line_number,fields[0], fields[1]);
                }
                else if(fields_num == 3){
                    fprintf(output_file, "--- %d ---\nid: %s\ndate: %s\ntype: %s\n", line_number,fields[0], fields[1], fields[2]);
                }
                break;
            case 3:
                fprintf(output_file, "--- 1 ---\nrating: %s", fields[0]);
                break;
            case 4:
                fprintf(output_file, "--- %d ---\nid: %s\nbegin_date: %s\nend_date: %s\nuser_id: %s\nrating: %s\ntotal_price: %s\n",
                        line_number, fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]);
                break;
            case 5:
                fprintf(output_file, "--- %d ---\nid: %s\nschedule_departure_date: %s\ndestination: %s\nairline: %s\nplane_model: %s\n",
                        line_number, fields[0], fields[1], fields[2], fields[3], fields[4]);
                break;
            case 6:
                fprintf(output_file, "--- %d ---\nname: %s\npassengers: %s\n",
                        line_number, fields[0], fields[1]);
                break;
            case 7:
                fprintf(output_file, "--- %d ---\nname: %s\nmedian: %s\n",
                        line_number, fields[0], fields[1]);
                break;
            case 8:
                fprintf(output_file, "--- 1 ---\nrevenue: %s", fields[0]);
                break;
            case 9:
                fprintf(output_file, "--- %d ---\nid: %s\nname: %s\n",
                        line_number, fields[0], fields[1]);
                break;
            case 10:
                switch (input_type) {
                    case 'Y':
                        fprintf(output_file, "--- %d ---\nyear: %s\nusers: %s\nflights: %s\npassengers: %s\nunique_passengers: %s\nreservations: %s\n",
                                line_number, fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]);
                        break;
                    case 'M':
                        fprintf(output_file, "--- %d ---\nmonth: %s\nusers: %s\nflights: %s\npassengers: %s\nunique_passengers: %s\nreservations: %s\n",
                                line_number, fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]);
                        break;
                    case 'D':
                        fprintf(output_file, "--- %d ---\nday: %s\nusers: %s\nflights: %s\npassengers: %s\nunique_passengers: %s\nreservations: %s\n",
                                line_number, fields[0], fields[1], fields[2], fields[3], fields[4], fields[5]);
                        break;
                }
                break;
            default:
                break;
        }

        if(line_number == -1){
            fprintf(output_file, "\n");
        }
    }
    else{
        //modo sem flag
        if(fields_num > 1) {
            for (int i = 0; i < fields_num - 1; i++) {
                fprintf(output_file, "%s;", fields[i]);
            }
        }
        fprintf(output_file, "%s\n", fields[fields_num - 1]);
    }

    free(filepath);
    fclose(output_file);

    return 0;
}