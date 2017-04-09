#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "treeQueue.h"

struct treeQueue{
    unsigned char byte;
    long long int frequence;
    treeQueue *next, *left, *right;
};

treeQueue* treeQueue_createNode(unsigned char byte, long long int frequence){
    treeQueue *newNode = malloc(sizeof(treeQueue));
    newNode->byte = byte;
    newNode->frequence = frequence;
    newNode->next = NULL;
    newNode->left = NULL;
    newNode->right = NULL;

    return newNode;
}

void treeQueue_enqueue(treeQueue **tree, unsigned char byte, long long int frequence){
    treeQueue *newNode = treeQueue_createNode(byte, frequence);
    newNode->next = *tree;
    *tree = newNode;
}

void treeQueue_enqueueMergedNode(treeQueue **tree, treeQueue *mergedNode){
    if(*tree == NULL)
        *tree = mergedNode;
    else{
        treeQueue *prev = NULL, *head = *tree;
        while(head && head->frequence < mergedNode->frequence) {
            prev = head;
            head = head->next;
        }
        if(prev == NULL){
            mergedNode->next = *tree;
            *tree = mergedNode;
        }
        else if(head == NULL)
            prev->next = mergedNode;
        else{
            prev->next = mergedNode;
            mergedNode->next = head;
        }
    }
}

void treeQueue_formTree(treeQueue **tree){
    if(*tree == NULL)
        return;
    while((*tree)->next){
        long long int freq = (*tree)->frequence + (*tree)->next->frequence;
        treeQueue *mergedNode = treeQueue_createNode('*', freq);

        mergedNode->next = NULL;
        mergedNode->left = *tree;
        mergedNode->right = (*tree)->next;

        *tree = (*tree)->next->next;
        mergedNode->left->next = NULL;
        mergedNode->right->next = NULL;

        treeQueue_enqueueMergedNode(tree, mergedNode);
        treeQueue_printQueue(*tree);
    }
}

void treeQueue_printQueue(treeQueue *tree){
    for(; tree; tree = tree->next)
        printf("(%c|%lld)->", tree->byte, tree->frequence);
    printf("\n\n");
}

char* treeQueue_printTreePreorder(treeQueue *tree) {
    if (tree == NULL)
        return "\0";

    char *str1 = treeQueue_printTreePreorder(tree->left);

    char *str2 = treeQueue_printTreePreorder(tree->right);

    char *str = NULL;
    if((tree->byte == '*' || tree->byte == '\\') && treeQueue_isLeafNode(tree)){
        str = malloc((3 + strlen(str1) + strlen(str2)) * sizeof(char));
        str[0] = '\\'; str[1] = tree->byte; str[2] = '\0';
    }
    else {
        str = malloc((2 + strlen(str1) + strlen(str2)) * sizeof(char));
        str[0] = tree->byte; str[1] = '\0';
    }
    strcat(str, str1);
    strcat(str, str2);
    return str;
}

treeQueue* treeQueue_createWildCardNode(unsigned char byte, long long int frequence, treeQueue* left, treeQueue* right) {
    treeQueue *newNode = malloc(sizeof(treeQueue));
    newNode->byte = byte;
    newNode->frequence = frequence;
    newNode->next = NULL;
    newNode->left = left;
    newNode->right = right;
    return newNode;
}
treeQueue* treeQueue_getLeft(treeQueue* tree) {
    return tree->left;
}
treeQueue* treeQueue_getRight(treeQueue* tree) {
    return tree->right;
}

unsigned char treeQueue_getByte(treeQueue* tree) {
    return tree->byte;
}
int treeQueue_isLeafNode(treeQueue* tree) {
    return tree->left == NULL && tree->right == NULL;
}