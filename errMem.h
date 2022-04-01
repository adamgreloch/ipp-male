/**
 * @file errMem.h
 * @brief Memory management and error handling module.
 */

#ifndef MALE_ERRMEM_H
#define MALE_ERRMEM_H

#include "input.h"
#include <stdlib.h>

/**
 * Frees input data.
 * @param d - #InputData pointer.
 */
void freeData(InputData *d);

/**
 * Frees collected input data and exits with code 1.
 * @param errNum - Error ID.
 * @param d - #InputData pointer.
 */
void exitWithError(int errNum, InputData *dataToFree);

#endif //MALE_ERRMEM_H
