#include <stdio.h>
#include <stdlib.h>
#include "treeQueue.h"

int main() {
    treeQueue *tree = NULL;

    treeQueue_enqueue(&tree, 'a', 10);
    treeQueue_enqueue(&tree, 'b', 15);
    treeQueue_print(tree);
    treeQueue_enqueue(&tree, 'c', 5);
    treeQueue_print(tree);
    treeQueue_enqueue(&tree, 'd', 10);
    treeQueue_print(tree);
    treeQueue_enqueue(&tree, 'e', 1);
    treeQueue_print(tree);
    treeQueue_enqueue(&tree, 'f', 27);
    treeQueue_print(tree);
    treeQueue_enqueue(&tree, 'g', 20);
    treeQueue_print(tree);

    getchar();
    return 0;
}