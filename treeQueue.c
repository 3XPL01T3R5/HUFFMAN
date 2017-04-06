#include <stdlib.h>
#include <stdio.h>
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

int treeQueue_queueSize(treeQueue *tree){
    int tam = 0;
    while(tree != NULL) {
        tree = tree->next;
        ++tam;
    }
    return tam;
}

void treeQueue_enqueue(treeQueue **tree, unsigned char byte, long long int frequence){
    if(*tree == NULL)
        *tree = treeQueue_createNode(byte, frequence);
    else{
        treeQueue *prev = NULL, *head = *tree, *newNode = treeQueue_createNode(byte, frequence);
        while(head && head->frequence < frequence) {
            prev = head;
            head = head->next;
        }
        if(prev == NULL){
            newNode->next = *tree;
            *tree = newNode;
        }
        else if(head == NULL)
            prev->next = newNode;
        else{
            prev->next = newNode;
            newNode->next = head;
        }
    }
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
    }
}

void treeQueue_print(treeQueue *tree){
    for(; tree; tree = tree->next)
        printf("%c - %lld\n", tree->byte, tree->frequence);
    printf("\n\n");
}