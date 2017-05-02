#include <malloc.h>
#include <stdlib.h>
#include "stack.h"
typedef struct no node;
struct no {
    treeQueue *item;
    node *next;
};

struct stack {
    node *top;
};

stack *newStack() {
    stack *newstack = malloc(sizeof(stack));
    newstack->top = NULL;
    return newstack;
}

void push(stack *stack, treeQueue *item) {
    node * vem = malloc(sizeof(node));
    vem->item = item;
    vem->next = stack->top;
    stack->top = vem;
}

treeQueue *pop(stack *stack) {
    if (stack->top == NULL)
        return NULL;
    treeQueue * retorna = stack->top->item;
    node * oldref = stack->top;
    stack->top = stack->top->next;
    free(oldref);
    return retorna;
}