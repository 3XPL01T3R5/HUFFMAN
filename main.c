#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treeQueue.h"
#include "listFrequence.h"
#include "dictionaryTable.h"


int isBitSet(unsigned char c, int i){
    return c & (1 << i);
}
unsigned char setBit(unsigned char c, int i){
    unsigned char mask = (unsigned char) (1 << i);
    return mask | c;
}

void formDictionary(treeQueue* tree, char path[], int idx, dictionaryTable** dt){
    if(tree == NULL) return;
    if(treeQueue_getLeft(tree) == NULL && treeQueue_getRight(tree) == NULL){
        path[idx] = '\0';
        dictionaryTable_setDictionary((*dt), treeQueue_getByte(tree), (unsigned char *) path);
    }else{
        if(treeQueue_getLeft(tree) != NULL){
            path[idx] = '0';
            formDictionary(treeQueue_getLeft(tree), path, idx+1, dt);
        }
        if(treeQueue_getRight(tree) != NULL){
            path[idx] = '1';
            formDictionary(treeQueue_getRight(tree), path, idx+1, dt);
        }
    }
}

int main() {
    FILE * pFile, *newFile;
    long lSize;
    int tSize;
    unsigned char *buffer, *newBuffer = NULL, *header = NULL;
    size_t result;
    treeQueue* tree = NULL;
    listFrequence* lf = listFrequence_create();


    pFile = fopen ("text.txt" , "rb");
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
        if(listFrequence_getFrequenceAtIdx(lf, (unsigned char) i) == 0) {
            break;
        }
        treeQueue_enqueue(&tree, listFrequence_getByteAtIndex(lf, (unsigned char) i), listFrequence_getFrequenceAtIdx(lf, (unsigned char) i));
    }

    treeQueue_formTree(&tree); //tree

    char *preOrdemTree = treeQueue_printTreePreorder(tree);
    char path[8];

    //printf("%s\n", treeQueue_printTreePreorder(tree));

    tSize = strlen(preOrdemTree);
    dictionaryTable *dt = dictionaryTable_create();
    formDictionary(tree, path, 0, &dt);
    newBuffer = realloc(buffer, 1024 * sizeof(unsigned char));
    int cur = 0;
    for(i = 0; i < lSize; i++){
        unsigned char compressedByte[8] = {'\0'};
        strcpy((char *) compressedByte, (const char *) dictionaryTable_getDictionary(dt, buffer[i]));
        for(int j = 0; j < 8 && compressedByte[j] != '\0'; j++) {
            if (cur % 1024 == 0)
                newBuffer = realloc(newBuffer, (cur + 1024) * sizeof(unsigned char));
            newBuffer[cur++] = compressedByte[j];
        }
    }
    header = malloc(sizeof(unsigned char)*(2 + tSize));

    int trash = 8 - cur%8;
    char trashBin[3], tSizeBin[13];

    for(i = 0; i < 3; i++){
        trashBin[2-i] = (char) ((trash % 2) + '0');
        trash /= 2;
    }
    trash = tSize;
    for(i = 0; i < 13; i++){
        tSizeBin[12-i] = (char) ((trash % 2) + '0');
        trash /= 2;
    }

    for(i = 0; i < 3; i++){
        if(trashBin[i] == '1')
            setBit(header[0], i);
    }
    for(i = 3; i < 16; i++){
        if(tSizeBin[i-3] == '1')
            setBit(header[i < 8 ? 0 : 1], i);
    }

    for(i = 0; i < tSize; i++){
        header[i+2] = (unsigned char) preOrdemTree[i];
    }


    newFile = fopen("newArchive.huff", "wb");
    fwrite(header, sizeof(char), (size_t) tSize + 2, newFile);
    fwrite (newBuffer , sizeof(char), (size_t) cur, newFile);
    fclose(newFile);
    free (buffer);

    getchar();
    return 0;
}