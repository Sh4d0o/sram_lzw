/**
 * author: shadolaptop
 * created: 18-03-2022
 * project: File compression (LZWd algorithm)
 **/

#include "lzwd_lib.h"
#define DICT_SIZE 4096

int pattern_exists(d_entry *d_entries, char *pattern, int pattern_size);
char *my_concat(char *pattern, char new_char, int n);

/**
 * Hash function
 * @param key string to be hashed
 **/
int hash(char *key) {
    int length = strlen(key);
    unsigned int hash_value = 0;
    for (int i = 0; i < length; i++) {
        hash_value += key[i];
        hash_value = (hash_value * key[i]) % DICT_SIZE;
    }
    return hash_value;
}
/**
 * Encode a given buffer of bytes in to an output buffer using LZWD algorithm.
 * @param buffer_in buffer to read from
 * @param buffer_out buffer to write to
 * @param nbytes number of bytes to process from buffer_in
 **/
int encode_lzwd(char *buffer_in, int nbytes, char *buffer_out) {

    // init dictionary containing all single chars in ASCII.
    d_entry *d_entries[DICT_SIZE];
    int nextIndex = 256; // index of first pattern after initializing dictionary
    for (int i = 0; i < 255; i++) {
        char c = i;
        d_entry *new = malloc(sizeof(d_entry));
        new->key = i;
        new->length = 1;
        // new->value = malloc(sizeof(char));
        new->value = &c;

        // int index = hash(new->value);
        d_entries[i] = new;
    }
    printf("Dictionary Initialized.\n");
    for (int p = 0; p < 255; p++) {
        printf("%d||%s\n", d_entries[p]->key, d_entries[p]->value);
    }
    int N = 0;         // apontador de leitura
    int out_bytes = 0; // tamanho escrito no buffer_out
    // char Pj = buffer_in[N++];
    // char Pk = buffer_in[N++];
    // printf("Pj: %c\nPk: %c\n", Pj, Pk);
    // char Pm[3];
    // Pm[0] = Pj;
    // Pm[1] = Pk;
    // Pm[2] = '\0';
    // printf("Pm: %s\n", Pm);

    // 1. Ler Pj apartir de N até padrão não existir ou chegar ao final do ficheiro.
    int exist_j = 1;      // exit condition flag
    int size_j = 1;       // pattern size
    char *Pj = malloc(2); // shortest pattern possible
    Pj[0] = buffer_in[N++];
    // searching string until pattern doesnt exist
    while (exist_j == 1) {
        // 2.concat new simbol to pattern
        Pj[size_j++] = buffer_in[N++];

        // 3.check if new pattern exists
        exist_j = pattern_exists(*d_entries, Pj, size_j);

        if (exist_j == 0) { // not found
            // add pattern to dict
            // int entry_size = size_j + (2 * sizeof(int)); // int key + int length
            // d_entry *new_entry = malloc(entry_size);
            d_entry *new_entry = malloc(sizeof(d_entry));
            new_entry->key = nextIndex++;
            new_entry->length = size_j;
            new_entry->value = malloc(size_j);

            // int hash_index = hash(new_entry->key);
            d_entries[nextIndex - 1] = new_entry;
            // save Pj to output
            for (int i = 0; i < size_j; i++) {
                buffer_out[out_bytes] = Pj[i];
                out_bytes++;
            }
        }

        // testing stuff
        if (size_j == 5) {
            exist_j = 0;
        }
    }

    // // ler Pk aseguir ao final de Pj até padrão não existir ou chegar ao final do ficheiro.
    // int pattern_k = 1;
    // while (pattern_k) {
    //     Pj = my_concat(Pj, buffer_in[N++]);
    //     pattern_j = pattern_exist(Pj);
    // }
    // // Pm = Pj+Pk, save Pm como novo padrão, i++

    // // escrever Pj no buffer_out, N = inicio de Pk

    // // voltar a ler Pj e Pk

    out_bytes = 1024;
    return out_bytes;
}

/**
 * Prints debug text in yellow color when debugging flag is on.
 * @param debug debug flag value
 * @param message message to print
 **/
void pdebug(int debug, char *message) {
    if (debug) {
        printf(DEBUG_TXT "%s" RESET_TXT, message);
    }
}

/**
 * Concatenate a pattern with a character.
 * @param pattern first string
 * @param new_char second string
 * @param n size of pattern
 **/
// char *my_concat(char *pattern, char new_char, int n) {
//     char *result = malloc(n + 1);
//     strcpy(result, pattern);
//     strcat(result, new_char);
//     return result;
// }

void my_array_copy(unsigned char *from, unsigned char *to, int size) {
    for (int i = 0; i < size; i++) {
        to[i] = from[i];
    }
}

int pattern_exists(d_entry *d_entries, char *pattern, int pattern_size) {
    return 1;
}