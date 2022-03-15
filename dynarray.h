#ifndef _STDIO_H_
#define _STDIO_H_
#include <stdio.h>
#endif

#define BLOCK_SIZE 100

struct dynarray {
    size_t data[BLOCK_SIZE];

    struct dynarray *next;
};

typedef struct dynarray DA;

extern size_t daGet(DA *arrayPtr, size_t index);
extern void daPut(DA *arrayPtr, size_t index, size_t data);
extern char daGetChar(DA *arrayPtr, size_t index);
extern void daPutChar(DA *arrayPtr, size_t index, char data);
