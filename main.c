#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include "treeQueue.h"
#include "listFrequence.h"

int isBitSet(unsigned char c, int i){
    return c & (1 << i);
}


int main() {
    /*treeQueue *tree = NULL;
    printf("opa: %s\n", treeQueue_printTreePreorder(tree));

    treeQueue_enqueue(&tree, 'f', 27);
    treeQueue_enqueue(&tree, 'g', 20);
    treeQueue_enqueue(&tree, 'b', 15);
    treeQueue_enqueue(&tree, 'd', 10);
    treeQueue_enqueue(&tree, 'a', 10);
    treeQueue_enqueue(&tree, 'c', 5);
    treeQueue_enqueue(&tree, 'e', 1);
    treeQueue_printQueue(tree);

    treeQueue_formTree(&tree);
    treeQueue_printQueue(tree);
    printf("%s\n", treeQueue_printTreePreorder(tree));

   */

    FILE * pFile, *newFile;
    long lSize;
    unsigned char * buffer;
    size_t result;
    treeQueue* tree = NULL;
    listFrequence* lf = listFrequence_create();


    pFile = fopen ("imagem.jpg" , "rb");
    if (pFile==NULL) {fputs ("File error",stderr); exit (1);}

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (unsigned char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {fputs ("Memory error",stderr); exit (2);}

    // copy the file into the buffer:
    result = fread (buffer, 1, (size_t) lSize, pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}
    fclose (pFile);
    int i;
    for(i = 0; i < lSize; i++){
        listFrequence_increaseFrequence(lf, buffer[i]);
    }

    listFrequence_sort(lf);

    for(i = 255; i >= 0; i--){
        if(listFrequence_getFrequenceAtIdx(lf, i) == 0) {
            break;
        }
        treeQueue_enqueue(&tree, listFrequence_getByteAtIndex(lf, i), listFrequence_getFrequenceAtIdx(lf, i));
    }

    treeQueue_formTree(&tree);
    printf("%s\n", treeQueue_printTreePreorder(tree));

    /*
    newFile = fopen("teste.mp4", "wb");
    fwrite (buffer , sizeof(char), (size_t) lSize, newFile);
    fclose(newFile);
    free (buffer);
*/
    getchar();
    return 0;
}