#ifndef HUFFMAN_HEADERPROCESSING_H
#define HUFFMAN_HEADERPROCESSING_H

typedef struct header header;
/*
 *  Decompresses the "buf FILE" into the "destino FILE".
 */
void decompressFile(FILE*, FILE*);

#endif //HUFFMAN_HEADERPROCESSING_H
