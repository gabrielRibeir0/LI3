#ifndef _GNU_SOURCE
#define _GNU_SOURCE 1
#endif

#include "output_test.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

//códigos para cores
#define NORMAL "\x1B[0m"
#define RED "\x1B[31m"
#define GREEN "\x1B[32m"

void outputResources(double total_time, long int max_memory){
    printf("Tempo total: %.6lf s\n", total_time);
    printf("Memória Máxima Usada: %ld KB\n", max_memory);
}

void outputQueriesTime(double times[10]){
    for(int i = 0; i < 10; i++){
        printf("Tempo Execução Query %d: %.6lf s\n", i+1, times[i]);
    }
}

void compareOutputFiles(const char *outputs_filepath, const char *input_filepath){
    char *test_path;
    int size;
    size = asprintf(&test_path, "%s/command1_output.txt", outputs_filepath);
    //verificar se a pasta dos outputs existe
    if(access(test_path, F_OK) != 0 || size == -1){
        fputs("Error opening the output folder!", stderr);
        return;
    }
    free(test_path);

    //determinar qual o tamanhado do dataset usado
    int command_max;
    if (access("Resultados/command101_output.txt", F_OK) == 0) {
        command_max = 500;
    } else {
        command_max = 100;
    }

    puts("\nComparação outputs:");
    FILE *input_file = fopen(input_filepath, "r");
    char *solution_path;
    char *result_path;
    int input_line = 0;
    int errors = 0;
    for(int i = 1; i <= command_max; i++){
        size = asprintf(&solution_path, "%s/command%d_output.txt", outputs_filepath, i);
        if(size == -1){
            return;
        }
        size = asprintf(&result_path, "Resultados/command%d_output.txt", i);
        if(size == -1){
            return;
        }

        FILE *solution_file = fopen(solution_path, "r");
        FILE *result_file = fopen(result_path, "r");
        //se o ficheiro não existir, escrever a mensagem de erro correspondente
        if(result_file == NULL){
            size_t len;
            char *line = NULL;
            //determinar qual a query do ficheiro
            while(getline(&line, &len, input_file) != -1){
                input_line++;
                if(input_line == i){
                    break;
                }
                free(line);
            }
            printf("Comando: %d Query: %c%c", i, line[0], line[1]);
            //escrever corretamente a query em certos casos (10 / 10F)
            if(line[1] != ' '){
                if(line[2] == 'F') {
                    printf("F");
                }
                printf(" ");
            }
            printf("%sFicheiro não existe!%s\n", RED, NORMAL);
            free(line);
            errors++;
        }
        else {
            int has_error = 0;
            size_t len_sol, len_res;
            ssize_t read_sol, read_res;
            char *line_sol = NULL;
            char *line_res = NULL;
            //comparar linha a linha dos 2 outputs e verificar se há diferenças
            read_res = getline(&line_res, &len_res, result_file);
            read_sol = getline(&line_sol, &len_sol, solution_file);
            while(read_res != -1 && read_sol != -1 && !has_error) {
                if (strcmp(line_res, line_sol) != 0) {
                    has_error = 1;
                } else {
                    read_res = getline(&line_res, &len_res, result_file);
                    read_sol = getline(&line_sol, &len_sol, solution_file);
                }
            }

            free(line_sol);
            free(line_res);

            //se algum ficheiro acabou primeiro que o outro marcar como erro
            if(read_res * read_sol < 0){
                has_error = 1;
            }

            size_t len;
            char *line = NULL;
            while(getline(&line, &len, input_file) != -1){
                input_line++;
                if(input_line == i){
                    break;
                }
                free(line);
            }
            //escrever mensagem de erro ou sucesso
            if(has_error){
                printf("Comando: %d Query: %c%c", i, line[0], line[1]);
                //escrever corretamente a query em certos casos (10 / 10F)
                if(line[1] != ' '){
                    if(line[2] == 'F') {
                        printf("F");
                    }
                    printf(" ");
                }
                printf("%sFicheiros com conteúdos diferentes!%s\n", RED, NORMAL);
                errors++;
            }
            else{
                printf("Comando: %d Query: %c%c", i, line[0], line[1]);
                //escrever corretamente a query em certos casos (10 / 10F)
                if(line[1] != ' '){
                    if(line[2] == 'F') {
                        printf("F");
                    }
                    printf(" ");
                }
                printf("%sFicheiros iguais!%s\n", GREEN, NORMAL);
            }

            free(line);
            fclose(result_file);
        }

        fclose(solution_file);

        free(solution_path);
        free(result_path);
    }

    printf("O programa passou em %d de %d queries (%d erradas)!\n", command_max - errors, command_max, errors);

    fclose(input_file);
}