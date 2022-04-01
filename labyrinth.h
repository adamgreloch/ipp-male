/**
 * @file labyrinth.h
 * @brief Module for finding shortest path in a k-dimensional labyrinth.
 */

#ifndef MALE_LABYRINTH_H
#define MALE_LABYRINTH_H

#include "input.h"
#include "stdint.h"
#include <stddef.h>

/**
 * Finds path in a labyrinth defined by #InputData.
 * @param d - #InputData pointer.
 * @return A path length or -1 in case when path in a labyrinth defined by
 * #InputData does not exist.
 */
int64_t findPath(InputData *d);

#endif //MALE_LABYRINTH_H
