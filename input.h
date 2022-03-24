#ifndef MALE_INPUT_H
#define MALE_INPUT_H

#include "dynarray.h"
#include "stdint.h"

void getFirstInput(DA* arrayPtr, int inputLine);
size_t* getInput(int inputLine);

size_t getDimNum();
size_t getDimProduct();

#define IN 1
#define OUT 0

uint8_t* getBinaryWallsRep();

#endif //MALE_INPUT_H