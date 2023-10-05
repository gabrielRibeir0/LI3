#include <stdio.h>
#include <stdlib.h>
#include "command_parser.h"
#include "../guiao1/deque.h"
#include "helper_functions.h"

int main(int argc, char **argv){
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
    push(deque, randomInt());
    push(deque, randomInt());
    push(deque, randomInt());

    printf("Initial deque: ");
    printDeque(deque, &printInt);
    printf("\n");


    char* line = NULL;
    __ssize_t read;
    __ssize_t len;
    while((read = getline(&line, &len, fp)) != 1){
        Cmd* cmd = parseLine(line);
        printf("%s = ", cmd->command);
        processCommand(deque, cmd);
        printDeque(deque, &printInt);
        printf("\n");
    }

    return 0;
}