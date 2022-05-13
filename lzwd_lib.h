/**
 * author: shadolaptop
 * created: 18-03-2022
 * project: File compression (LZWd algorithm)
 **/

#ifndef LZWD_LIB
#define LZWD_LIB

// DEFINES
#define BLOCK_SIZE 64000
#define DEBUG_TXT "\x1b[33m"
#define RESET_TXT "\x1b[0m"
#define USAGE_MSG "Usage: ./lzwd <filename-to-compress> [options]\nOptions:\n -d: debug mode\n -f: force rle encoding\n -s <block size>: reading block size. MIN: 64Kb\n"

#include "stdio.h"
#include "stdlib.h"
#include "string.h" //for file names concatenation
#include "time.h"   //for execution timing
#include "unistd.h"

typedef unsigned char BYTE; // from 0 to 255

// entrada no dicionario
typedef struct d_entry {
    int key;
    char *value;
    int length;
} d_entry;

// dicionario
typedef struct dict {
    int nextIndex;
    d_entry *entries[4096];
} dict;

int encode_lzwd(char *buffer_in, int nbytes, char *buffer_out);

void pdebug(int debug, char *message);

/**
 * Costum function to copy array.
 *
 * @param from buffer to read from
 * @param to buffer to copy to
 * @param size number of bytes to copy
 **/
void my_array_copy(BYTE *from, BYTE *to, int size);
#endif