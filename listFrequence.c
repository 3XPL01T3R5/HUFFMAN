#include <stdlib.h>
#include "listFrequence.h"

struct node{
    unsigned char byte;
    long long int frequence;
};

struct list{
    struct node array[256];
};

listFrequence* listFrequence_create(){
    listFrequence *newList = malloc(sizeof(listFrequence));
    unsigned char i;
    for(i = 0; i < 256; i++){
        newList->array[i].byte = i;
        newList->array[i].frequence = 0;
    }
    return newList;
}

void listFrequence_setFrequence(listFrequence *list, unsigned char byte, long long int frequence){
    if(list->array[byte].byte == byte)
        list->array[byte].frequence = frequence;
}

long long int listFrequence_getFrequence(listFrequence *list, unsigned char byte){
    if(list->array[byte].byte == byte)
        return list->array[byte].frequence;
    return -1;
}

void swap(struct node *a, struct node *b){
    struct node aux = *a; *a = *b; *b = aux;
}

void quicksort(struct node array[], unsigned char idxi, unsigned char idxf){
    if(idxf - idxi < 1)
        return;
    long long int pivot = array[(idxf + idxi)/2].frequence;
    unsigned char left = idxi;
    unsigned char right = idxf;

    while(left <= right){
        while(array[left].frequence < pivot && left < idxf)
            ++left;
        while(array[right].frequence > pivot && right > idxi)
            --right;

        if(left <= right) {
            swap(&array[left], &array[right]);
            ++left; --right;
        }
    }

    quicksort(array, idxi, right);
    quicksort(array, left, idxf);
}

void listFrequence_sort(listFrequence *list){
    quicksort(list->array, 0, 255);
}