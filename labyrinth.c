#include "labyrinth.h"
#include "bitTable.h"
#include "cubes.h"
#include "err.h"
#include "main.h"

//#define DEBUG_LABYRINTH

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

    #ifdef DEBUG_LABYRINTH
    printf("# %d <- q\n", n);
    #endif

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
    #ifdef DEBUG_LABYRINTH
    printf("# try %zu, mod = %zu\n", rankedPos, mod);
    #endif

    if (!isCubeFull(rankedPos, binaryRep)) {
        if (getTwoBit(visited, rankedPos) == 3) {
            push(posQueue, addMod(rankedPos, mod));
            #ifdef DEBUG_LABYRINTH
            printf("# %d -> q (%d, %d)\n", addMod(rankedPos, mod),
                   rankedPos, mod);
            #endif
        }
    }
}

static void expand(size_t rankedPos, uint8_t *binaryRep, int mod) {
    size_t dimNum = getDimNum();
    size_t *pos = unrankCube(rankedPos);
    size_t *debugPos;

    #ifdef DEBUG_LABYRINTH
    printf("# WILL EXPAND for %zu:\n# [", rankedPos);
    for (int z = 0; z < dimNum; z++)
        printf("%d, ", pos[z]);
    printf("]\n");
    #endif

    for (size_t i = 0; i < dimNum; i++) {
        if (pos[i] < daGet(getDimensions(), i)) {
            tryToPush(moveRank(rankedPos, i, 1), binaryRep, mod);
            #ifdef DEBUG_LABYRINTH
            debugPos = unrankCube(moveRank(rankedPos, i, 1));
            printf("# ^EXPANDED FORWARD for %zu, i = %zu:\n# [", rankedPos,
                   i);
            for (int z = 0; z < dimNum; z++)
                printf("%d, ", debugPos[z]);
            printf("] ---- as %d\n", moveRank(rankedPos, i, 1));
            #endif
        }
        if (pos[i] > 1) {
            tryToPush(moveRank(rankedPos, i, -1), binaryRep, mod);
            #ifdef DEBUG_LABYRINTH
            debugPos = unrankCube(moveRank(rankedPos, i, -1));
            printf("# ^EXPANDED BACK for %zu, i = %zu:\n# [", rankedPos, i);
            for (int z = 0; z < dimNum; z++)
                printf("%d, ", debugPos[z]);
            printf("] ---- as %d\n", moveRank(rankedPos, i, -1));
            #endif
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
    int found = 0;

    #ifdef DEBUG_LABYRINTH
    printf("endPos mod: %d\n", mod);
    printf("--> %d\n", rankedStartPos);
    #endif

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
                        #ifdef DEBUG_LABYRINTH
                        printf("found, rpos = %d, mod = %d, f_mod = %d\n",
                               rankCube(pos), mod, (mod - 1) % 3);
                        #endif
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

    visited = malloc(sizeof(uint8_t) * getMaxRank());

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

    int foundPath = 0;
    size_t combined, rankedPos;

    #ifdef DEBUG_LABYRINTH
    printf("# rankedStartPos = %d\n", rankedStartPos);
    printf("# rankedEndPos = %d\n", rankedEndPos);
    #endif

    // TODO divide into another function
    while (!foundPath && !isEmpty(posQueue)) {
        combined = pop(posQueue);
        mod = (getMod(combined) + 1) % 3;
        rankedPos = getPos(combined);

        #ifdef DEBUG_LABYRINTH
        printf("# SPLIT: combined = %d, rankedPos = %d, mod = %d\n",
               combined,
               rankedPos, getMod(combined));
        #endif

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