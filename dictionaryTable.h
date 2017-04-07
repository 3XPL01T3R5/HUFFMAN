#ifndef HUFFMAN_DICTIONARYTABLE_H
#define HUFFMAN_DICTIONARYTABLE_H

typedef struct dictionary dictionaryTable;
dictionaryTable* dictionaryTable_create();
void dictionaryTable_setDictionary(dictionaryTable* dt, unsigned char byte, unsigned char bits[8]);
unsigned char* dictionaryTable_getDictionary(dictionaryTable *dt, unsigned char byte);

//char* newFileBits(dictionaryTable* dt, )

#endif //HUFFMAN_DICTIONARYTABLE_H
