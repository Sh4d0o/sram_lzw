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
    // if (DEBUG_FLAG) {
    //     printf("hashstring:");
    //     for (int i = 0; i < size; i++) {
    //         printf("%d", key[i]);
    //     }
    //     printf("\n");
    // }
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
    int *p = malloc(sizeof(int) * 1);
    for (int p_idx = 0; p_idx < 256; p_idx++) {
        p[0] = p_idx;
        dict_add(dictionary, p, p_idx, 1);
    }
    free(p);

    return dictionary;
}

/* key - pattern, value - pattern index, size -  pattern size*/
d_entry *map_pair(int *key, int value, int size) {

    d_entry *entry = malloc(sizeof(d_entry));
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

    int slot = hash(key, size);
    d_entry *new = dictionary->entries[slot];
    if (DEBUG_FLAG) {
        printf("adding to slot [%d]: idx[%d]", slot, value);
        for (int a = 0; a < size; a++) {
            printf("%d ", key[a]);
        }
    }
    // entry empty so insert
    if (new == -1) {
        if (DEBUG_FLAG)
            printf("Empty entry. Added.\n");
        dictionary->entries[slot] = map_pair(key, value, size);
        return;
    }

    // otherwise an colission occured
    // printf("Found collision.");
    d_entry *prev;
    while (new != -1) {
        // next in chain
        prev = new;
        new = prev->next;
    }

    // end of chain reached, add new
    prev->next = map_pair(key, value, size);
    if (DEBUG_FLAG)
        printf("Added to end of chain.\n");
}

/* key - pattern, size - pattern size
return -1 if no match found or idx of matching pattern*/
int dict_get_value(dict *dictionary, int *key, int size) {

    int idx = hash(key, size);
    d_entry *d_search = dictionary->entries[idx];

    if (DEBUG_FLAG) {
        printf("searching:");
        for (int a = 0; a < size; a++) {
            printf("%d ", key[a]);
        }
        printf("\n");
    }
    // no match found
    if (d_search == -1) {
        if (DEBUG_FLAG)
            printf("Match not found. Empty entry.\n");
        return -1;
    }

    // match found, walk the entries
    while (d_search != -1) {
        // check equal key
        if (compare_pattern(key, size, d_search->key, d_search->length) == 1) {
            if (DEBUG_FLAG)
                printf("Match found. Idx: %d\n", d_search->value);
            return d_search->value;
        }
        // next in chain
        d_search = d_search->next;
    }

    // in case key hashes to existing slot but key doesnt match
    if (DEBUG_FLAG)
        printf("Match not found. No entries match.\n");
    return -1;
}

/* index - pattern index
out_pattern - buffer where to save the matched pattern
return -1 if no match found or pattern of matching index*/
int dict_get_pattern(dict *dictionary, int index, int *out_pattern) {
    // cycle the whole dictionary
    for (int i = 0; i < DICT_SIZE; i++) {
        d_entry *d_entry = dictionary->entries[i];
        if (d_entry == -1)
            return -1;
        else {
            while (d_entry != -1) {
                // check equal value
                if (d_entry->value == index) {
                    out_pattern = d_entry->key;
                    return d_entry->length;
                }
                // next in chain
                d_entry = d_entry->next;
            }
            // end of chain reached without a match
            return -1;
        }
    }
    return -1;
}

/* index - pattern index
return -1 if no match found or pattern of matching index*/
d_entry *dict_get_entry(dict *dictionary, int index) {
    // cycle the whole dictionary
    for (int i = 0; i < DICT_SIZE; i++) {
        d_entry *d_entry = dictionary->entries[i];
        if (d_entry == -1)
            continue;
        else {
            while (d_entry != -1) {
                // check equal value
                if (d_entry->value == index) {
                    return d_entry;
                }
                // next in chain
                d_entry = d_entry->next;
            }
            // end of chain reached without a match
            return -1;
        }
    }
    return -1;
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
 * @param result where to save result
 **/
int *concat_pattern(int *pattern_x, int size_x, int *pattern_y, int size_y) {
    int *result = malloc(size_x + size_y);
    for (int i = 0; i < size_x; i++) {
        result[i] = pattern_x[i];
    }
    for (int a = size_x, b = 0; b < size_y; a++, b++) {
        result[a] = pattern_y[b];
    }
    if (DEBUG_FLAG) {
        printf("Concatenated pattern:\n");
        for (int j = 0; j < (size_x + size_y); j++) {
            printf("%d", result[j]);
        }
        printf("\n");
    }
    return result;
}
// void concat_pattern(int *pattern_x, int size_x, int *pattern_y, int size_y, int *result) {
//     // int *result = malloc(size_x + size_y);
//     for (int i = 0; i < size_x; i++) {
//         result[i] = pattern_x[i];
//     }
//     for (int a = size_x, b = 0; b < size_y; a++, b++) {
//         result[a] = pattern_y[b];
//     }
//     // return result;
// }

void dict_print(dict *dictionary) {
    printf("Printing dictionary: \n[slot][idx][pattern]\n");
    for (int i = 0; i < DICT_SIZE; i++) {
        d_entry *d_entry = dictionary->entries[i];
        if (d_entry == -1)
            continue;
        else {
            // cycle the chain
            while (d_entry != -1) {
                printf("[%d] [%d] [", i, d_entry->value);
                for (int a = 0; a < d_entry->length; a++) {
                    printf("%d ", d_entry->key[a]);
                }
                printf("]\n");
                // next in chain
                d_entry = d_entry->next;
            }
        }
    }
}

// TO-DO
// void dict_print_order(dict *dictionary) {
//     printf("Printing dictionary in order: \n[slot][idx][pattern]\n");
//     for (int i = 0; i < DICT_SIZE; i++) {
//         d_entry *d_entry = dict_get_entry(dictionary, i);
//         if (d_entry == -1)
//             continue;
//         else {
//             printf("[%d][%d][", i, d_entry->value);
//             for (int a = 0; a < d_entry->length; a++) {
//                 printf("%d ", d_entry->key[a]);
//             }
//             printf("]\n");
//         }
//     }
// }

void dict_free(dict *dictionary) {
    for (int i = 0; i < DICT_SIZE; i++) {
        d_entry *d_entry = dictionary->entries[i];
        if (d_entry == -1)
            continue;
        else {
            // cycle the chain
            while (d_entry != -1) {

                // next in chain
                d_entry = d_entry->next;
            }
        }
    }
    printf(DEBUG_TXT "%s" RESET_TXT, "Cleared dictionary.\n");
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
    // print buffer de entrada
    // if (DEBUG_FLAG) {
    //     printf("buffer_in (lzwd_encode):\n");
    //     for (int b = 0; b < nbytes; b++) {
    //         printf("%d ", ((unsigned char *)buffer_in)[b]);
    //     }
    // }

    int N = 0; // apontador de leitura do bloco
    int M = 0; // apontador de escrita do output
    int nextIndex = 256;
    dict *dictionary = create_dict();

    printf(DEBUG_TXT "%s" RESET_TXT, "Dictionary Initializated.\n");

    int save_pj_index = 0;
    int save_N_Pk = 0;
    int exist_j = 1; // exit condition flag
    int size_j = 0;  // pattern size
    // int Pj[nbytes];  // longest pattern possible(full block)
    int *Pj = malloc(nbytes * sizeof(int));
    memset(Pj, 0, nbytes * sizeof(int));

    int exist_k = 1; // exit condition flag
    int size_k = 0;  // pattern size
    // int Pk[nbytes];  // longest pattern possible(full block)
    int *Pk = malloc(nbytes * sizeof(int));
    memset(Pk, 0, nbytes * sizeof(int));

    // int Pm[nbytes];
    int *Pm = malloc(2 * nbytes * sizeof(int));
    memset(Pm, 0, 2 * nbytes * sizeof(int));

    do {
        N = save_N_Pk;

        // 1. Ler Pj apartir de N até padrão não existir ou chegar ao final do ficheiro.
        while (exist_j != -1 && N < nbytes) {
            Pj[size_j++] = ((unsigned char *)buffer_in)[N++]; /*concat new simbol to pattern*/

            if (DEBUG_FLAG) {
                printf("::Pj-> ");
                for (int b = 0; b < size_j; b++) {
                    printf("%d ", Pj[b]);
                }
                printf("\n");
            }
            save_pj_index = exist_j;                          /*to later write to output*/
            exist_j = dict_get_value(dictionary, Pj, size_j); /*check if new pattern exists*/

            if (exist_j == -1) { // Pj not found
                N--;
                size_j--; /* ignore last symbol added to pattern*/

                save_N_Pk = N;
                // ler Pk aseguir ao final de Pj até padrão não existir ou chegar ao final do ficheiro.
                while (exist_k != -1 && N < nbytes) {

                    Pk[size_k++] = ((unsigned char *)buffer_in)[N++];

                    if (DEBUG_FLAG) {
                        printf("::Pk-> ");
                        for (int b = 0; b < size_k; b++) {
                            printf("%d ", Pk[b]);
                        }
                        printf("\n");
                    }
                    exist_k = dict_get_value(dictionary, Pk, size_k);
                    if (exist_k == -1) {
                        N--;
                        size_k--;

                        // add pattern Pm(Pj+Pk) to dict
                        Pm = concat_pattern(Pj, size_j, Pk, size_k);

                        dict_add(dictionary, Pm, nextIndex, size_j + size_k);

                        // save Pj index to output
                        buffer_out[M] = save_pj_index;
                        M++;
                        if (DEBUG_FLAG) {
                            printf("::OUT-> %d \n", save_pj_index);
                        }
                        nextIndex++;
                    }
                }
                // TODO: save Pk when is last pattern of the buffer
            }
        }
        size_j = size_k = 0;
        exist_j = exist_k = 0;
    } while (N < nbytes);

    // if (DEBUG_FLAG)
    //     dict_print(dictionary);

    dict_free(dictionary);
    free(dictionary);
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