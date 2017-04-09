#ifndef HUFFMAN_LISTFREQUENCE_H
#define HUFFMAN_LISTFREQUENCE_H

typedef struct list listFrequence;
listFrequence* listFrequence_create();
void listFrequence_increaseFrequence(listFrequence *list, unsigned char byte); //NAO DEVE SER EXECUTADO DPS DE UM SORT!
long long int listFrequence_getFrequenceAtIdx(listFrequence *list, unsigned char idx);
unsigned char listFrequence_getByteAtIndex(listFrequence *list, unsigned char idx);
//void listFrequence_sort(listFrequence *list);

#endif //HUFFMAN_LISTFREQUENCE_H
