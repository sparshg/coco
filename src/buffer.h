#ifndef BUFFER_H
#define BUFFER_H
#define BUFSIZE 128
#include <stdio.h>
typedef struct Buf {
    char b[2][BUFSIZE];
    int curr;
    int ptr;
    FILE* f;
} Buf;

typedef struct Buf* BUF;

// char next(BUF buf);
// char current(BUF buf);
BUF read_file(char* filename);
BUF get_stream(FILE* fp);

#endif