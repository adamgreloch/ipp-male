#include "bitTable.h"
#include "cubes.h"
#include "input.h"
#include "err.h"
#include <stdint.h>
#include <stdlib.h>

static DA *dimensions;
static unsigned char* binaryRepArray;

int main() {
    dimensions = malloc(sizeof(DA));
    dimensions->next = NULL;

    getFirstInput(dimensions, 1);

    size_t* startPos = getInput(2);
    size_t* endPos = getInput(3);

    binaryRepArray = getBinaryWallsRep();
    // z tego co widzę, ta liczba może być też za mała, by
    // być w stanie opisać labirynt. To też należałoby chyba sprawdzić.

    if (!binaryRepArray)
        // memory error: malloc failed
        exitWithError(0);

//    if (getchar() != EOF) {
//        // input error: too many input lines
//        free(dimensions);
//        free(binaryRepArray);
//        exitWithError(5);
//    }

    for (int i = 0; i < getMaxInputBitLength(); i++) {
        printf("--%d", getBit(&binaryRepArray, i));
    }

//    putchar('\n');
//
//    printf("%d\n", getMaxInputBitLength());
//
//    printf("%d\n", rankCube(startPos, dimensions));
//    printf("%d\n", rankCube(unrankCube(rankCube(startPos, dimensions), dimensions), dimensions));

    if (isCubeFull(startPos, dimensions, binaryRepArray) != 0) {
        // input error: startPos filled or cube pos outside dimension
        free(dimensions);
        free(binaryRepArray);
        exitWithError(2);
    }

    free(startPos);
    free(endPos);
    free(dimensions);
    free(binaryRepArray);

    return 0;
}

DA* getDimensions() {
    return dimensions;
}

unsigned char* getBinaryRepArray() {
    return binaryRepArray;
}