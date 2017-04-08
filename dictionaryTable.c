#include <stdlib.h>
#include <string.h>
#include "dictionaryTable.h"

struct dictionary{
    unsigned char array[256][9];
};

dictionaryTable* dictionaryTable_create(){
    dictionaryTable* new_dt = (dictionaryTable*) malloc(sizeof(dictionaryTable));
    int i;
    for(i = 0; i < 256; i++)
        memset(new_dt->array[i], '\0', 9);
    return new_dt;
}

void dictionaryTable_setDictionary(dictionaryTable* dt, unsigned char byte, unsigned char bits[9]){
    strcpy((char *) dt->array[byte], (const char *) bits);
}

unsigned char* dictionaryTable_getDictionary(dictionaryTable *dt, unsigned char byte){
    return dt->array[byte];
}