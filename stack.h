#ifndef STACK_H
#define STACK_H

#include <stdbool.h>
#include "calc.h"

/**
 * Represents a node in a linked stack.
 *
 * @param value Character stored in the node.
 * @param next  Pointer to the next node in the stack.
 */
typedef struct Stack {
    char value;
    struct Stack *next;
} Stack;

/**
 * Creates an empty stack.
 *
 * @return A pointer to an empty Stack.
 * @note Exits with EXIT_FAILURE if memory allocation fails.
 */
Stack *StackCreateEmpty(void);

/**
 * Pushes a character onto the stack.
 *
 * @param stack Pointer to the stack; must not be NULL.
 * @param value Character to store.
 * @return the new pointer to the head stack.
 */
Stack * StackPush(Stack *stack, char value);

/**
 * Checks whether a stack is empty.
 *
 * @param stack Pointer to the stack to check; must not be NULL.
 * @return true if the stack is empty, otherwise false.
 */
bool StackIsEmpty(Stack *stack);

/**
 * Checks whether a stack pointer is NULL.
 *
 * @param stack Stack pointer to check; may be NULL.
 * @return true if the stack is NULL, otherwise false.
 */
bool StackIsNULL(Stack *stack);

/**
 * Removes the top node and returns the new stack head.
 *
 * @param stack Pointer to a non-empty stack.
 * @return The next node, or NULL if the removed node was the only node.
 * @note Exits with EXIT_FAILURE if the stack is NULL or empty.
 */
Stack *StackPop(Stack *stack);

/**
 * Gets the next node in the stack.
 *
 * @param stack Pointer to a stack node; must not be NULL.
 * @return A pointer to the next node, or NULL if there is no next node.
 */
Stack *StackNext(Stack *stack);

/**
 * Computes the number of characters in the stack.
 *
 * @param stack Pointer to the stack; must not be NULL.
 * @return Number of characters in the stack.
 */
uint StackSize(Stack *stack);

/**
 * Frees a stack and all of its nodes.
 *
 * @param stack Pointer to the stack to free; must not be NULL.
 */
void StackFree(Stack *stack);

/**
 * Gets the value stored in a stack node.
 *
 * @param stack Pointer to a stack node; must not be NULL.
 * @return The character stored in the node.
 */
char StackGetValue(Stack *stack);

/**
 * Prints each node in the stack.
 *
 * @param stack Pointer to the stack to print; must not be NULL.
 */
void StackPrint(Stack *stack);

#endif /* STACK_H */
