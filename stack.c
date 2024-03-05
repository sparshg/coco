// Group 26
// Rishi Gupta     (2021A7PS0690P)
// Sparsh Goenka   (2021A7PS2413P)
// Utkarsh Sharma  (2021A7PS0693P)
// Saumya Sharma   (2021A7PS0544P)
// Akshat Bajpai   (2021A7PS0573P)
#include "stack.h"

STACK create_stack() {
    STACK stack = (STACK)malloc(sizeof(Stack));
    stack->data = (TREENODE*)malloc(MAX_STACK_SIZE * sizeof(TREENODE));
    stack->top = -1;
    return stack;
}

int is_empty(STACK stack) {
    return stack->top == -1;
}

int size(STACK stack) {
    return stack->top + 1;
}

TREENODE push(STACK stack, int symbolId, char* lexeme, int line) {
    if (stack->top == MAX_STACK_SIZE - 1) {
        printf("Stack Overflow\n");
        exit(1);
    }
    TREENODE node = create_node(symbolId, lexeme, line);
    stack->data[++stack->top] = node;
    return node;
}

TREENODE pop(STACK stack) {
    if (is_empty(stack)) {
        printf("Stack Underflow\n");
        exit(1);
    }
    return stack->data[stack->top--];
}

TREENODE top(STACK stack) {
    if (is_empty(stack)) {
        printf("Stack is Empty!\n");
        exit(1);
    }
    return stack->data[stack->top];
}

void print_stack(STACK stack) {
    for (int i = 0; i <= stack->top; i++) {
        printf("%d ", stack->data[i]->symbol);
    }
    printf("<--TOP \n");
}

void delete_stack(STACK stack) {
    free(stack->data);
    free(stack);
}