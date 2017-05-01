#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "listFrequence.h"
#include "treeQueue.h"
#include "dictionaryTable.h"
#include "compressModules.h"

unsigned char setBit(unsigned char c, int i) {
    unsigned char mask = (unsigned char) (1 << i);
    return mask | c;
}

void formDictionary(treeQueue *tree, char path[], int idx, dictionaryTable **dt) {
    if (tree == NULL) return;
    if (treeQueue_getLeft(tree) == NULL && treeQueue_getRight(tree) == NULL) {
        /*if (idx == 0)
            path[idx++] = '0';*/
        path[idx] = '\0';
        dictionaryTable_setDictionary((*dt), treeQueue_getByte(tree), (unsigned char *) path);
    } else {
        if (treeQueue_getLeft(tree) != NULL) {
            path[idx] = '0';
            formDictionary(treeQueue_getLeft(tree), path, idx + 1, dt);
        }
        if (treeQueue_getRight(tree) != NULL) {
            path[idx] = '1';
            formDictionary(treeQueue_getRight(tree), path, idx + 1, dt);
        }
    }
}

long long int sizeFile(FILE *file) {
    long long int size;
    fseek(file, 0, SEEK_END);
    size = ftell(file);
    rewind(file);
    return size;
}

void fileData(FILE *buffer, long long int bSize, FILE *destiny, int *trash, dictionaryTable *dt, long long int tambuf) {
    long long int progresstable[50], idxpg = 0;
    double prop = (double) tambuf / (double) 50, cont = 0;
    for (int i = 0; i < 50; i++) {
        progresstable[i] = (long long int) cont;
        cont += prop;
    }
    progresstable[49] = tambuf - 1;
    int idx = 0;
    unsigned char str[8], newByte, temp;
    unsigned char compressedByte[300];

    for (long long int i = 0; i < bSize; i++) {
        compressedByte[0] = '\0';
        temp = (unsigned char) getc(buffer);
        strcpy((char *) compressedByte, (const char *) dictionaryTable_getDictionary(dt, temp));

        for (int j = 0; j < 300 && compressedByte[j] != '\0'; j++) {
            str[idx] = compressedByte[j];
            ++idx;
            if (idx == 8) {
                idx = 0;
                newByte = '\0';
                for (int k = 0; k < 8; k++)
                    if (str[k] == '1')
                        newByte = setBit(newByte, 7 - k);
                putc(newByte, destiny);

            }
        }

        if (progresstable[idxpg] == i) {
            printf("%lld\n", idxpg + 51);
            fflush(stdout);
            idxpg++;
        }
    }
    printf("100\nEND_PROGRESS\n");
    fflush(stdout);
    (*trash) = 0;
    if (idx > 0) {
        (*trash) = 8 - idx;
        newByte = 0;
        for (int i = 0; i < 8 - (*trash); i++)
            if (str[i] == '1')
                newByte = setBit(newByte, 7 - i);
        putc(newByte, destiny);
    }
}

void fileHeader(FILE *destiny, int trash, int tSize) {
    unsigned char header[2];
    memset(header, 0, (size_t) 2);

    char trashBin[4], tSizeBin[14];
    trashBin[3] = '\0';
    tSizeBin[13] = '\0';

    if (trash != 8 && trash != 0)
        for (int i = 0; i < 3; i++) {
            trashBin[2 - i] = (char) ((trash % 2) + '0');
            trash /= 2;
        }
    else
        memset(trashBin, '0', 3);

    for (int i = 0; i < 13; i++) {
        tSizeBin[12 - i] = (char) ((tSize % 2) + '0');
        tSize /= 2;
    }

    for (int i = 0; i < 3; i++) {
        if (trashBin[i] == '1')
            header[0] = setBit(header[0], 7 - i);
    }

    for (int i = 3; i < 16; i++) {
        if (i < 8) {
            if (tSizeBin[i - 3] == '1')
                header[0] = setBit(header[0], 7 - i);
        } else {
            if (tSizeBin[i - 3] == '1')
                header[1] = setBit(header[1], 15 - i);
        }
    }

    rewind(destiny);
    putc(header[0], destiny);
    putc(header[1], destiny);
}

void compressFile(FILE *buffer, FILE *compressed) {
    int i;
    long long int tambuf = sizeFile(buffer);
    long long int bSize = 0;

    long long int progresstable[50], idxpg = 0;
    double prop = (double) tambuf / (double) 50, cont = 0;
    for (i = 0; i < 50; i++) {
        progresstable[i] = (long long int) cont;
        cont += prop;
    }
    progresstable[49] = tambuf - 1;

    listFrequence *lf = listFrequence_create();
    int temp;
    printf("BEGIN_PROGRESS\n");
    while ((temp = getc(buffer)) != EOF) {
        if (progresstable[idxpg] == bSize) {
            printf("%lld\n", idxpg + 1);
            fflush(stdout);
            idxpg++;
        }

        bSize++;
        listFrequence_increaseFrequence(lf, (unsigned char) temp);
    }
    rewind(buffer);
    treeQueue *tree = NULL;
    int tSize = 0;
    for (i = 255; i >= 0; i--) {
        if (listFrequence_getFrequenceAtIdx(lf, (unsigned char) i) != 0) {
            treeQueue_enqueue(&tree, listFrequence_getByteAtIndex(lf, (unsigned char) i),
                              listFrequence_getFrequenceAtIdx(lf, (unsigned char) i));
        }
    }
    treeQueue_sort(&tree);

    //treeQueue_printQueue(tree);
    treeQueue_formTree(&tree);
    fwrite("\0\0", 1, 2, compressed);

    treeQueue_printTreePreorder(tree, compressed);
    tSize = getHeightTree();

    char path[300];

    dictionaryTable *dt = dictionaryTable_create();
    formDictionary(tree, path, 0, &dt);
    int trash = 0;
    fileData(buffer, bSize, compressed, &trash, dt, tambuf);

    fileHeader(compressed, trash, tSize);
}