#ifndef MALE_DYNARRAY_H
#define MALE_DYNARRAY_H

#include <stdio.h>
#include <stdint.h>

#define BLOCK_SIZE 100

struct dynarray {
    size_t data[BLOCK_SIZE];

    struct dynarray *next;
};

typedef struct dynarray DA;

extern size_t daGet(DA *arrayPtr, size_t index);

extern void daPut(DA *arrayPtr, size_t index, size_t data);

void daBut8Bit(DA *arrayPtr, size_t index, uint8_t data);

uint8_t daGet8Bit(DA *arrayPtr, size_t index);

#endif //MALE_DYNARRAY_H
