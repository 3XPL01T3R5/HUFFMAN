#ifndef HUFFMAN_QUEUE_H
#define HUFFMAN_QUEUE_H
#include "treeQueue.h"

typedef struct no node;
typedef struct stack stack;
stack *newStack();
void push(stack*, treeQueue*);
treeQueue *pop(stack*);
#endif //HUFFMAN_QUEUE_H
