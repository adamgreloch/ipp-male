#include "bitTable.h"
#include "cubes.h"
#include "dynarray.h"
#include "input.h"
#include "err.h"
#include <stdint.h>
#include <stdlib.h>

int main() {

    DA *dimensions;
    dimensions = malloc(sizeof(DA));
    dimensions->next = NULL;

    getFirstInput(dimensions, 1);

    size_t* startPos = getInput(2);
    size_t* endPos = getInput(3);

    unsigned char* binaryRepArray = getBinaryWallsRep();
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

    if (findCubeState(startPos, dimensions, binaryRepArray) != 0) {
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
