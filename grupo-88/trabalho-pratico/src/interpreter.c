#include "interpreter.h"
#include "queries.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct command{
    int query_id;
    int flag;
    int nargs;
    char **args;
};

int get_n_args(char* line){
    if(line == NULL){
        return 0;
    }

    char* aux;
    char* orig;
    orig = aux = strdup(line);

    int nArgs = 0;
    char* token = strsep(&aux, " \"");
    while(token != NULL){
        //só contar o argumento se não for uma string vazia
        if((strcmp(token, "") != 0) && token[0] != '\n'){
            nArgs++;
        }
        token = strsep(&aux, " \"");
    }

    free(orig);
    return nArgs;
}

Cmd* parse_command(char* line){
    Cmd* cmd = malloc(sizeof(Cmd));

    char* token = strsep(&line, " \"");

    if(!token){
        return NULL;
    }

    int token_len = strlen(token);
    if(token[token_len - 1] == 'F'){
        cmd->flag = 1;
        token[token_len - 1] = '\0';
    }
    else if(token[token_len - 1] == '\n'){
        if(token[token_len - 2] == 'F'){
            cmd->flag = 1;
        }
        else{
            cmd->flag = 0;
        }
    }
    else{
        cmd->flag = 0;
    }

    cmd->query_id = atoi(token);
    cmd->nargs = get_n_args(line);
    cmd->args = malloc(sizeof(char *) * cmd->nargs);
    token = strsep(&line, " \"");

    int i = 0;
    while(i < cmd->nargs){
        //guardar se o token não for vazio
        if(strcmp(token,"") != 0){
            cmd->args[i] = malloc(sizeof(char) * (strlen(token) + 1));
            strcpy(cmd->args[i], token);

            //remover o '\n' final se for o último argumento
            if(cmd->args[i][strlen(cmd->args[i]) -1] == '\n'){
                cmd->args[i][strlen(cmd->args[i]) - 1] = '\0';
            }

            i++;
        }
        token = strsep(&line, " \"");
    }

    return cmd;
}

void execute_command(Cmd* command, int command_number, DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics, PROGRAM_MODE mode){
     query_manager(data_flights, data_reservations, data_users, data_statistics, command->query_id, command->nargs, command->args, command->flag, command_number, mode);
}

void free_command(Cmd* cmd){
    for(int i = 0; i < cmd->nargs; i++){
        free(cmd->args[i]);
    }
    free(cmd->args);
    free(cmd);
}

void process_commands(const char *filepath, DATA_USERS *data_users, DATA_FLIGHTS *data_flights, DATA_RESERVATIONS *data_reservations, DATA_STATISTICS *data_statistics){
    FILE* fp = fopen(filepath, "r");

    if(!fp){
        perror("Error");
        exit(1);
    }
    char* line = NULL;
    size_t len;
    Cmd* cmd;
    int command_number = 1;
    while(getline(&line, &len, fp) != -1) {
        cmd = parse_command(line);
        execute_command(cmd, command_number, data_users, data_flights, data_reservations, data_statistics, BATCH);
        command_number++;
        free_command(cmd);
    }

    free(line);
    fclose(fp);
}