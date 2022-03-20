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
    size_t count;
    node* front;
    node* rear;
};
typedef struct queue queue;

static void init(queue* q) {
    q->count = 0;
    q->front = NULL;
    q->rear = NULL;
}

static int isEmpty(queue* q) {
    return (q->rear == NULL);
}

static void push(queue* q, size_t value) {
    node* tmp;
    tmp = malloc(sizeof(node));
    tmp->data = value;
    tmp->next = NULL;
    if (!isEmpty(q)) {
        q->rear->next = tmp;
        q->rear = tmp;
    } else {
        q->front = q->rear = tmp;
    }
    q->count++;
}

static size_t pop(queue* q) {
    node* tmp;
    size_t n = q->front->data;

    tmp = q->front;
    q->front = q->front->next;
    q->count--;
    free(tmp);

    return n;
}

static unsigned char* visited;

static void tryToPush(size_t* currentPos, queue* states, unsigned char* binaryRepArray) {
    if (!isCubeFull(currentPos, binaryRepArray) && !visited[rankCube(currentPos)])
        push(states, rankCube(currentPos));
}

static void expand(size_t* currentPos, queue* states, unsigned char* binaryRepArray) {
    size_t dimNum = getDimNum();

    for (size_t i = 0; i < dimNum; i++)
        if (currentPos[i] < daGet(getDimensions(), i)) {
            currentPos[i]++; // Make one step forward in a dimension.
            tryToPush(currentPos, states, binaryRepArray);

            currentPos[i] -= 2; // Make one step backward.
            tryToPush(currentPos, states, binaryRepArray);

            currentPos[i]++; // Revert value modifications.
        }
}

size_t findPathLength(size_t* startPos, size_t* endPos, unsigned char* binaryRepArray) {
    visited = (unsigned char*) malloc(128*sizeof(size_t)); // ! rozmiar narazie roboczy
}
