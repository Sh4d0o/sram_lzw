/**
 * author: shadolaptop
 * created: 18-03-2022
 * project: File compression (LZWd algorithm)
 **/

#include "lzwd_lib.h"

/**
 * Hash function.
 * @param key string to be hashed
 * @param size string size
 **/
int hash(int *key, int size) {
    unsigned int hash_value = 0;
    for (int i = 0; i < size; i++) {
        hash_value += key[i];
        hash_value = (hash_value * key[i]) % DICT_SIZE;
    }
    return hash_value;
}

/**
 * Creates new dictionary. Allocates memory for it. Sets 256 first entries.
 *
 * @return Pointer to dictionary
 **/
dict *create_dict() {
    dict *dictionary = malloc(sizeof(dict) * 1);
    dictionary->entries = malloc(sizeof(d_entry) * DICT_SIZE);

    // set all entries as empty
    for (int i = 0; i < DICT_SIZE; i++) {
        dictionary->entries[i] = -1;
    }

    // set first 256 entries
    int p[] = {0};
    for (int p_idx = 0; p_idx < 256; p_idx++) {
        p[0] = p_idx;
        dict_add(dictionary, p, p_idx, 1);
    }

    return dictionary;
}

/* key - pattern, value - pattern index, size -  pattern size*/
d_entry *map_pair(int *key, int value, int size) {

    d_entry *entry = malloc(sizeof(d_entry) * 1);
    entry->key = malloc(sizeof(int) * size);

    for (int i = 0; i < size; i++) {
        entry->key[i] = key[i];
    }

    entry->value = value;
    entry->length = size;
    entry->next = -1;

    return entry;
}

/* key - pattern, value - pattern index, size - pattern size*/
void dict_add(dict *dictionary, int *key, int value, int size) {

    unsigned int idx = hash(key, size);
    d_entry *new = dictionary->entries[idx];

    // entry empty so insert
    if (new == -1) {
        dictionary->entries[idx] = map_pair(key, value, size);
        return;
    }

    // otherwise an colission occured
    d_entry *prev;
    while (new != -1) {
        // next in chain
        prev = new;
        new = prev->next;
    }

    // end of chain reached, add new
    prev->next = map_pair(key, value, size);
}

/* key - pattern, value - pattern index, size - pattern size
return 0 if no match found or idx of matching pattern*/
int dict_get_value(dict *dictionary, int *key, int size) {

    unsigned int idx = hash(key, size);
    d_entry *d_search = dictionary->entries[idx];
    printf("searching:");
    for (int a = 0; a < size; a++) {
        printf("%d ", ((char *)key)[a]);
    }

    // no match found
    if (d_search == -1) {
        printf("Empty entry.\n");
        return 0;
    }

    // match found, walk the entries
    while (d_search != -1) {
        // check equal key
        if (compare_pattern(key, size, d_search->key, d_search->length) == 1) {
            printf("Match found. Idx: %d\n", d_search->value);
            return d_search->value;
        }
        // next in chain
        d_search = d_search->next;
    }

    // in case key hashes to existing slot but key doesnt match
    printf("No entries match.\n");
    return 0;
}

/**
 * Compare two patterns.
 * @param pattern_x first pattern
 * @param size_x size of 1st pattern
 * @param pattern_y second pattern
 * @param size_y size of 2nd pattern
 *
 * @return 0 if different, 1 if equal
 **/
int compare_pattern(int *pattern_x, int size_x, int *pattern_y, int size_y) {
    if (size_x != size_y) {
        return 0;
    }
    for (int i = 0; i < size_x; i++) {
        if (pattern_x[i] != pattern_y[i]) {
            return 0;
        }
    }
    return 1;
}

/**
 * Concatenate two patterns.
 * @param pattern_x first pattern
 * @param size_x size of 1st pattern
 * @param pattern_y second pattern
 * @param size_y size of 2nd pattern
 **/
int *concat_pattern(int *pattern_x, int size_x, int *pattern_y, int size_y) {
    int *result = malloc(size_x + size_y);
    for (int i = 0; i < size_x; i++) {
        result[i] = pattern_x[i];
    }
    for (int a = size_x, b = 0; b < size_y; a++, b++) {
        result[a] = pattern_y[b];
    }
    return result;
}

void dict_print(dict *dictionary) {
    for (int i = 0; i < DICT_SIZE; i++) {
        d_entry *d_entry = dictionary->entries[i];
        if (d_entry == -1)
            continue;
        printf("slot[%d] - idx[%d] - pattern[", i, d_entry->value);
        for (int a = 0; a < d_entry->length; a++) {
            printf("%d ", ((char *)d_entry->key)[a]);
        }
        printf("]\n");
    }
}

/**
 * Encode a given buffer of bytes in to an output buffer using LZWD algorithm.
 * @param buffer_in buffer to read from
 * @param buffer_out buffer to write to
 * @param nbytes number of bytes to process from buffer_in
 *
 * @returns number of bytes written to buffer_out
 **/
int lzwd_encode(int *buffer_in, int nbytes, int *buffer_out) {
    int N = 0; // apontador de leitura do bloco
    int M = 0; // apontador de escrita do output
    int nextIndex = 256;
    dict *dictionary = create_dict();
    printf("Dictionary Initializated.\n");
    dict_print(dictionary);
    // 1. Ler Pj apartir de N até padrão não existir ou chegar ao final do ficheiro.
    int exist_j = 1;          // exit condition flag
    int size_j = 1;           // pattern size
    int *Pj = malloc(nbytes); // longest pattern possible(full block)

    int exist_k = 1;          // exit condition flag
    int size_k = 1;           // pattern size
    int *Pk = malloc(nbytes); // longest pattern possible(full block)

    int *Pm = malloc(2 * nbytes);

    Pj[0] = buffer_in[N++];
    // searching string until pattern doesnt exist
    while (exist_j != 0 && N < nbytes) {
        // 2.concat new simbol to pattern
        Pj[size_j++] = buffer_in[N++];

        // 3.check if new pattern exists
        exist_j = dict_get_value(dictionary, Pj, size_j);

        if (exist_j == 0) { // not found
            N--;
            size_j--;
            // ler Pk aseguir ao final de Pj até padrão não existir ou chegar ao final do ficheiro.
            while (exist_k != -1 && N < nbytes) {
                Pk[size_k++] = buffer_in[N++];
                exist_k = dict_get_value(dictionary, Pk, size_k);
                if (exist_k == -1) {
                    N--;
                    size_k--;

                    // add pattern Pm(Pj+Pk) to dict
                    Pm = concat_pattern(Pj, size_j, Pk, size_k);
                    dict_add(dictionary, Pm, nextIndex, size_j + size_k);

                    // save Pj to output
                    for (int i = 0; i < size_j; i++) {
                        buffer_out[M] = nextIndex;
                        M++;
                    }
                    nextIndex++;
                    size_j = size_k = 0;
                }
            }
        }
    }
    // free(Pj);
    // free(Pk);
    // free(Pm);

    // // ler Pk aseguir ao final de Pj até padrão não existir ou chegar ao final do ficheiro.
    // int pattern_k = 1;
    // while (pattern_k) {
    //     Pj = my_concat(Pj, buffer_in[N++]);
    //     pattern_j = pattern_exist(Pj);
    // }
    // // Pm = Pj+Pk, save Pm como novo padrão, i++

    // // escrever Pj no buffer_out, N = inicio de Pk

    // // voltar a ler Pj e Pk

    return M;
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