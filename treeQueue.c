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

void treeQueue_print(treeQueue *tree){
    for(; tree; tree = tree->next)
        printf("%c - %lld\n", tree->byte, tree->frequence);
    printf("\n\n");
}