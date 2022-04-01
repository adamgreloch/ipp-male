/**
 * @file stack.h
 * @brief Implements a stack.
 */

#ifndef MALE_STACK_H
#define MALE_STACK_H

#include <stddef.h>

struct Node {
    size_t data;
    struct Node *next;
};
typedef struct Node Node;

struct Stack {
    size_t size;
    Node *top;
};
typedef struct Stack Stack;

/**
 * Initializes a stack.
 * @param s - Stack pointer.
 */
void init(Stack *s);

/**
 * Checks if s stack is empty.
 * @param s - Stack pointer.
 */
int isEmpty(Stack *s);

/**
 * Pushes an element on top of a stack.
 * @param s - Stack pointer.
 * @param value - Value of pushed element.
 */
void push(Stack *s, size_t value);

/**
 * Removes top element from stack. Assumes stack is non-empty.
 * @param s - Stack pointer.
 * @return Value of removed element.
 */
size_t pop(Stack *s);

/**
 * Empties stack and frees its memory.
 * @param s - Stack pointer.
 */
void empty(Stack *s);

#endif //MALE_STACK_H
