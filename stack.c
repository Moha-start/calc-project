#include <stdio.h>
#include <stdlib.h>
#include "stack.h"

Stack *StackCreateEmpty(void) {
    Stack *stack = malloc(sizeof(Stack));
    if (stack == NULL) {
        fprintf(stderr, "Not enough memory\n");
        exit(EXIT_FAILURE);
    }
    stack->value = '\0';
    stack->next = NULL;
    return stack;
}

Stack * StackPush(Stack *stack, char value) {
    if (stack == NULL) {
        fprintf(stderr, "NULL stack pointer in StackPush\n");
        exit(EXIT_FAILURE);
    }    
    if (StackIsEmpty(stack)) {
        stack->value = value;
        return stack;
    } 
    Stack *new_node = StackCreateEmpty();
    new_node->value = value;
    new_node->next = stack;
    
    return new_node;
}

bool StackIsNULL(Stack *stack){
    return stack==NULL;
}
bool StackIsEmpty(Stack *stack) {
    if (stack == NULL) {
        fprintf(stderr, "NULL stack pointer in StackIsEmpty \n");
    }
    return stack->value == '\0';
}

Stack *StackPop(Stack *stack) {
    if (stack == NULL) {
        fprintf(stderr, "NULL stack pointer\n");
        exit(EXIT_FAILURE);
    }
    if (StackIsEmpty(stack)) {
        fprintf(stderr, "Cannot pop from an empty stack\n");
        exit(EXIT_FAILURE);
    }    
    Stack *new_head = stack->next;
    free(stack);
    if(new_head==NULL){
        new_head=StackCreateEmpty();
    }
    return new_head;
}

Stack *StackNext(Stack *stack) {
    if (stack == NULL) {
        fprintf(stderr, "NULL stack pointer\n");
        exit(EXIT_FAILURE);
    }
    return stack->next;
}

uint StackSize(Stack *stack) {
    if (stack == NULL) {
        fprintf(stderr, "NULL stack pointer\n");
        exit(EXIT_FAILURE);
    }
    Stack *current = stack;
    uint size = 0;
    while (!StackIsEmpty(current)) {
        size++;
        current = StackNext(current);
    }
    return size;
}

void StackFree(Stack *stack) {
    while (stack != NULL) {
        Stack *next = stack->next;
        free(stack);
        stack = next;
    }
}

char StackGetValue(Stack *stack) {
    if (stack == NULL) {
        fprintf(stderr, "NULL stack pointer\n");
        exit(EXIT_FAILURE);
    }
    return stack->value;
}

void StackPrint(Stack *stack){
    if (stack == NULL) {
        return ;
    }
    printf("| %c |\n",stack->value);
    Stack * next = StackNext(stack);
    if(next!=NULL){
        StackPrint(next);
    }
}

