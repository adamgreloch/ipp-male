#ifndef MALE_CUBES_H
#define MALE_CUBES_H

#include "input.h"
#include "stdint.h"

int isCubeFull(size_t rankedCube, uint8_t *binaryRep);

size_t rankCube(size_t *cube);

size_t *unrankCube(size_t cubeRank);

size_t moveRank(size_t cubeRank, size_t dim, int steps);

size_t getMaxRank();

#endif //MALE_CUBES_H