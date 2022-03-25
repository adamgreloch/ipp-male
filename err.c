#include <stdio.h>
#include "err.h"

void exitWithError(int errNum) {
    fprintf(stderr, "ERROR %d\n", errNum);
    // TODO znaleźć miejsca, w których trzeba jeszcze zwolnić pamięć przy błędzie
    // TODO błedy na stderr i '1>', '2>' w testujacym
    exit(1);
}
