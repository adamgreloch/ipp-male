#ifndef MALE_INPUT_H
#define MALE_INPUT_H

#include "dynarray.h"

void getFirstInput(DA* arrayPtr, int inputLine);
size_t* getInput(int inputLine);
size_t getDimNum();

#define IN 1
#define OUT 0

unsigned char* getBinaryWallsRep();

#endif //MALE_INPUT_H