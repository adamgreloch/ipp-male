#include "bitTable.h"
#include "cubes.h"
#include "input.h"
#include "err.h"
#include "labyrinth.h"
#include <stdint.h>
#include <stdlib.h>

static DA *dimensions;
static uint8_t* binaryRep;

int main() {
    dimensions = malloc(sizeof(DA));
    dimensions->next = NULL;

    getFirstInput(dimensions, 1);

    size_t* startPos = getInput(2);
    size_t* endPos = getInput(3);

    binaryRep = getBinaryWallsRep();
    // z tego co widzę, ta liczba może być też za mała, by
    // być w stanie opisać labirynt. To też należałoby chyba sprawdzić.

    if (!binaryRep)
        // memory error: malloc failed
        exitWithError(0);

//    if (getchar() != EOF) {
//        // input error: too many input lines
//        free(dimensions);
//        free(binaryRep);
//        exitWithError(5);
//    }

    for (int i = 0; i < getMaxInputBitLength(); i++) {
        printf("--%d", getBit(&binaryRep, i));
    }
    putchar('\n');
//
//    printf("%d\n", getMaxInputBitLength());
//
//    printf("%d\n", rankCube(startPos, dimensions));
//    printf("%d\n", rankCube(unrankCube(rankCube(startPos, dimensions), dimensions), dimensions));

    if (isCubeFull(startPos, binaryRep) != 0) {
        // input error: startPos filled or cube pos outside dimension
        free(dimensions);
        free(binaryRep);
        exitWithError(2);
    }

    printf("%d", findPath(startPos, endPos, binaryRep));

    free(startPos);
    free(endPos);
    free(dimensions);
    free(binaryRep);

    return 0;
}

DA* getDimensions() {
    return dimensions;
}

uint8_t* getBinaryRep() {
    return binaryRep;
}