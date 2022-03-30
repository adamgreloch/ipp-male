/**
 * @file errMem.h
 * @brief Memory management and error handling module.
 */

#ifndef MALE_ERRMEM_H
#define MALE_ERRMEM_H

#include "input.h"
#include <stdlib.h>

/**
 * @brief Frees input data.
 * @param d - Input data pointer..
 */
void freeData(inputData *d);

/**
 * @brief Frees collected input data and exits with code 1.
 * @param errNum - Error ID.
 * @param d - Input data pointer..
 */
void exitWithError(int errNum, inputData *dataToFree);

#endif //MALE_ERRMEM_H
