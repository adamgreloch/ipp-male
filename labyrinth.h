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
 * @brief Finds path in a labyrinth defined by #inputData.
 * @param d - Input data pointer.
 * @return A path length or -1 in case when path between two points specified in
 * #inputData does not exist.
 */
int64_t findPath(inputData *d);

#endif //MALE_LABYRINTH_H
