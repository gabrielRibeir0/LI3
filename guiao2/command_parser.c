#include "command_parser.h"
#include "helper_functions.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

void processCommand(Deque* deque, Cmd* cmd){
    if(strcmp(cmd->command, "PUSH") == 0){
        for(int i = 0; i < cmd->nargs; i++){
            push(deque, &(cmd->args[i]));
        }
    }
    else if(strcmp(cmd->command, "PUSH_FRONT") == 0){
        for(int i = 0; i < cmd->nargs; i++){
            pushFront(deque, &(cmd->args[i]));
        }
    }
    else if(strcmp(cmd->command, "POP") == 0){
        pop(deque);
    }
    else if(strcmp(cmd->command, "POP_FRONT") == 0){
        popFront(deque);
    }
    else if(strcmp(cmd->command, "SIZE") == 0){
        size(deque);
    }
    else if(strcmp(cmd->command, "REVERSE") == 0){
        reverse(deque);
    }
    else if(strcmp(cmd->command, "PRINT") == 0){
        printDeque(deque, &printInt);
    }
    else{
        perror("Error: Unrecognized command");
        return;
    }
}

int getNargs(char* line){
    char* aux;
    strcpy(aux, line);
    int nArgs = 0;
    char* token = strsep(&aux, " ");
    while(token != NULL){
        nArgs++;
        token = strsep(&aux, " ");
    }

    return nArgs;
}

Cmd* parseLine(char* line){
    //create the command and read it
    Cmd* cmd;
    char* token = strsep(&line, " ");
    toUpperStr(token);
    printf("\nteste----3\n");
    strcpy(cmd->command, token);
    printf("\nteste1\n");

    //if the comment is PUSH or PUSH_FRONT, we need to read the arguments
    if(strcmp(cmd->command, "PUSH") == 0 || strcmp(cmd->command,"PUSH_FRONT") == 0){
        int read_count = 0; //number of arguments read
        //get the number of arguments
        cmd->nargs = getNargs(line);
        //read and store the arguments
        int* stored_args = malloc(cmd->nargs * sizeof(int));
        token = strsep(&line, " ");
        for(int i = 0; i < cmd->nargs; i++){
            stored_args[0] = atoi(token);
            read_count++;
            token = strsep(&line, " ");
        }

        //if the number of arguments read is not correct, return an error
        if(read_count != cmd->nargs){
            perror("Error");
            return NULL;
        }
        cmd->args = stored_args;
    }
    //if not, there are no arguments
    else{
        cmd->nargs = 0;
        cmd->args = NULL;
    }

    return cmd;
}