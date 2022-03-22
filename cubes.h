#ifndef MALE_CUBES_H
#define MALE_CUBES_H

#include "input.h"
#include "stdint.h"

int isCubeFull(size_t* cube, uint8_t* binaryRepArrayPtr);

size_t rankCube(size_t* cube);
size_t* unrankCube(size_t cubeRank);

#endif //MALE_CUBES_H