#include <stdio.h>
#include "errMem.h"

void freeData(InputData *d) {
    free(d->dimensions);
    free(d->startPos);
    free(d->endPos);
    free(d->binaryRep);
    free(d->dimProducts);
    free(d);
}

void exitWithError(int errNum, InputData *d) {
    fprintf(stderr, "ERROR %d\n", errNum);
    freeData(d);
    exit(1);
}
