#include "labyrinth.h"
#include "bitTable.h"
#include "cubes.h"
#include "err.h"
#include "main.h"

static int debug_labyrinth = 0;

static struct node {
    size_t data;
    struct node *next;
};
typedef struct node node;

static struct queue {
    size_t size;
    node *front;
    node *rear;
};
typedef struct queue queue;

static void init(queue *q) {
    q->size = 0;
    q->front = q->rear = NULL;
}

static int isEmpty(queue *q) {
    return (q->size <= 0);
}

static void push(queue *q, size_t value) {
    node *new;
    new = malloc(sizeof(node));
    new->data = value;
    new->next = NULL;

    if (!(q->front))
        q->front = q->rear = new;
    else {
        q->rear->next = new;
        q->rear = new;
    }

    q->size++;
}

static size_t pop(queue *q) {
    node *tmp;
    size_t n = q->front->data;

    tmp = q->front;
    q->front = q->front->next;

    q->size--;
    free(tmp);

    if (debug_labyrinth) printf("# %d <- q\n", n);
    return n;
}

static void empty(queue *q) {
    while (!isEmpty(q))
        pop(q);
}

static uint8_t *visited;

static queue *posQueue;

static size_t addMod(size_t pos, int mod) {
    return (pos << 2) | mod;
}

static size_t getPos(size_t posMod) {
    return posMod >> 2;
}

static int getMod(size_t posMod) {
    return (int *) ((posMod) & 3);
}

static void tryToPush(size_t rankedPos, uint8_t *binaryRep, int mod) {
    if (debug_labyrinth) printf("# try %zu, mod = %zu\n", rankedPos, mod);

    if (!isCubeFull(rankedPos, binaryRep)) {
        if (getTwoBit(visited, rankedPos) == 3) {
            push(posQueue, addMod(rankedPos, mod));
            if (debug_labyrinth)
                printf("# %d -> q (%d, %d)\n", addMod(rankedPos, mod),
                       rankedPos, mod);
        }
    }
}

static void expand(size_t rankedPos, uint8_t *binaryRep, int mod) {
    size_t dimNum = getDimNum();
    size_t *pos = unrankCube(rankedPos);
    size_t *debugPos;

    if (debug_labyrinth) {
        printf("# WILL EXPAND for %zu:\n# [", rankedPos);
        for (int z = 0; z < dimNum; z++)
            printf("%d, ", pos[z]);
        printf("]\n");
    }

    for (size_t i = 0; i < dimNum; i++) {
        if (pos[i] < daGet(getDimensions(), i)) {
            tryToPush(moveRank(rankedPos, i, 1), binaryRep, mod);
            if (debug_labyrinth) {
                debugPos = unrankCube(moveRank(rankedPos, i, 1));
                printf("# ^EXPANDED FORWARD for %zu, i = %zu:\n# [", rankedPos,
                       i);
                for (int z = 0; z < dimNum; z++)
                    printf("%d, ", debugPos[z]);
                printf("] ---- as %d\n", moveRank(rankedPos, i, 1));
            }
        }
        if (pos[i] > 1) {
            tryToPush(moveRank(rankedPos, i, -1), binaryRep, mod);
            if (debug_labyrinth) {
                debugPos = unrankCube(moveRank(rankedPos, i, -1));
                printf("# ^EXPANDED BACK for %zu, i = %zu:\n# [", rankedPos, i);
                for (int z = 0; z < dimNum; z++)
                    printf("%d, ", debugPos[z]);
                printf("] ---- as %d\n", moveRank(rankedPos, i, -1));
            }
        }
    }

    free(pos);
}

static int64_t
findPathLength(size_t rankedStartPos, size_t *endPos, uint8_t *visited) {
    size_t dimNum = getDimNum();
    int64_t length = 0;
    size_t *pos = endPos;
    int mod = getMod(getTwoBit(visited, rankCube(pos)));
    if (debug_labyrinth) printf("endPos mod: %d\n", mod);
    int found = 0;

    if (debug_labyrinth) printf("--> %d\n", rankedStartPos);

    while (rankCube(pos) != rankedStartPos) {
        if (mod == 0)
            mod = 3;

        for (size_t i = 0; i < dimNum; i++) {
            for (int j = -1; !found && j <= 1; j += 2)
                if (1 <= pos[i] + j &&
                    pos[i] + j <= daGet(getDimensions(), i)) {
                    pos[i] += j;
                    if (getMod(getTwoBit(visited, rankCube(pos))) ==
                        (mod - 1) % 3) {
                        if (debug_labyrinth)
                            printf("found, rpos = %d, mod = %d, f_mod = %d\n",
                                   rankCube(pos), mod, (mod - 1) % 3);
                        found = 1;
                    } else
                        pos[i] -= j;
                }
        }

        if (found) {
            length++;
            found = 0;
            mod = (mod - 1) % 3;
        }
    }

    free(visited);
    return length;
}

static int isNoWayOneDim(size_t rankedStartPos, size_t rankedEndPos, uint8_t *binaryRep) {
    if (getDimNum() != 1) return 0;
    size_t min = rankedStartPos, max = rankedEndPos;
    if (rankedStartPos > rankedEndPos) {
        min = rankedEndPos;
        max = rankedStartPos;
    }
    for (size_t i = min; i < max; i++)
        if (getBit(binaryRep, i) == 1)
            return 1;

    return 0;
}

int64_t findPath(size_t *startPos, size_t *endPos, uint8_t *binaryRep) {
    size_t rankedStartPos = rankCube(startPos);
    size_t rankedEndPos = rankCube(endPos);

    if (isNoWayOneDim(rankedStartPos, rankedEndPos, binaryRep))
        return -1;

    visited = (uint8_t *) malloc(sizeof(uint8_t) * getMaxRank());

    if (!visited) {
        // malloc failed
        free(startPos);
        free(endPos);
        free(binaryRep);
        exitWithError(0);
    }

    for (size_t i = 0; i < getMaxRank(); i++) {
        visited[i] = 0xFF;
    }

    posQueue = malloc(sizeof(queue));
    init(posQueue);

    int mod = 0;

    push(posQueue, addMod(rankedStartPos, mod));
/*
    size_t rankedPos = rankCube(startPos);
    setTwoBit(&visited, rankedPos, mod);
    expand(rankedPos, binaryRep, mod);
*/

    int foundPath = 0;
    size_t combined, rankedPos;

    if (debug_labyrinth) printf("# rankedStartPos = %d\n", rankedStartPos);
    if (debug_labyrinth) printf("# rankedEndPos = %d\n", rankedEndPos);

    // TODO divide into another function
    while (!foundPath && !isEmpty(posQueue)) {
        combined = pop(posQueue);
        mod = (getMod(combined) + 1) % 3;
        rankedPos = getPos(combined);

        if (debug_labyrinth)
            printf("# SPLIT: combined = %d, rankedPos = %d, mod = %d\n",
                   combined,
                   rankedPos, getMod(combined));

        if (rankedPos == rankedEndPos) {
            foundPath = 1;
            empty(posQueue);
        } else
            expand(rankedPos, binaryRep, mod);
        setTwoBit(&visited, rankedPos, getMod(combined));
    }

    if (!foundPath)
        return -1;
    else
        return findPathLength(rankedStartPos, endPos, visited);
}