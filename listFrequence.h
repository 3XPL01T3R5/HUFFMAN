#ifndef HUFFMAN_LISTFREQUENCE_H
#define HUFFMAN_LISTFREQUENCE_H

typedef struct list listFrequence;

/*
 * Creates a new listFrequence completely blank and returns it.
 */
listFrequence* listFrequence_create();

/*
 * Updates (increases) the frequency of the passed byte.
 */
void listFrequence_increaseFrequence(listFrequence*, unsigned char);

/*
 * Returns the frequency of the byte stored at specified index of passed listFrequence.
 */
long long int listFrequence_getFrequenceAtIdx(listFrequence*, unsigned char);

/*
 * Returns the byte stored at specified index of the passed listFrequence.
 */
unsigned char listFrequence_getByteAtIndex(listFrequence*, unsigned char);

#endif //HUFFMAN_LISTFREQUENCE_H
