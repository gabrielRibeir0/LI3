#include "command_parser.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

void processCommand(Deque* deque, Cmd* cmd){

}

Cmd* parseLine(char* line){
    //create the command and read it
    Cmd* cmd;
    char* token = strsep(&line, " ");
    strcpy(cmd->command, toupper(token));

    //if the comment is PUSH or PUSH_FRONT, we need to read the arguments
    if(cmd->command == "PUSH" || cmd->command == "PUSH_FRONT"){
        int read_count = 0; //number of arguments read
        token = strsep(&line, " "); //get the first parameter, which is the number of arguments
        cmd->nargs = atoi(token);
        //read and store the arguments
        int* stored_args = malloc(cmd->nargs * sizeof(int));
        token = strsep(&line, " ");
        while(token != NULL || read_count == cmd->nargs){
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