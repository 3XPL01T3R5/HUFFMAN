#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treeQueue.h"
#include "listFrequence.h"
#include "dictionaryTable.h"

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
    unsigned char *buffer, *newBuffer = NULL;
    size_t result;
    treeQueue* tree = NULL;
    listFrequence* lf = listFrequence_create();


    pFile = fopen ("arquivo.bin" , "rb");
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
    printf("%s\n", preOrdemTree);
    tSize = (int) strlen(preOrdemTree);
    dictionaryTable *dt = dictionaryTable_create();
    formDictionary(tree, path, 0, &dt);
    //newBufferBits = realloc(newBufferBits, 1024 * sizeof(unsigned char));

    //for(int j = 0; j < 256; j++)
        printf("%c - %s\n", 'r', dictionaryTable_getDictionary(dt, 'r'));
        printf("%c - %s\n", 'o', dictionaryTable_getDictionary(dt, 'o'));
        printf("%c - %s\n", 'l', dictionaryTable_getDictionary(dt, 'l'));
        printf("%c - %s\n", 'a', dictionaryTable_getDictionary(dt, 'a'));

    newBuffer = realloc(newBuffer, 1024 * sizeof(unsigned char));
    int idx = 0, newBytes = 0, trash = 0;
    unsigned char str[8];
    for(i = 0; i < lSize; i++){
        unsigned char compressedByte[9] = {'\0'};
        strcpy((char *) compressedByte, (const char *) dictionaryTable_getDictionary(dt, buffer[i]));
        for(int j = 0; j < 8 && compressedByte[j] != '\0'; j++){
            str[idx] = compressedByte[j];
            ++idx;
            if(idx == 8){
                idx = 0;
                newBuffer[newBytes] = 0;
                for(int k = 0; k < 8; k++) {
                    if (str[k] == '1')
                        newBuffer[newBytes] = setBit(newBuffer[newBytes], 7 - k);
                }
                printf("%d ", newBuffer[newBytes]);
                if(++newBytes%1024 == 0)
                    newBuffer = realloc(newBuffer, (newBytes + 1024) * sizeof(unsigned char));
            }
        }
    }
    printf("\n\n");
    while(idx > 0 && idx < 8){
        str[idx++] = '0';
        ++trash;
    }
    for(i = 0; i < 8; i++)
        if(str[i] == '1')
            newBuffer[newBytes] = setBit(newBuffer[newBytes], 7 - i);

    unsigned char header[2 + tSize];
    memset(header, 0, (size_t) (2 + tSize));

    char trashBin[4], tSizeBin[14];
    trashBin[3] = '\0'; tSizeBin[13] = '\0';
    //trash = 4;
    if(trash != 8) {
        for (i = 0; i < 3; i++) {
            trashBin[2 - i] = (char) ((trash % 2) + '0');
            trash /= 2;
        }
    }
    else
        memset(trashBin, '0', 3);
    printf("trashBin: %s\n", trashBin);
    trash = tSize;
    for(i = 0; i < 13; i++){
        tSizeBin[12-i] = (char) ((trash % 2) + '0');
        trash /= 2;
    }
    printf("tSize: %d | tSizeBin: %s\n", tSize, tSizeBin);

    for(i = 0; i < 3; i++){
        if(trashBin[i] == '1')
            header[0] = setBit(header[0], 7 - i);
    }
    printf("header(trashOnly): '%d'\n", header[0]);
    for(i = 3; i < 16; i++){
        if(i < 8) {
            if (tSizeBin[i - 3] == '1')
                header[0] = setBit(header[0], 7 - i);
        }
        else {
            if(tSizeBin[i-3] == '1')
                header[1] = setBit(header[1], 15 - i);
        }
    }
    printf("header: '%d''%d'\n", header[0], header[1]);

    for(i = 0; i < tSize; i++){
        header[i+2] = (unsigned char) preOrdemTree[i];
    }


    newFile = fopen("newArchive.huff", "wb");
    fwrite(header, sizeof(char), (size_t) tSize + 2, newFile);
    fwrite (newBuffer, sizeof(char), (size_t) newBytes, newFile);
    fclose(newFile);
    free (buffer);
    printf("comprimido com sucesso!\n");
    getchar();
    return 0;
}