#include "cubes.h"
#include "bitTable.h"
#include "main.h"
#include <stdlib.h>

/**
 * isCubeFull determines whether a given cube is full or empty.
 * @param cube is an array of cube coordinates (z_1, z_2, ..., z_k).
 * @param dimensionsPtr should point to the dimensions array declared in main.
 * @param binaryRep should point to the binary representation of the 4th input line.
 * @returns 1 when cube is full, 0 when empty.
 */
int isCubeFull(size_t* cube, uint8_t* binaryRep) {
    size_t index;

    if ((index = rankCube(cube)) == -1)
        // error: cube pos outside dimension
        return -1;
    else if (index > getMaxInputBitLength())
        return 0;
    else
        return getBit(binaryRep, getMaxInputBitLength() - 1 - index);
}

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
    DA* dimensions = getDimensions();
    size_t dim;
    size_t sum = cube[0] - 1, product = 1;
    if (cube[0] > daGet(dimensions, 0))
        // error: cube pos outside dimension
        return -1;
    for (size_t k = 1; k < getDimNum(); k++) {
        product = 1;
        for (size_t i = 0; i < k; i++) {
            dim = daGet(dimensions, i);
            if (cube[i] > dim)
                // error: cube pos outside dimension
                return -1;
            else
                product *= dim;
        }
        sum += (cube[k] - 1) * product;
    }
    //printf("%zu\n", sum);
    return sum;
}

size_t* unrankCube(size_t cubeRank) {
    size_t dimNum = getDimNum();
    size_t* cubeCords = malloc(dimNum*sizeof(size_t));
    size_t product;

    for (size_t i = 0; i < dimNum; i++) {
        product = daGet(getDimensions(), i);
        cubeCords[i] = cubeRank % product + 1;
        cubeRank /= product;
    }

    return cubeCords;
}

size_t getMaxRank() {
    size_t dimNum = getDimNum();
    size_t* cubeCords = malloc(dimNum*sizeof(size_t));

    for (size_t i = 0; i < dimNum; i++)
        cubeCords[i] = daGet(getDimensions(), i);

    return rankCube(cubeCords);
}