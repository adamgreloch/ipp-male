#include "bitTable.h"
#include "cubes.h"
#include "input.h"
#include "err.h"
#include "labyrinth.h"
#include <stdint.h>
#include <stdlib.h>

static DA *dimensions;
static DA *binaryRep;

static int debug = 0;

int main() {
    dimensions = malloc(sizeof(DA));
    dimensions->next = NULL;

    getFirstInput(dimensions);

    size_t *startPos = getInput(2);
    size_t *endPos = getInput(3);

    if (debug) {
        for (int i = 0; i < getDimNum(); i++) {
            printf("%d ", daGet(dimensions, i));
        }
        putchar('\n');
        for (int i = 0; i < getDimNum(); i++) {
            printf("%d ", startPos[i]);
        }
        putchar('\n');
        for (int i = 0; i < getDimNum(); i++) {
            printf("%d ", endPos[i]);
        }
        putchar('\n');
    }

    if (debug) printf("%d\n", getDimProduct());

    binaryRep = getBinaryWallsRep();

/*
    if (getchar() != EOF) {
        // input error: too many input lines
        free(dimensions);
        free(binaryRep);
        exitWithError(5);
    }
*/

    if (debug) {
        for (int i = 0; i < getMaxInputBitLength(); i++) {
            printf("-%d", getBit(binaryRep, i));
        }
        putchar('\n');
    }

    if (isCubeFull(startPos, binaryRep) != 0) {
        // input error: startPos filled or cube pos outside dimension
        free(dimensions);
        free(binaryRep);
        exitWithError(2);
    }

    printf("%d\n", findPath(startPos, endPos, binaryRep));

    free(startPos);
    free(endPos);
    free(dimensions);
    free(binaryRep);

    return 0;
}

DA *getDimensions() {
    return dimensions;
}

uint8_t *getBinaryRep() {
    return binaryRep;
}