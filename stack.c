#include <stdlib.h>
#include "stack.h"

void init(Stack *s) {
    s->size = 0;
    s->top = NULL;
}

int isEmpty(Stack *s) {
    return (s->size <= 0);
}

void push(Stack *s, size_t value) {
    Node *new = malloc(sizeof(Node));
    new->data = value;
    new->next = NULL;

    if (!(s->top))
        new->next = NULL;
    else
        new->next = s->top;

    s->top = new;
    s->size++;
}

size_t pop(Stack *s) {
    Node *tmp = s->top;
    size_t popped = s->top->data;
    s->top = s->top->next;
    s->size--;

    free(tmp);
    return popped;
}

void empty(Stack *s) {
    while (!isEmpty(s))
        pop(s);
    free(s);
}

