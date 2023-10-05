#include "command_parser.h"
#include "helper_functions.h"
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void processCommand(Deque* deque, Cmd* cmd){
    int len = strlen(cmd->command);
    if(cmd->command[len-1] == '\n')
        cmd->command[len-1] = '\0';
    
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
        printf("%d\n", size(deque));
    }
    else if(strcmp(cmd->command, "REVERSE") == 0){
        reverse(deque);
    }
    else if(strcmp(cmd->command, "PRINT") == 0){
        printDeque(deque, &printInt);
    }
    else{
        printf("Error: Unrecognized command");
        return;
    }
}

int getNargs(char* line){
    char* aux;
    aux = strdup(line);
    int nArgs = 0;
    char* token = strsep(&aux, " ");
    while(token != NULL){
        nArgs++;
        token = strsep(&aux, " ");
    }
    free(aux);

    return nArgs;
}

Cmd* parseLine(char* line){
    //create the command and read it
    Cmd* cmd = malloc(sizeof(Cmd));
    char* token = strsep(&line, " ");
    if(token == NULL)
        return NULL;

    toUpperStr(token);
    cmd->command = strdup(token); //use strdup() to also allocate the memory

    //if the comment is PUSH or PUSH_FRONT, we need to read the arguments
    if((strcmp(cmd->command, "PUSH") == 0) || (strcmp(cmd->command,"PUSH_FRONT") == 0)){
        //get the number of arguments
        cmd->nargs = getNargs(line);
        //read and store the arguments
        cmd->args = malloc(cmd->nargs * sizeof(int));
        token = strsep(&line, " ");
        for(int i = 0; i < cmd->nargs; i++){
            cmd->args[i] = atoi(token);
            token = strsep(&line, " ");
        }
    }
    //if not, there are no arguments
    else{
        cmd->nargs = 0;
        cmd->args = NULL;
    }

    return cmd;
}