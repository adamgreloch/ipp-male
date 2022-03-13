#include "err.h"
#include <stdio.h>
#include <stdlib.h>

void exitWithError(int errNum) {
    printf("ERROR %d\n", errNum);
    // TODO znaleźć miejsca, w których trzeba jeszcze zwolnić pamięć przy błędzie
    exit(1);
}
