#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "command_parser.h"
#include "../guiao1/deque.h"
#include "helper_functions.h"

int main(int argc, char **argv){
    srand(time(NULL));
    if(argc < 2){
        perror("Error: Missing filename\n");
        return 1;
    }

    char* filename = argv[1];
    FILE* fp = fopen(filename, "r");

    if(!fp){
        perror("Error:");
        return 2;
    }

    Deque* deque = create();
    /*push(deque, randomInt());
    push(deque, randomInt());
    push(deque, randomInt());*/

    Cmd* cmd = malloc(sizeof(Cmd));
    char* line = NULL;
    __ssize_t read;
    __ssize_t len;
    while((read = getline(&line, &len, fp)) != -1){
        cmd = parseLine(line);
        processCommand(deque, cmd);
    }

    free(cmd->command);
    free(cmd->args);
    free(cmd);
    free(line);
    free(deque->front);
    free(deque->back);
    free(deque);
    fclose(fp);

    return 0;
}