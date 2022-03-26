#include "bitTable.h"
#include "cubes.h"
#include "input.h"
#include "err.h"
#include "labyrinth.h"
#include <stdint.h>
#include <stdlib.h>

static DA *dimensions;
static DA *binaryRep;

static int debug_main = 0;

int main() {
    dimensions = malloc(sizeof(DA));
    dimensions->next = NULL;

    getFirstInput(dimensions);

    size_t *startPos = getInput(2, getDimNum());
    size_t *endPos = getInput(3, getDimNum());

    if (debug_main) {
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
    }

    if (debug_main) printf("%zu\n", getDimProduct(getDimNum()));

    binaryRep = getBinaryWallsRep();

/*
    if (getchar() != EOF) {
        // input error: too many input lines
        free(dimensions);
        free(binaryRep);
        exitWithError(5);
    }
*/

    if (debug_main) {
        for (int i = 0; i <= getMaxInputBitLength(); i++) {
            printf("-%d", getBit(binaryRep, i));
        }
        printf(" -- %d\n", getMaxInputBitLength());
    }

    if (isCubeFull(rankCube(startPos), binaryRep) != 0) {
        // input error: startPos filled or cube pos outside dimension
        free(dimensions);
        free(binaryRep);
        exitWithError(2);
    }

    uint64_t pathLength = findPath(startPos, endPos, binaryRep);

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

DA *getBinaryRep() {
    return binaryRep;
}