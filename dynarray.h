#include <stdio.h>
#include <stdlib.h>

#define BLOCK_SIZE 100

struct dynarray {
    size_t data[BLOCK_SIZE];

    struct dynarray *next;
};

typedef struct dynarray DA;

#define daInit(arrayPtr) {(arrayPtr) = malloc(sizeof(DA)); (arrayPtr)->next = NULL;}

extern size_t daGet(DA *arrayPtr, size_t index);
extern void daPut(DA *arrayPtr, size_t index, size_t data);
