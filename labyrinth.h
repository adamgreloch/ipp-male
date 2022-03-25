#ifndef MALE_LABYRINTH_H
#define MALE_LABYRINTH_H

#include "dynarray.h"
#include "stdint.h"
#include <stddef.h>

size_t findPath(size_t *startPos, size_t *endPos, DA *binaryRep);

#endif //MALE_LABYRINTH_H
