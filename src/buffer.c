#include "buffer.h"

#include <ctype.h>
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
    buf->next = 1;
    buf->ptr = 0;
    buf->f = fp;
    buf->st_ptr = 0;
    buf->mode = READ;
    memset(buf->b[buf->curr], EOF, BUFSIZE);
    fread(buf->b[buf->curr], sizeof(char), BUFSIZE, fp);
    return buf;
}

char current(BUF buf) {
    return buf->b[buf->curr][buf->ptr];
}

char back(BUF buf) {
    if (buf->ptr-- == 0) {
        if (buf->next == buf->curr) {
            perror("Cannot go back, consider increasing buffer size\n");
            exit(1);
        }
        buf->curr = 1 - buf->curr;
        buf->ptr = BUFSIZE - 1;
        buf->mode = RETRACT;
    }
    return buf->b[buf->curr][buf->ptr];
}

char next(BUF buf) {
    char c = buf->b[buf->curr][buf->ptr];
    if (buf->ptr++ == BUFSIZE - 1) {
        buf->curr = 1 - buf->curr;
        buf->ptr = 0;
        if (buf->mode == READ || buf->curr == buf->next) {
            if (buf->save_stack[0][1] == buf->curr) {
                perror("Cannot pop in future, consider increasing buffer size\n");
                exit(1);
            }
            buf->next = 1 - buf->next;
            buf->mode = READ;
            memset(buf->b[buf->curr], EOF, BUFSIZE);
            fread(buf->b[buf->curr], sizeof(char), BUFSIZE, buf->f);
        }
    }
    return c;
}

char* string_from(BUF buf, int n) {
    char* str;
    int len = buf->ptr + BUFSIZE - buf->save_stack[n][0];
    if (buf->save_stack[n][1] <= buf->curr) {
        if (buf->save_stack[n][1] == buf->curr) len -= BUFSIZE;
        str = malloc((len + 1) * sizeof(char));
        strncpy(str, &buf->b[buf->save_stack[n][1]][buf->save_stack[n][0]], len);
    } else {
        str = malloc((len + 1) * sizeof(char));
        strncpy(str, &buf->b[1 - buf->curr][buf->save_stack[n][0]], BUFSIZE - buf->save_stack[n][0]);
        strncpy(str + BUFSIZE - buf->save_stack[n][0], &buf->b[buf->curr][0], buf->ptr);
    }
    str[len] = '\0';
    return str;
}

int push_state(BUF buf) {
    if (buf->st_ptr == BUFSAVE) {
        perror("Buffer save stack overflowed\n");
        exit(1);
    }
    buf->save_stack[buf->st_ptr][0] = buf->ptr;
    buf->save_stack[buf->st_ptr][1] = buf->curr;
    return buf->st_ptr++;
}

void pop_state(BUF buf) {
    if (--buf->st_ptr < 0) {
        perror("Buffer save stack underflow\n");
        exit(1);
    }
    buf->ptr = buf->save_stack[buf->st_ptr][0];
    buf->curr = buf->save_stack[buf->st_ptr][1];
    buf->mode = RETRACT;
}

void clear_saves(BUF buf) {
    buf->st_ptr = 0;
    // buf->mode = READ;
}

void pop_nth(BUF buf, int n) {
    buf->st_ptr = n;
    buf->ptr = buf->save_stack[buf->st_ptr][0];
    buf->curr = buf->save_stack[buf->st_ptr][1];
    buf->mode = RETRACT;
}

int skip_whitespace(BUF buf) {
    int n = 0;
    while (isspace(current(buf))) {
        if (next(buf) == '\n') n++;
    }
    return n;
}

void close_buf(BUF buf) {
    fclose(buf->f);
}