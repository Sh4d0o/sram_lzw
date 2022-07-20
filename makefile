CC = gcc #compiler to use
CFLAGS = -g -Wall #compiler flags
TARGET = lzwd #name of executable
TARGET2 = lzw #name of executable

build: lzw.c lzwd_lib.c
	${CC} $(CFLAGS) lzw.c lzwd_lib.c -o $(TARGET2)

lzwd: lzwd.c lzwd_lib.c
	${CC} $(CFLAGS) lzwd.c lzwd_lib.c -o $(TARGET)

lzw: lzw.c lzwd_lib.c
	${CC} $(CFLAGS) lzw.c lzwd_lib.c -o $(TARGET2)

clean:
	rm -rf *.lzwd *.lzw
