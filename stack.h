#ifndef HUFFMAN_QUEUE_H
#define HUFFMAN_QUEUE_H
#include "treeQueue.h"

typedef struct stack stack;
/*
 * Creates a new stack
 */
stack *newStack();
/*
 * Puts the treeQueue* on the top of the stack*
 * */
void push(stack*, treeQueue*);
/*
 * Retrieves and removes the top item of the stack*
 */
treeQueue *pop(stack*);
#endif //HUFFMAN_QUEUE_H
