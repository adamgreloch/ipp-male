#ifndef MALE_CUBES_H
#define MALE_CUBES_H

#include "input.h"

int findCubeState(size_t* cube, DA* dimensionsPtr, unsigned char* binaryRepArrayPtr);

size_t rankCube(size_t* cube, DA* dimensionsPtr);
size_t* unrankCube(size_t cubeRank, DA* dimensionsPtr);

#endif //MALE_CUBES_H
