#ifndef STACK_H
#define STACK_H

#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#define MAX_STACK_SIZE 100

typedef struct {
    TREENODE* data;
    int top;
} Stack;

typedef Stack* STACK;

STACK create_stack();
TREENODE push(STACK stack, int symbolId, char* lexeme, int line);
TREENODE pop(STACK stack);
TREENODE top(STACK stack);
int is_empty(STACK stack);
int size(STACK stack);
void print_stack(STACK stack);
void delete_stack(STACK stack);

#endif