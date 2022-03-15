#include "dynarray.h"

// TODO ogarnąć
// TODO dokumentacja

static DA* daFind(DA *arrayPtr, size_t index, size_t *currentIndexPtr) {
    DA *currentPtr;

    currentPtr = arrayPtr;
    *currentIndexPtr = index;

    while (*currentIndexPtr >= BLOCK_SIZE) {
        if (currentPtr->next == NULL) {
            currentPtr->next = malloc(sizeof(DA));

            if (currentPtr->next == NULL) {
                // error: not enough memory to expand further
                fprintf(stderr, "ERROR 0\n");
                exit(1);
            }

            memset(currentPtr->next, '\0', sizeof(DA));
        }
        currentPtr = currentPtr->next;
        *currentIndexPtr -= BLOCK_SIZE;
    }
    return currentPtr;
}

void daPut(DA *arrayPtr, size_t index, size_t data) {
    DA *currentPtr;
    size_t currentIndex;

    currentPtr = daFind(arrayPtr, index, &currentIndex);
    currentPtr->data[currentIndex] = data;
}

size_t daGet(DA *arrayPtr, size_t index) {
    DA *currentPtr;

    size_t currentIndex;

    currentPtr = daFind(arrayPtr, index, &currentIndex);
    return currentPtr->data[currentIndex];
}

void daPutBinary(DA *arrayPtr, size_t index, char data) {
    DA *currentPtr;
    size_t currentIndex;

    currentPtr = daFind(arrayPtr, index, &currentIndex);
    currentPtr->data[currentIndex] = data;
}

char daGetBinary(DA *arrayPtr, size_t index) {
    DA *currentPtr;

    size_t currentIndex;

    currentPtr = daFind(arrayPtr, index, &currentIndex);
    return currentPtr->data[currentIndex];
}
