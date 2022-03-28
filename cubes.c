#include "cubes.h"
#include "bitTable.h"
#include "main.h"
#include <stdlib.h>

//#define DEBUG_CUBES

/**
 * rankCube translates (z_1, z_2, ..., z_k) cube coordinates into a bitNumber.
 *
 * The result is calculated using the following equation
 * bitNumber(z_1,z_2,...,z_k) = (z_1-1)+(z_2-1)n_1 + (z_3-1)n_1n_2 + ... + (z_k-1)n_1n_2...n_{k-1}.
 * @param cube is an array of cube coordinates (z_1, z_2, ..., z_k).
 * @param dimensionsPtr should point to the dimensions array declared in main.
 * @returns bitNumber.
 */
// TODO wykazać, że rank jest bijekcją
size_t rankCube(size_t *cube) {
    DA *dimensions = getDimensions();
    size_t dim, product;
    size_t sum = cube[0] - 1;
    if (cube[0] > daGet(dimensions, 0))
        // error: cube pos outside dimension
        return -1;
    for (size_t k = 1; k < getDimNum(); k++) {
        product = 1;
        for (size_t i = 0; i < k; i++) {
            dim = daGet(dimensions, i);
#ifdef DEBUG_CUBES
            printf("# cube = %zu, dim = %zu\n", cube[i], dim);
#endif
            if (cube[i] > dim)
                // error: cube pos outside dimension
                return -1;
            else
                product *= dim;
        }
        sum += (cube[k] - 1) * product;
    }
#ifdef DEBUG_CUBES
    printf("rankCube: %zu\n", sum);
#endif
    return sum;
}

// TODO optimize
size_t *unrankCube(size_t cubeRank) {
    size_t dimNum = getDimNum();
    size_t *cubeCords = malloc(dimNum * sizeof(size_t));
    size_t product;

    for (size_t i = 0; i < dimNum; i++) {
        product = daGet(getDimensions(), i);
        cubeCords[i] = cubeRank % product + 1;
        cubeRank /= product;
    }

    return cubeCords;
}

/***
 * moveRank moves from one cube to another in one dimension by a integer steps.
 * @param cubeRank
 * @param dim
 * @param steps
 * @return destination rank
 */
size_t moveRank(size_t cubeRank, size_t dim, int steps) {
    if (dim == 0)
        return cubeRank + steps;
    else
        return cubeRank + steps * getDimProduct(dim);
}

size_t getMaxRank() {
    // TODO check if memoization would be useful
    size_t dimNum = getDimNum();
    size_t *cubeCords = malloc(dimNum * sizeof(size_t));

    for (size_t i = 0; i < dimNum; i++)
        cubeCords[i] = daGet(getDimensions(), i);

    size_t maxRank = rankCube(cubeCords);
    free(cubeCords);

    return maxRank;
}