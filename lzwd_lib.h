/**
 * author: shadolaptop
 * created: 18-03-2022
 * project: File compression (LZWd algorithm)
 **/

#ifndef LZWD_LIB
#define LZWD_LIB

// DEFINES
#define BLOCK_SIZE_DEFAULT 64000
#define DEBUG_TXT "\x1b[33m"
#define RESET_TXT "\x1b[0m"
#define USAGE_MSG "Usage: ./lzwd <filename-to-compress> [options]\nOptions:\n -d: debug mode\n -f: force rle encoding\n -s <block size>: reading block size. MIN: 64Kb\n"
#define DICT_SIZE 4096

extern int debugflag;
extern int sizeflag;
extern int textflag;

#include <getopt.h> //for cmd arguments parsing
#include <stdio.h>
#include <stdlib.h>
#include <string.h> //for file names concatenation
#include <time.h>   //for execution timing
#include <unistd.h>

typedef unsigned char byte; // from 0 to 255

// entrada no dicionario
typedef struct d_entry {
    int *key;             // pattern
    int value;            // pattern index in dictionary
    int length;           // pattern size
    struct d_entry *next; // next pattern pointer (for collisions)
} d_entry;

// dicionario
typedef struct dict {
    d_entry **entries;
} dict;

int hash(int *key, int size);
dict *create_dict();
d_entry *map_pair(int *key, int value, int size);
void dict_add(dict *dictionary, int *key, int value, int size);
int dict_get_value(dict *dictionary, int *key, int size);

void dict_print(dict *dictionary);
void dict_free(dict *dictionary);

int compare_pattern(int *pattern_x, int size_x, int *pattern_y, int size_y);
void concat_pattern(int *pattern_x, int size_x, int *pattern_y, int size_y, int *result);

int lzwd_encode(int *buffer_in, int nbytes, int *buffer_out);

void pdebug(int debug, char *message);

#endif