#include "cubes.h"
#include <stdlib.h>

size_t rankCube(size_t *cube, InputData *d) {
    size_t dim, product;
    size_t sum = cube[0] - 1;
    if (cube[0] > (d->dimensions)[0])
        // Error: Cube position is outside dimension.
        return -1;
    for (size_t k = 1; k < d->dimNum; k++) {
        product = 1;
        for (size_t i = 0; i < k; i++) {
            dim = (d->dimensions)[i];
            if (cube[i] > dim)
                // Error as above.
                return -1;
            else
                product *= dim;
        }
        sum += (cube[k] - 1) * product;
    }
    return sum;
}

size_t *unrankCube(size_t cubeRank, InputData *d) {
    size_t *cubeCords = malloc((d->dimNum) * sizeof(size_t));
    size_t dimSize;

    for (size_t i = 0; i < d->dimNum; i++) {
        dimSize = (d->dimensions)[i];
        cubeCords[i] = cubeRank % dimSize + 1;
        cubeRank /= dimSize;
    }
    return cubeCords;
}

size_t moveRank(size_t cubeRank, size_t dim, int steps) {
    if (dim == 0)
        return cubeRank + steps;
    else
        return cubeRank + steps * getDimProduct(dim);
}

static size_t maxRank = 0;

size_t getMaxRank(InputData *d) {
    if (maxRank == 0) {
        size_t *cubeCords = malloc((d->dimNum) * sizeof(size_t));

        for (size_t i = 0; i < d->dimNum; i++)
            cubeCords[i] = (d->dimensions)[i];

        maxRank = rankCube(cubeCords, d);
        free(cubeCords);
    }
    return maxRank;
}
