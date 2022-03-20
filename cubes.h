#ifndef MALE_CUBES_H
#define MALE_CUBES_H

#include "input.h"

int isCubeFull(size_t* cube, unsigned char* binaryRepArrayPtr);

size_t rankCube(size_t* cube);
size_t* unrankCube(size_t cubeRank);

#endif //MALE_CUBES_H
