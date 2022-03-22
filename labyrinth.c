#include "labyrinth.h"
#include "bitTable.h"
#include "cubes.h"
#include "err.h"
#include "main.h"

// TODO FIFO

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

    printf("# %d <- q\n", n);
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
    printf("# try %d, mod = %d\n", rankCube(pos), mod);

    if (!isCubeFull(pos, binaryRep)) {
        size_t rankedPos = rankCube(pos);

        if (getTwoBit(visited, rankedPos) == 3) {
            push(posQueue, addMod(rankedPos, mod));
            printf("# %d -> q (%d, %d)\n", addMod(rankedPos, mod), rankedPos, mod);
        }
    }
}

static void expand(size_t rankedPos, uint8_t* binaryRep, int mod) {
    size_t dimNum = getDimNum();
    size_t dimSize;
    size_t* pos = unrankCube(rankedPos);

    for (size_t i = 0; i < dimNum; i++) {
        if (pos[i] < daGet(getDimensions(), i)) {
            pos[i]++; // Make one step forward in a dimension.
            printf("# gen %d\n", rankCube(pos));
            tryToPush(pos, binaryRep, mod);
            pos[i]--; // Revert value modifications.
        }
        if (pos[i] > 1) {
            pos[i]--; // Make one step backward.
            printf("# gen %d\n", rankCube(pos));
            tryToPush(pos, binaryRep, mod);
            pos[i]++; // Revert value modifications.
        }
    }
}

static size_t findPathLength(size_t* startPos, size_t* endPos, uint8_t* visited) {
    return 42;
}

size_t findPath(size_t* startPos, size_t* endPos, uint8_t* binaryRep) {
    visited = (uint8_t*) malloc(1024*sizeof(uint8_t)); // ! rozmiar narazie roboczy

    if (!visited)
        // malloc failed
        return -1;

    for (int i = 0; i < 1024; i++) {
        visited[i] = 0xFF;
    }

    posQueue = malloc(sizeof(queue));
    init(posQueue);

    int mod = 0;
    size_t rankedPos = rankCube(startPos);
    expand(rankedPos, binaryRep, mod);

    int foundPath = 0;
    size_t posMod;

    size_t rankedEndPos = rankCube(endPos);
    printf("# rEndPos: %d\n", rankedEndPos);

    while (!foundPath && !isEmpty(posQueue)) {
        posMod = pop(posQueue);

        mod = (getMod(posMod)+1) % 3;
        rankedPos = getPos(posMod);
        printf("# SPLIT: posMod = %d, rankedPos = %d, mod = %d\n", posMod, rankedPos, getMod(posMod));

        if (rankedPos == rankedEndPos) {
            foundPath = 1;
            empty(posQueue);
        } else {
            setTwoBit(&visited, rankedPos, mod);
            expand(rankedPos, binaryRep, mod);
        }
    }
/*
    int xd = 11;
    setTwoBit(&visited, xd, 3);
    printf("---%d\n", getTwoBit(visited, xd));
    setTwoBit(&visited, 11, 2);
    printf("---%d\n", getTwoBit(visited, xd));
    xd = 10;
    setTwoBit(&visited, xd, 3);
    printf("---%d\n", getTwoBit(visited, xd));
    setTwoBit(&visited, xd, 2);
    printf("---%d\n", getTwoBit(visited, xd));
    for (int i = 0; i < 8; i++)
        printf("%d\n", (223 & (3 << (2*i))) >> (2*i));
*/

    if (!foundPath)
        return -1;
    else
        return findPathLength(startPos, endPos, visited);
}
