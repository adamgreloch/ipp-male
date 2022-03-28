#ifndef MALE_INPUT_H
#define MALE_INPUT_H

#include "dynarray.h"
#include "stdint.h"

void getFirstInput(DA *arrayPtr);

size_t *getInput(int inputLine, size_t argumentsCount);

size_t getDimNum();

size_t getDimProduct(size_t maxNIndex);

uint8_t *getBinaryWallsRep();

#endif //MALE_INPUT_H