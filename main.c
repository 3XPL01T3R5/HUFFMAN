#include <stdio.h>
#include <stdlib.h>

typedef struct treeQueue treeQueue;
treeQueue* treeQueue_createNode(unsigned char byte, long long int frequence);
void treeQueue_enqueue(treeQueue **tree, unsigned char byte, long long int frequence);

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
        while(*tree && (*tree)->frequence < frequence){
            *tree = (*tree)->next;
        }
        treeQueue *newNode = treeQueue_createNode(byte, frequence);
        newNode->next = (*tree);
        (*tree)->next = newNode;
    }
}

void treeQueue_print(treeQueue *tree){
    for(; tree; tree = tree->next){
        printf("%c - %lld\n", tree->byte, tree->frequence);
    }
}

int main() {
    treeQueue *tree = NULL;

    treeQueue_enqueue(&tree, 'a', 10);
    treeQueue_enqueue(&tree, 'b', 15);
    /*treeQueue_enqueue(&tree, 'c', 5);
    /*treeQueue_enqueue(&tree, 'd', 10);
    treeQueue_enqueue(&tree, 'e', 1);
    treeQueue_enqueue(&tree, 'f', 27);
*/
    treeQueue_print(tree);
    getchar();
    return 0;
}