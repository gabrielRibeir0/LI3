#include "deque.h"
#include <stdlib.h>
#include <stdio.h>

Deque* create(){
    Deque* deque = malloc(sizeof(Deque));
    deque->size = 0;
    deque->back = NULL;
    deque->front = NULL;

    return deque;
}

void push(Deque* deque, void* data){
    Node* node = nodeCreate(data);
    node->next = NULL;
    node->prev = deque->back;
    
    if(deque->size == 0){
        deque->front = node;
    }
    else{
        deque->back->next = node;
    }
    
    deque->back = node;
    deque->size++;
}

void pushFront(Deque* deque, void* data){ 
    Node* node = nodeCreate(data);
    node->next = deque->front;
    node->prev = NULL;
    
    if(deque->size == 0){
        deque->back = node;
    }
    else{
        deque->front->prev = node;
    }  
     
    deque->front = node;
    deque->size++;
}

void* pop(Deque* deque){
    if(deque->size == 0){
        printf("EMPTY\n");
        return NULL;
    }

    Node* aux = deque->back;
    void* dataAux = aux->data;
    if(deque->size == 1){
        deque->front = NULL;
        deque->back = NULL;
    }
    else{
        deque->back = aux->prev;
        deque->back->next = NULL;
    }

    free(aux);
    deque->size--;

    return dataAux;
}

void* popFront(Deque* deque){
    if(deque->size == 0){
        printf("EMPTY\n");
        return NULL;
    }

    Node* aux = deque->front;
    void* dataAux = aux->data;

    if(deque->size == 1){
        deque->front = NULL;
        deque->back = NULL;
    }
    else{
        deque->front = aux->next;
        deque->front->prev = NULL;
    }

    free(aux);
    deque->size--;

    return dataAux;
}

int size(Deque* deque){
    return deque->size;
}

bool isEmpty(Deque* deque){
    return deque->size == 0;
}

void reverse(Deque* deque){
    Node* current = deque->front;
    Node* aux;
    
    while(current != NULL){
        aux = current->next;
        current->next = current->prev;
        current->prev = aux;
        current = aux;
    }

    aux = deque->front;
    deque->front = deque->back;
    deque->back = aux;
}

void printDeque(Deque* deque, void(*printFunc)(void*)){
    Node* aux = deque->front;
    printf("[");
    while(aux != NULL){
        printFunc(aux->data);
        aux = aux->next;
        if(aux != NULL){
            printf(" -> ");
        }
    }
    printf("]\n");
}

void destroy(Deque* deque){
    while(deque->size != 0){
        pop(deque);
    }

    free(deque);
}
