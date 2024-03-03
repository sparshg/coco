#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100


typedef struct {
    int* data;
    int top;
} Stack;

typedef Stack* STACK;

STACK create_stack();
void push(STACK stack, int value);
void pop(STACK stack);
int top(STACK stack);
int is_empty(STACK stack);
int size(STACK stack);
void print_stack(STACK stack);


#endif