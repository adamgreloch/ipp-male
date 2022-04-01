/**
 * @file input.h
 * @brief Input gathering module.
 */

#ifndef MALE_INPUT_H
#define MALE_INPUT_H

#include "stdint.h"
#include "stddef.h"

/**
 * Holds gathered input data that is necessary for path finding.
 */
struct InputData {
    size_t *dimensions;
    /**< Stores n_1, n_2, ..., n_k. */
    size_t *startPos;
    /**< Stores x_1, x_2, ..., x_k. */
    size_t *endPos;
    /**< Stores y_1, y_2, ..., y_k. */
    uint8_t *binaryRep;
    /**< Stores binary representation of the 4th line input.
     *
     * To store the binary expansion we will use an array of chars. 1 char
     * can store 8 bits. Since 1 hex digit represents 4 bits, we can store 2
     * hex values in 1 char thus optimizing memory usage. The bitTable module
     * implements intuitive accessors for such data. */
    size_t dimNum;
    /**< Number of dimensions. */
    size_t *dimProducts;
    /**< Stores every i-th n_1*n_2*...*n_i product. Useful in cube operators. */
};

typedef struct InputData InputData;

/**
 * Gets input from the stdin.
 * @return #InputData pointer.
 */
InputData *getInputData();

size_t getDimProduct(size_t i);

#endif //MALE_INPUT_H
