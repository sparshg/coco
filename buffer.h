// Group 26
// Rishi Gupta     (2021A7PS0690P)
// Sparsh Goenka   (2021A7PS2413P)
// Utkarsh Sharma  (2021A7PS0693P)
// Saumya Sharma   (2021A7PS0544P)
// Akshat Bajpai   (2021A7PS0573P)

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
    FILE* f;
    char b[2][BUFSIZE];

    // current buffer, next buffer
    int curr, next;

    // current ptr
    int ptr;

    // (ptr, curr)
    int save_stack[BUFSAVE][2];
    int st_ptr;
    BufMode mode;
} Buf;

typedef struct Buf* BUF;

char next(BUF buf);
char current(BUF buf);
char back(BUF buf);
char* string_from(BUF buf, int nth_save_earlier);

int skip_whitespace(BUF buf);
int push_state(BUF buf);
void pop_state(BUF buf);
void pop_nth(BUF buf, int n);
void clear_saves(BUF buf);
void close_buf(BUF buf);

BUF read_file(char* filename);
BUF get_stream(FILE* fp);

#endif