#ifndef HUFFMAN_DICTIONARYTABLE_H
#define HUFFMAN_DICTIONARYTABLE_H

typedef struct dictionary dictionaryTable;

/*
 * Creates a new dictionaryTable, completely blank and returns it.
 */
dictionaryTable *dictionaryTable_create();

/*
 * Sets the passed string as the new value for the passed byte.
 */
void dictionaryTable_setDictionary(dictionaryTable*, unsigned char, unsigned char*);

/*
 * Returns the string corresponding to the passed byte.
 */
unsigned char *dictionaryTable_getDictionary(dictionaryTable*, unsigned char);

#endif //HUFFMAN_DICTIONARYTABLE_H
