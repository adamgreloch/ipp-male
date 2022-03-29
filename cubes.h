#ifndef MALE_CUBES_H
#define MALE_CUBES_H

#include "input.h"
#include "stdint.h"

size_t rankCube(size_t *cube, inputData *d);

size_t *unrankCube(size_t cubeRank, inputData *d);

size_t moveRank(size_t cubeRank, size_t dim, int steps);

size_t getMaxRank(inputData *d);

#endif //MALE_CUBES_H