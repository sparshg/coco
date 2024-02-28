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

STACK createStack();
void push(STACK stack, int value);
void pop(STACK stack);
int top(STACK stack);
int isEmpty(STACK stack);
int size(STACK stack);
void printStack(STACK stack);


#endif