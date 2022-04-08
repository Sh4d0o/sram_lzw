/**
 * author: shadolaptop
 * created: 18-03-2022
 * project: File compression (LZWd algorithm)
 **/

#include "lzwd_lib.h"

int main(int argc, char *argv[]) {
    //---Program variables---//
    clock_t t_start = clock(), t_end; // clocks for executing time calculations

    FILE *src_file, *dest_file;
    int debugflag = 0; // if true use debug moode
    int sizeflag = 0;  // if true use costum block size
    int block_size = 0;
    int src_size = 0, dest_size = 0, block_count = 0, last_block_size = 0; // std output auxiliars

    // 1. read and interpret the input
    int opt;
    while ((opt = getopt(argc, argv, "ds:")) != -1) {
        switch (opt) {
        case 'd':
            debugflag = 1;
            pdebug(debugflag, "Program iniciated in debug mode.\n");
            break;
        case 's':
            sizeflag = 1;
            block_size = atoi(optarg);
            pdebug(debugflag, "Costum block size.\n");
            break;
        case '?':
            printf("%s\n", USAGE_MSG);
            return 1;
        default:
            abort();
        }
    }
    // faulty input check
    int maxarguments = 0, idx_arg = 0;
    for (idx_arg = optind; idx_arg < argc; idx_arg++) {
        maxarguments++;
        if (maxarguments > 1) {
            printf("%s", "Too many arguments.");
            return 1;
        }
    }

    // 2.OPEN SOURCE file
    src_file = fopen(argv[optind], "rb");
    if (!src_file) {
        printf("Unable to open supplied file.\n");
        return 1;
    }
    // extract filename from source to name the output file
    char *src_name = malloc(strlen(argv[optind]));
    strcpy(src_name, argv[optind]);
    char *filename = strtok(argv[optind], ".");
    // printf("filename: %s\ninput: %s\n", filename, argv[optind]); // CLEAR

    // 3. CREATE AND OPEN destination file with ".lzwd" extension
    char *compress_name = my_concat(filename, ".lzwd");
    dest_file = fopen(compress_name, "w");
    // printf("dest_path: %s\n", dest_path); // CLEAR
    if (!dest_file) {
        printf("Unable to create destination file.\n");
        return 1;
    }

    // 4. Initialize dictionary
    // DNode *dict_root = NULL;
    // init_dictionary(dict_root);

    // 5. Block Read
    if (!sizeflag) {
        block_size = BLOCK_SIZE;
    }
    size_t nbytes = 0; // quantity of bytes read in block
    BYTE buffer_in[block_size];
    BYTE buffer_out[block_size];
    int output_size = 0;

    // 5.1.read blocks
    while ((nbytes = fread(buffer_in, 1, block_size, src_file)) > 0) {
        block_count++;
        // 5.2.process block
        output_size = encode_lzwd(buffer_in, nbytes, buffer_out);

        // save size of last block and total sizes
        src_size += nbytes;
        dest_size += output_size;
        last_block_size = nbytes;

        // write encoded block to output file
        fwrite(buffer_out, output_size, 1, dest_file);
    }
    // X. Program Output
    printf("Author: Tiago & Joana\n");
    time_t now;
    time(&now); // get current date and time
    printf("Execution time: %s", ctime(&now));
    printf("Source: %s with %d bytes\nCompressed: %s with %d bytes\n", src_name, src_size, compress_name, dest_size);
    printf("Blocks processed: %d || Block size: %d || Last Block: %d\n", block_count, block_size, last_block_size);
    float compression = (1 - (float)dest_size / src_size) * 100;
    printf("Total compresion: %.2f %%\n", compression);

    t_end = clock() - t_start;
    printf("Duration(TOTAL): %f seconds\n", ((double)t_end) / CLOCKS_PER_SEC);
    free(compress_name);
    free(src_name);
    fclose(src_file);
    fclose(dest_file);
}
