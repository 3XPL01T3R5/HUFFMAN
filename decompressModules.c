#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "treeQueue.h"
#include "decompressModules.h"
#include "stack.h"

struct header {
    char trashSize;
    short treeSize;
    treeQueue *tree;
};
unsigned char b[9];

int isBitSet(unsigned char c, int i) {
    return (c & (1 << i)) ? 1 : 0;
}

void byteToBin(unsigned char x) {
    b[0] = isBitSet(x, 7) + 48;
    b[1] = isBitSet(x, 6) + 48;
    b[2] = isBitSet(x, 5) + 48;
    b[3] = isBitSet(x, 4) + 48;
    b[4] = isBitSet(x, 3) + 48;
    b[5] = isBitSet(x, 2) + 48;
    b[6] = isBitSet(x, 1) + 48;
    b[7] = isBitSet(x, 0) + 48;
    b[8] = '\0';
}

void insertInTree(stack *pilha, unsigned char in, int isEscaped) {
    if (isEscaped) {
        treeQueue *node = treeQueue_createNode('*', 0);
        push(pilha, node);
        return;
    }
    if (in != '*') {
        treeQueue *node = treeQueue_createNode(in, 0);
        push(pilha, node);
        return;
    }
    treeQueue *node1 = pop(pilha), *node2 = pop(pilha);
    treeQueue *node = treeQueue_createWildCardNode(in, 0, node1, node2);
    push(pilha, node);
}

treeQueue *processPreOrderTree(unsigned char *pre, int tam) {
    stack *pilha = newStack();
    for (int i = tam - 1; i >= 0; i--) {
        int escaped = 0;
        if (pre[i] == '*' && i > 0 && pre[i - 1] == '\\') {
            if (i > 1 && pre[i - 2] == '\\') {
                if (i > 2 && pre[i - 3] == '\\') {
                    insertInTree(pilha, '*', 1);
                    insertInTree(pilha, pre[i - 3], 0);
                    i -= 3;
                    continue;
                }
                insertInTree(pilha, '*', 0);
                insertInTree(pilha, pre[i - 2], 0);
                i -= 2;
                continue;
            }
            escaped = 1;
            i--;
        }
        if (pre[i] == '\\' && i > 0 && pre[i - 1] == '\\')
            i--;
        insertInTree(pilha, pre[i], escaped);
    }
    return pop(pilha);
}

header *processHeader(FILE *file) {
    unsigned char *bin = malloc(2);
    fread(bin, 1, 2, file);
    header *cabecalho = malloc(sizeof(header));
    unsigned char trash[4], treesize[14];
    int pos = 0;
    trash[3] = '\0';
    for (int i = 7; i >= 5; i--)
        trash[pos++] = (unsigned char) (isBitSet(bin[0], i) + 48);
    pos = 0;
    for (int i = 4; i >= 0; i--)
        treesize[pos++] = (unsigned char) (isBitSet(bin[0], i) + 48);
    for (int i = 7; i >= 0; i--)
        treesize[pos++] = (unsigned char) (isBitSet(bin[1], i) + 48);
    treesize[13] = '\0';
    int tamtree = strtol((const char *) treesize, NULL, 2), tamtrash = strtol((const char *) trash, NULL, 2);
    bin = malloc((size_t) tamtree);
    cabecalho->trashSize = (char) tamtrash;
    fread(bin, 1, (size_t) tamtree, file);
    treeQueue *treee = processPreOrderTree(bin, tamtree);
    cabecalho->treeSize = (short) tamtree;
    cabecalho->tree = treee;
    return cabecalho;
}

void decompressFile(FILE *buf, FILE *destino) {
    fseek(buf, 0, SEEK_END);
    long long int tambuf = ftell(buf);
    rewind(buf);//..
    long long int progresstable[100];
    double prop = (double) tambuf / (double) 100, cont = 0;
    for (int i = 0; i < 100; i++) {
        progresstable[i] = (long long int) cont;
        cont += prop;
    }
    progresstable[99] = tambuf - 1;
    header *header = processHeader(buf);
    tambuf -= 2 + header->treeSize;
    unsigned char temp[256];
    temp[0] = '\0';
    unsigned char leu;
    int atual = 1;
    int tamtemp = 0;
    printf("BEGIN_PROGRESS\n");
    for (long long int i = 0; i < tambuf; i++) {
        leu = (unsigned char) getc(buf);
        if (i == tambuf - 1 && header->trashSize != 0) {
            unsigned char trashhandler[8];
            int bit = 7, j;
            for (j = 0; j < 8 - header->trashSize; j++)
                trashhandler[j] = (unsigned char) (isBitSet(leu, bit--) + 48);
            trashhandler[j] = '\0';
            strcat((char *) temp, (const char *) trashhandler);
            tamtemp += strlen((const char *) trashhandler);
        } else {
            byteToBin(leu);
            strcat((char *) temp, (const char *) b);
            tamtemp += 8;
        }
        treeQueue *root = header->tree, *now = root;
        for (int j = 0; j < tamtemp; j++) {
            if (temp[j] == '0')
                now = treeQueue_getLeft(now);
            else
                now = treeQueue_getRight(now);
            if (treeQueue_isLeafNode(now)) {
                putc(treeQueue_getByte(now), destino);
                int offset = j + 1;
                int tamnew = tamtemp - offset;
                memmove(temp, temp + offset, (size_t) tamnew);
                temp[tamnew] = '\0';
                tamtemp = tamnew;
                now = root;
                j = -1;
            }
        }
        if (i == progresstable[atual - 1]) {
            printf("%d\n", atual);
            fflush(stdout);
            atual++;
        }
    }
    printf("100\nEND_PROGRESS\n");
    fflush(stdout);
}


