#include "labyrinth.h"
#include "input.h"

int main() {

    DA *dimensions, *startCords, *destCords;

    dimensions = malloc(sizeof(DA));
    dimensions->next = NULL;

    startCords = malloc(sizeof(DA));
    startCords->next = NULL;

    destCords = malloc(sizeof(DA));
    destCords->next = NULL;

    getInput(dimensions, 1);
    getInput(startCords, 2);
    getInput(destCords, 3);

    free(dimensions);
    free(startCords);
    free(destCords);

    return 0;
}


