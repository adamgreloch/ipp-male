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

static struct stack {
    size_t size;
    node *top;
};
typedef struct stack stack;

static void init(stack *s) {
    s->size = 0;
    s->top = NULL;
}

static int isEmpty(stack *s) {
    return (s->size <= 0);
}

static void push(stack *s, size_t value) {
    node *new = malloc(sizeof(node));
    new->data = value;
    new->next = NULL;

    if (!(s->top))
        new->next = NULL;
    else
        new->next = s->top;

    s->top = new;
    s->size++;
}

// Assumes s is non-empty.
static size_t pop(stack *s) {
    node *tmp = s->top;
    size_t popped = s->top->data;
    s->top = s->top->next;
    s->size--;
    free(tmp);

#ifdef DEBUG_LABYRINTH
    printf("# %d <- q\n", popped);
#endif

    return popped;
}

static void empty(stack *s) {
    while (!isEmpty(s))
        pop(s);
}

static uint8_t *visited;
static stack *mod0Stack;
static stack *mod1Stack;

// TODO optimize, too many args (can pass on stacks, no need for global)
static void tryToPush(size_t rankedPos, int mod) {
#ifdef DEBUG_LABYRINTH
    printf("# try %zu, mod = %zu\n", rankedPos, mod);
#endif

    if (!getBit(visited, rankedPos)) {
        setBit(&visited, rankedPos, 1);
        push(mod ? mod1Stack : mod0Stack, rankedPos);

#ifdef DEBUG_LABYRINTH
        printf("# visited %d\n", rankedPos);
        printf("# %d -> q (mod %d)\n", rankedPos, mod);
#endif
    }
}

// TODO too many args
static void expand(size_t rankedPos, size_t *dimensions, int mod) {
    size_t dimNum = getDimNum();
    size_t *pos = unrankCube(rankedPos);

#ifdef DEBUG_LABYRINTH
    size_t *debugPos;
    printf("# WILL EXPAND for %zu:\n# [", rankedPos);
    for (int z = 0; z < dimNum; z++)
        printf("%d, ", pos[z]);
    printf("]\n");
#endif

    for (size_t i = 0; i < dimNum; i++) {
        if (pos[i] < dimensions[i]) {
            tryToPush(moveRank(rankedPos, i, 1), 1 - mod);
#ifdef DEBUG_LABYRINTH
            debugPos = unrankCube(moveRank(rankedPos, i, 1));
            printf("# ^EXPANDED FORWARD for %zu, i = %zu:\n# [", rankedPos, i);
            for (int z = 0; z < dimNum; z++)
                printf("%d, ", debugPos[z]);
            printf("] ---- as %d\n", moveRank(rankedPos, i, 1));
            free(debugPos);
#endif
        }
        if (pos[i] > 1) {
            tryToPush(moveRank(rankedPos, i, -1), 1 - mod);
#ifdef DEBUG_LABYRINTH
            debugPos = unrankCube(moveRank(rankedPos, i, -1));
            printf("# ^EXPANDED BACK for %zu, i = %zu:\n# [", rankedPos, i);
            for (int z = 0; z < dimNum; z++)
                printf("%d, ", debugPos[z]);
            printf("] ---- as %d\n", moveRank(rankedPos, i, -1));
            free(debugPos);
#endif
        }
    }

    free(pos);
}

static int
isNoWayOneDim(size_t rankedStartPos, size_t rankedEndPos, uint8_t *binaryRep) {
    if (getDimNum() != 1) return 0;
    size_t min = rankedStartPos, max = rankedEndPos;
    if (rankedStartPos > rankedEndPos) {
        min = rankedEndPos;
        max = rankedStartPos;
    }
    for (size_t i = min; i < max; i++)
        if (getBit(binaryRep, i))
            return 1;

    return 0;
}

int64_t findPath(size_t *startPos, size_t *endPos, uint8_t *binaryRep) {
    size_t rankedStartPos = rankCube(startPos);
    size_t rankedEndPos = rankCube(endPos);
    size_t *dimensions = getDimensions();

    if (isNoWayOneDim(rankedStartPos, rankedEndPos, binaryRep))
        return -1;

    visited = binaryRep;

    mod0Stack = malloc(sizeof(stack));
    mod1Stack = malloc(sizeof(stack));
    init(mod0Stack);
    init(mod1Stack); // TODO specialize init

    setBit(&visited, rankedStartPos, 1);
    push(mod0Stack, rankedStartPos);

    stack *stacks[2] = {mod0Stack, mod1Stack};

    int foundPath = 0;
    int mod = 0;

    size_t pos;
    int64_t pathLength = -1;

#ifdef DEBUG_LABYRINTH
    printf("# rankedStartPos = %d\n", rankedStartPos);
    printf("# rankedEndPos = %d\n", rankedEndPos);
#endif

    while (!foundPath && !isEmpty(stacks[mod])) {
        while (!isEmpty(stacks[mod])) {
            pos = pop(stacks[mod]);
            if (pos == rankedEndPos) {
                foundPath = 1;
                empty(mod0Stack);
                empty(mod1Stack);
            } else
                expand(pos, dimensions, mod);
        }
        mod = 1 - mod;
        pathLength++;
    }

    if (!foundPath)
        return -1;
    else
        return pathLength;
}