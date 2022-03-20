#include "cubes.h"
#include "bitTable.h"
#include "main.h"

/**
 * isCubeFull determines whether a given cube is full or empty.
 * @param cube is an array of cube coordinates (z_1, z_2, ..., z_k).
 * @param dimensionsPtr should point to the dimensions array declared in main.
 * @param binaryRepArrayPtr should point to the binary representation of the 4th input line.
 * @returns 1 when cube is full, 0 when empty.
 */
int isCubeFull(size_t* cube, unsigned char* binaryRepArrayPtr) {
/*
    size_t sum = 0, product = 1;
    for (int k = 0; k < getDimNum(); k++) {
        product = 1;
        for (int i = 0; i < k; i++)
            product *= daGet(dimensionsPtr, i);
        sum += (cube[k] - 1) * product;
    }
*/
    size_t index;

    if ((index = rankCube(cube)) == -1)
        // error: cube pos outside dimension
        return -1;
    else
        return getBit(getBinaryRepArray(), getMaxInputBitLength() - 1 - index);
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
    return sum;
}

size_t* unrankCube(size_t cubeRank) {
    size_t dimNum = getDimNum(), product;
    size_t* cubeCords = (size_t*) malloc(dimNum*sizeof(size_t));

    for (size_t i = 0; i < dimNum; i++) {
        product = daGet(getDimensions(), i);
        cubeCords[i] = cubeRank % product + 1;
        cubeRank /= product;
    }

    return cubeCords;
}
