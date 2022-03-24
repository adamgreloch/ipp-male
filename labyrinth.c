#include "labyrinth.h"
#include "bitTable.h"
#include "cubes.h"
#include "err.h"
#include "main.h"

int debug = 0;

static struct node {
    size_t data;
    struct node* next;
};
typedef struct node node;

static struct queue {
    size_t size;
    node* front;
    node* rear;
};
typedef struct queue queue;

static void init(queue* q) {
    q->size = 0;
    q->front = q->rear = NULL;
}

static int isEmpty(queue* q) {
    return (q->size <= 0);
}

static void push(queue* q, size_t value) {
    node* new;
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

static size_t pop(queue* q) {
    node* tmp;
    size_t n = q->front->data;

    tmp = q->front;
    q->front = q->front->next;

    q->size--;
    free(tmp);

    if (debug) printf("# %d <- q\n", n);
    return n;
}

static void empty(queue* q) {
    while (!isEmpty(q))
        pop(q);
}

static uint8_t* visited;

static queue* posQueue;

static size_t addMod(size_t pos, int mod) {
    return (pos << 2) | mod;
}

static size_t getPos(size_t posMod) {
    return posMod >> 2;
}

static int getMod(size_t posMod) {
    return (int*) ((posMod) & 3);
}

static void tryToPush(size_t* pos, uint8_t* binaryRep, int mod) {
    if (debug) printf("# try %d, mod = %d\n", rankCube(pos), mod);

    if (!isCubeFull(pos, binaryRep)) {
        size_t rankedPos = rankCube(pos);

        if (getTwoBit(visited, rankedPos) == 3) {
            push(posQueue, addMod(rankedPos, mod));
            if (debug) printf("# %d -> q (%d, %d)\n", addMod(rankedPos, mod), rankedPos, mod);
        }
    }
}

static void expand(size_t rankedPos, uint8_t* binaryRep, int mod) {
    size_t dimNum = getDimNum();
    size_t* pos = unrankCube(rankedPos);

    for (size_t i = 0; i < dimNum; i++) {
        if (pos[i] < daGet(getDimensions(), i)) {
            pos[i]++; // Make one step forward in a dimension.
            if (debug) printf("# gen %d\n", rankCube(pos));
            tryToPush(pos, binaryRep, mod);
            pos[i]--; // Revert value modifications.
        }
        if (pos[i] > 1) {
            pos[i]--; // Make one step backward.
            if (debug) printf("# gen %d\n", rankCube(pos));
            tryToPush(pos, binaryRep, mod);
            pos[i]++; // Revert value modifications.
        }
    }

    free(pos);
}

static size_t findPathLength(size_t rankedStartPos, size_t* endPos, uint8_t* visited) {
    size_t dimNum = getDimNum();
    size_t length = 0;
    size_t* pos = endPos;
    int mod = getMod(getTwoBit(visited, rankCube(pos)));
    int found = 0;

    if (debug) printf("--> %d\n", rankedStartPos);

    while (rankCube(pos) != rankedStartPos) {
        if (mod == 0)
            mod = 3;

        for (size_t i = 0; i < dimNum; i++) {
            for (int j = -1; !found && j <= 1; j += 2)
                if (1 <= pos[i] + j && pos[i] + j <= daGet(getDimensions(), i)) {
                    pos[i] += j;
                    if (getMod(getTwoBit(visited, rankCube(pos))) == (mod - 1) % 3) {
                        if (debug) printf("found, rpos = %d, mod = %d, f_mod = %d\n", rankCube(pos), mod, (mod - 1) % 3);
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

size_t findPath(size_t* startPos, size_t* endPos, uint8_t* binaryRep) {
    visited = (uint8_t*) malloc(sizeof(uint8_t)*getMaxRank());  // ! rozmiar narazie roboczy

    if (!visited)
        // malloc failed
        return -1;

    for (int i = 0; i < getMaxRank(); i++) {
        visited[i] = 0xFF;
    }

    posQueue = malloc(sizeof(queue));
    init(posQueue);

    int mod = 0;

    push(posQueue, addMod(rankCube(startPos),mod));
/*
    size_t rankedPos = rankCube(startPos);
    setTwoBit(&visited, rankedPos, mod);
    expand(rankedPos, binaryRep, mod);
*/

    int foundPath = 0;
    size_t posMod, rankedPos;

    size_t rankedEndPos = rankCube(endPos);
    if (debug) printf("# rEndPos: %d\n", rankedEndPos);

    while (!foundPath && !isEmpty(posQueue)) {
        posMod = pop(posQueue);

        mod = (getMod(posMod)+1) % 3;
        rankedPos = getPos(posMod);
        if (debug) printf("# SPLIT: posMod = %d, rankedPos = %d, mod = %d\n", posMod, rankedPos, getMod(posMod));

        if (rankedPos == rankedEndPos) {
            foundPath = 1;
            empty(posQueue);
        } else
            expand(rankedPos, binaryRep, mod);
        setTwoBit(&visited, rankedPos, mod);
    }

    if (!foundPath)
        return -1;
    else
        return findPathLength(rankCube(startPos), endPos, visited);
}
