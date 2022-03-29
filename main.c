#include "bitTable.h"
#include "cubes.h"
#include "input.h"
#include "err.h"
#include "labyrinth.h"
#include <stdint.h>
#include <stdlib.h>

static size_t *dimensions;

//#define DEBUG_MAIN

int main() {
    // TODO ujednolicić wszystkie dane w jedną strukturę aby
    // 1) skrócić liczbę argumentów w funkcjach
    // 2) uprościć zwalnianie pamięci przy błędach

    dimensions = getFirstInput();
    size_t *startPos = getInput(2, getDimNum());
    size_t *endPos = getInput(3, getDimNum());

#ifdef DEBUG_MAIN
    for (int i = 0; i < getDimNum(); i++) {
        printf("%zu ", #ifdef DEBUG_LABYRINTH(dimensions, i));
    }
    putchar('\n');
    for (int i = 0; i < getDimNum(); i++) {
        printf("%zu ", startPos[i]);
    }
    putchar('\n');
    for (int i = 0; i < getDimNum(); i++) {
        printf("%zu ", endPos[i]);
    }
    putchar('\n');
    printf("%zu\n", getDimProduct(getDimNum()));
#endif

    uint8_t *binaryRep = getBinaryWallsRep();

    if (getchar() != EOF) {
        // TODO move to input
        // input error: too many input lines
        free(startPos);
        free(endPos);
        free(dimensions);
        free(binaryRep);
        exitWithError(5);
    }

    if (getBit(binaryRep, rankCube(startPos)) != 0) {
        // TODO move to input
        // input error: startPos filled or cube pos outside dimension
        free(startPos);
        free(endPos);
        free(dimensions);
        free(binaryRep);
        exitWithError(2);
    }

    if (getBit(binaryRep, rankCube(endPos)) != 0) {
        // TODO move to input
        // input error: startPos filled or cube pos outside dimension
        free(startPos);
        free(endPos);
        free(dimensions);
        free(binaryRep);
        exitWithError(3);
    }

    int64_t pathLength = findPath(startPos, endPos, binaryRep);

    if (pathLength == -1)
        printf("NO WAY\n");
    else
        printf("%d\n", pathLength);

    free(startPos);
    free(endPos);
    free(dimensions);
    free(binaryRep);
    free(getDimProductsPtr());

    return 0;
}

size_t *getDimensions() {
    return dimensions;
}
