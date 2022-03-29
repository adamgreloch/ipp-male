#ifndef MALE_INPUT_H
#define MALE_INPUT_H

#include "stdint.h"

size_t *getFirstInput();

size_t *getInput(int inputLine, size_t argumentsCount);

size_t getDimNum();

size_t getDimProduct(size_t maxNIndex);
size_t *getDimProductsPtr();

uint8_t *getBinaryWallsRep();

#endif //MALE_INPUT_H