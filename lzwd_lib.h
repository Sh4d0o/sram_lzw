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

// typedef struct node {
//     BYTE code;
//     BYTE prefix; // or int index_of_prefix
//     BYTE byte;
//     struct node *left, *right;
// } DNode;

struct hm_element {
    int key;
    char *value;
};

struct dict {
    struct hm_element *dictionary[4096]
};
/**
 * Prints debug text in yellow color when debugging flag is on.
 *
 * @param debug debug flag value
 * @param message message to print
 **/
void pdebug(int debug, char *message);

/**
 * Concatenate two strings.
 *
 * @param s1 first string
 * @param s2 second string
 **/
char *my_concat(const char *s1, const char *s2);

/**
 * Costum function to copy array.
 *
 * @param from buffer to read from
 * @param to buffer to copy to
 * @param size number of bytes to copy
 **/
void my_array_copy(BYTE *from, BYTE *to, int size);

/**
 * COnstruct byte from 1's and 0's array.
 *
 * @param byte pointer to byte
 * @param bits array of 1 and 0
 **/
void build_byte(BYTE *byte, int bits[]);
#endif