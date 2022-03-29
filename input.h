#ifndef MALE_INPUT_H
#define MALE_INPUT_H

#include "stdint.h"
#include "stddef.h"

struct inputData {
    size_t *dimensions;
    size_t *startPos;
    size_t *endPos;
    uint8_t *binaryRep;
    size_t dimNum;
    size_t *dimProducts;
};

typedef struct inputData inputData;

inputData *getInputData();

size_t getDimProduct(size_t maxNIndex);

uint8_t *getBinaryWallsRep();

#endif //MALE_INPUT_H