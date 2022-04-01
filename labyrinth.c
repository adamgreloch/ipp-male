#include "labyrinth.h"
#include "bitTable.h"
#include "cubes.h"
#include "errMem.h"
#include "stack.h"

static uint8_t *visited;
static Stack *mod0Stack;
static Stack *mod1Stack;

/**
 * Tries to push a cube into a Stack. Will succeed if the cube is not a
 * wall and was not visited earlier.
 * @param rankedPos - Rank of cube.
 * @param mod - Path length modulo 2.
 * @param d - #InputData pointer.
 */
static void tryToPush(size_t rankedPos, int mod, InputData *d) {
    if (!getBit(visited, rankedPos, d)) {
        setBit(&visited, rankedPos, 1);
        push(mod ? mod1Stack : mod0Stack, rankedPos);
    }
}

/**
 * Expands broader into a graph.
 * @param rankedPos - Rank of cube.
 * @param mod - Path length modulo 2.
 * @param d - #InputData pointer.
 */
static void expand(size_t rankedPos, int mod, InputData *d) {
    size_t *pos = unrankCube(rankedPos, d);

    for (size_t i = 0; i < d->dimNum; i++) {
        if (pos[i] < (d->dimensions)[i])
            tryToPush(moveRank(rankedPos, i, 1), 1 - mod, d);
        if (pos[i] > 1)
            tryToPush(moveRank(rankedPos, i, -1), 1 - mod, d);
    }
    free(pos);
}

/**
 * Checks whether a given labyrinth configuration is one-dimensional
 * and thus if it is even worth breadth searching.
 * @details If a labyrinth is one-dimensional, it is easy to determine
 * whether a path exists by checking if there exists a wall between
 * startPos and endPos, since that would be the only possible path.
 * @param rankedStartPos - Rank of starting position
 * @param rankedEndPos - Rank of destination
 * @param d - #InputData pointer.
 * @return 1 if the labyrinth is 1D and there is no possible path, 0 otherwise.
 */
static int
isNoWayOneDim(size_t rankedStartPos, size_t rankedEndPos, InputData *d) {
    if (d->dimNum != 1) return 0;
    size_t min = rankedStartPos, max = rankedEndPos;
    if (rankedStartPos > rankedEndPos) {
        min = rankedEndPos;
        max = rankedStartPos;
    }
    for (size_t i = min; i < max; i++)
        if (getBit(d->binaryRep, i, d))
            return 1;

    return 0;
}

/**
 * Initialize mod0 and mod1 stacks used for BFS.
 * @return Table of two stacks.
 */
static Stack **initStacks() {
    mod0Stack = malloc(sizeof(Stack));
    mod1Stack = malloc(sizeof(Stack));
    init(mod0Stack);
    init(mod1Stack);

    Stack **stacks = malloc(2 * sizeof(Stack));

    stacks[0] = mod0Stack;
    stacks[1] = mod1Stack;

    return stacks;
}

static void freeStacks(Stack **stacks) {
    empty(mod0Stack);
    empty(mod1Stack);
    free(stacks);
}

int64_t findPath(InputData *d) {
    size_t rankedStartPos = rankCube(d->startPos, d);
    size_t rankedEndPos = rankCube(d->endPos, d);
    size_t pos;

    if (isNoWayOneDim(rankedStartPos, rankedEndPos, d))
        return -1;

    visited = d->binaryRep;

    Stack **stacks = initStacks();

    setBit(&visited, rankedStartPos, 1);
    push(mod0Stack, rankedStartPos);

    int foundPath = 0;
    int mod = 0;

    int64_t pathLength = -1;

    while (!foundPath && !isEmpty(stacks[mod])) {
        while (!isEmpty(stacks[mod])) {
            pos = pop(stacks[mod]);
            if (pos == rankedEndPos)
                foundPath = 1;
            else
                expand(pos, mod, d);
        }
        mod = 1 - mod;
        pathLength++;
    }

    freeStacks(stacks);

    if (!foundPath)
        return -1;
    else
        return pathLength;
}
