#include "dynarray.h"

// TODO ogarnąć
// TODO dokumentacja

static DA *daFind(DA *arrayPtr, size_t index, size_t *currentIndexPtr) {
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

            // TODO check if necessary
//            for (int i = 0; i < BLOCK_SIZE; i++)
//                currentPtr->next->data[i] = 0;

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

void daPut8Bit(DA *arrayPtr, size_t index, uint8_t data) {
    DA *currentPtr;
    size_t currentIndex;

    currentPtr = daFind(arrayPtr, index, &currentIndex);
    currentPtr->data[currentIndex] = data;
}

uint8_t daGet8Bit(DA *arrayPtr, size_t index) {
    DA *currentPtr;

    size_t currentIndex;

    currentPtr = daFind(arrayPtr, index, &currentIndex);
    return currentPtr->data[currentIndex];
}
