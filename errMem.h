#ifndef MALE_ERRMEM_H
#define MALE_ERRMEM_H

#include "input.h"

void freeData(inputData *d);

void exitWithError(int errNum, inputData *dataToFree);

#endif //MALE_ERRMEM_H
