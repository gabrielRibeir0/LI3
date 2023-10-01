#ifndef COMMAND_PARSER_H
#define COMMAND_PARSER_H

#include "../guiao1/deque.h"

typedef struct cmd{
    char* command;
    int* args;
    int nargs;
} Cmd;

void processCommand(Deque* deque, Cmd* cmd);

Cmd* parseLine(char* line);
#endif