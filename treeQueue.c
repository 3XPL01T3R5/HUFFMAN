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

treeQueue* treeQueue_createWildCardNode(unsigned char byte, long long int frequence, treeQueue* left, treeQueue* right) {
    treeQueue *newNode = malloc(sizeof(treeQueue));
    newNode->byte = byte;
    newNode->frequence = frequence;
    newNode->next = NULL;
    newNode->left = left;
    newNode->right = right;
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
    if((*tree)->next == NULL)
        (*tree) = treeQueue_createWildCardNode('*', (*tree)->frequence, (*tree), NULL);
    else {
        while ((*tree)->next) {
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
}

int treeQueue_isLeafNode(treeQueue* tree) {
    return tree->left == NULL && tree->right == NULL;
}
int height = 0;
void treeQueue_printTreePreorder(treeQueue *tree, FILE *file) {
    if (tree == NULL)
        return;
    height++;
    if(treeQueue_isLeafNode(tree) && (tree->byte == '*' || tree->byte == '\\')) {
        putc('\\', file);
        height++;
    }
    putc(tree->byte, file);
    treeQueue_printTreePreorder(tree->left, file);
    treeQueue_printTreePreorder(tree->right, file);
}

int getHeightTree(){
    return height;
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

void FrontBackSplit(treeQueue* source, treeQueue** frontRef, treeQueue** backRef) {
    treeQueue* fast;
    treeQueue* slow;
    if (source==NULL || source->next==NULL)
    {
        *frontRef = source;
        *backRef = NULL;
    }
    else
    {
        slow = source;
        fast = source->next;

        while (fast != NULL)
        {
            fast = fast->next;
            if (fast != NULL)
            {
                slow = slow->next;
                fast = fast->next;
            }
        }

        *frontRef = source;
        *backRef = slow->next;
        slow->next = NULL;
    }
}

treeQueue* SortedMerge(treeQueue* a, treeQueue* b)
{
    treeQueue* result = NULL;

    if (a == NULL)
        return(b);
    else if (b==NULL)
        return(a);

    if (a->byte <= b->byte)
    {
        result = a;
        result->next = SortedMerge(a->next, b);
    }
    else
    {
        result = b;
        result->next = SortedMerge(a, b->next);
    }
    return(result);
}

void treeQueue_sort(treeQueue **tree){
    treeQueue* head = *tree;
    treeQueue* a;
    treeQueue* b;

    if ((head == NULL) || (head->next == NULL)) {
        return;
    }

    FrontBackSplit(head, &a, &b);

    treeQueue_sort(&a);
    treeQueue_sort(&b);

    *tree = SortedMerge(a, b);
}