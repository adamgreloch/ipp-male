#include "err.h"
#include <stdio.h>
#include <stdlib.h>

void exitWithError(int errNum) {
    printf("ERROR %d\n", errNum);
    // TODO jawne zwolnienie pamięci
    exit(1);
}
