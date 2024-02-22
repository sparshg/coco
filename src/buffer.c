#include "buffer.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

BUF read_file(char* filename) {
    FILE* fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("File %s not found", filename);
        exit(1);
    }
    return get_stream(fp);
}

BUF get_stream(FILE* fp) {
    BUF buf = malloc(sizeof(Buf));
    buf->curr = 0;
    buf->ptr = 0;
    buf->f = fp;
    memset(buf->b[buf->curr], EOF, BUFSIZE);
    fread(buf->b[buf->curr], sizeof(char), BUFSIZE, fp);
    return buf;
}

// char current(BUF buf) {
//     return buf->b[buf->curr][buf->ptr];
// }

// char next(BUF buf) {
//     char c = buf->b[buf->curr][buf->ptr];
//     if (buf->ptr++ == BUFSIZE - 1) {
//         buf->curr = 1 - buf->curr;
//         buf->ptr = 0;
//         memset(buf->b[buf->curr], EOF, BUFSIZE);
//         fread(buf->b[buf->curr], sizeof(char), BUFSIZE, buf->f);
//     }
//     return c;
// }