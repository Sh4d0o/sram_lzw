CC = gcc #compiler to use
CFLAGS = -Wall #compiler flags
TARGET = lzwd

build: lzwd.c lzwd_lib.c
	${CC} $(CFLAGS) lzwd.c lzwd_lib.c -o $(TARGET)

clean:
	rm -rf lzwd.o lzwd *.lzwd
