#ifndef HUFFMAN_TREEQUEUE_H
#define HUFFMAN_TREEQUEUE_H
#include <stdio.h>

typedef struct treeQueue treeQueue;

/*
 * Creates a new node, sets its byte and frequency as the both passed byte and frequency. And returns it.
 */
treeQueue *treeQueue_createNode(unsigned char, long long int);

/*
 * Creates a new node, sets its byte, frequency and childs as the both passed byte, frequency and childs. And returns it.
 */
treeQueue *treeQueue_createWildCardNode(unsigned char, long long int, treeQueue*, treeQueue*);

/*
 * Enqueues the passed byte with its frequency in the passed treeQueue.
 */
void treeQueue_enqueue(treeQueue**, unsigned char, long long int);

/*
 * Prints in the file FILE* the treeQueue in preorder format.
 */
void treeQueue_printTreePreorder(treeQueue*, FILE*);

/*
 * Transforms the queue of the passed treeQueue into a tree.
 */
void treeQueue_formTree(treeQueue**);

/*
 * Returns the left child of the passed treeQueue.
 */
treeQueue *treeQueue_getLeft(treeQueue*);

/*
 * Returns the right child of the passed treeQueue.
 */
treeQueue *treeQueue_getRight(treeQueue*);

/*
 * Returns the byte stored in the passed treeQueue node.
 */
unsigned char treeQueue_getByte(treeQueue*);

/*
 * Returns 1 if the passed treeQueue node is a leaf node and returns 0 otherwise.
 */
int treeQueue_isLeafNode(treeQueue*);

/*
 * Sorts the queue of the passed treeQueue.
 */
void treeQueue_sort(treeQueue**);

/*
 * Returns the length of the printed preorder treeQueue.
 */
int getHeightTree();

#endif //HUFFMAN_TREEQUEUE_H
