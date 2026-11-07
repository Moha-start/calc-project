#include <stdio.h>
#include "stack.h"

int main(void) {
    Stack *stack = StackCreateEmpty();
    stack = StackPush(stack, 'A');
    stack = StackPush(stack, 'B');

    printf("Stack contents:\n");
    StackPrint(stack);
    StackFree(stack);
    return 0;
}
