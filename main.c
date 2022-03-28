#include "bitTable.h"
#include "cubes.h"
#include "input.h"
#include "err.h"
#include "labyrinth.h"
#include <stdint.h>
#include <stdlib.h>

static DA *dimensions;

//#define DEBUG_MAIN

int main() {
    dimensions = malloc(sizeof(DA));
    dimensions->next = NULL;

    // TODO ujednolicić wszystkie dane w jedną strukturę aby
    // 1) skrócić liczbę argumentów w funkcjach
    // 2) uprościć zwalnianie pamięci przy błędach

    getFirstInput(dimensions);
    size_t *startPos = getInput(2, getDimNum());
    size_t *endPos = getInput(3, getDimNum());

#ifdef DEBUG_MAIN
    for (int i = 0; i < getDimNum(); i++) {
        printf("%zu ", daGet(dimensions, i));
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

    uint8_t binaryRep = getBinaryWallsRep();

    if (getchar() != EOF) {
        // input error: too many input lines
        free(startPos);
        free(endPos);
        free(dimensions);
        free(binaryRep);
        exitWithError(5);
    }

    if (getBit(binaryRep, rankCube(startPos)) != 0) {
        // input error: startPos filled or cube pos outside dimension
        free(startPos);
        free(endPos);
        free(dimensions);
        free(binaryRep);
        exitWithError(2);
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

    return 0;
}

DA *getDimensions() {
    return dimensions;
}
