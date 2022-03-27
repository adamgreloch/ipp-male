#ifndef MALE_LABYRINTH_H
#define MALE_LABYRINTH_H

#include "dynarray.h"
#include "stdint.h"
#include <stddef.h>

int64_t findPath(size_t *startPos, size_t *endPos, uint8_t *binaryRep);

#endif //MALE_LABYRINTH_H
