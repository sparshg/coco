#ifndef BUFFER_H
#define BUFFER_H

// should't be too less (> 32)
#define BUFSIZE 256

#define BUFSAVE 10

#include <stdio.h>

typedef enum BufMode {
    READ,
    RETRACT
} BufMode;

typedef struct Buf {
    char b[2][BUFSIZE];
    int curr, next, ptr;
    FILE* f;

    // (ptr, curr)
    int save_stack[BUFSAVE][2];
    int st_ptr;
    BufMode mode;
} Buf;

typedef struct Buf* BUF;

char next(BUF buf);
char current(BUF buf);
char* string_from(BUF buf, int nth_save_earlier);

void skip_whitespace(BUF buf);
int push_state(BUF buf);
void pop_state(BUF buf);
void pop_nth(BUF buf, int n);
void clear_saves(BUF buf);
void closeFile(BUF buf);

BUF read_file(char* filename);
BUF get_stream(FILE* fp);

#endif