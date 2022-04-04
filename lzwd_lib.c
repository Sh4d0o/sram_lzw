/**
 * author: shadolaptop
 * created: 18-03-2022
 * project: File compression (LZWd algorithm)
 **/

#include "lzwd_lib.h"

void pdebug(int debug, char *message) {
    if (debug) {
        printf(DEBUG_TXT "%s" RESET_TXT, message);
    }
}

char *my_concat(const char *s1, const char *s2) {
    char *result = malloc(strlen(s1) + strlen(s2) + 1);
    strcpy(result, s1);
    strcat(result, s2);
    return result;
}

void my_array_copy(unsigned char *from, unsigned char *to, int size) {
    for (int i = 0; i < size; i++) {
        to[i] = from[i];
    }
}

void build_byte(unsigned char *byte, int bits[]) {
    int tempValue = bits[0];

    for (int i = 7; i > -1; i--) {
        bits[i] = tempValue & 1;
        tempValue = tempValue >> 1;
    }
}