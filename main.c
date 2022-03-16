#include "bitTable.h"
#include "walls.h"
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

    if (!binaryRepArray)
        // memory error: malloc failed
        exitWithError(0);

//    if (getchar() != EOF) {
//        // input error: too many input lines
//        free(dimensions);
//        free(binaryRepArray);
//        exitWithError(5);
//    }

//    for (int i = 0; i < getMaxBitLength(); i++) {
//        printf("%d", getBit(&binaryRepArray, i));
//    }
//
//    printf("\n%d", getMaxBitLength());

    if (findCubeState(startPos, dimensions, binaryRepArray)) {
        // input error: startPos filled
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
