#include "stack.h"

STACK create_stack() {
    STACK stack = (STACK)malloc(sizeof(Stack));
    stack->data = (int*)malloc(sizeof(int) * MAX_STACK_SIZE);
    stack->top = -1;
    return stack;
}

int is_empty(STACK stack) {
    return stack->top == -1;
}

int size(STACK stack) {
    return stack->top + 1;
}

void push(STACK stack, int value) {
    if (stack->top == MAX_STACK_SIZE - 1) {
        printf("Stack Overflow\n");
        return;
    }
    stack->data[++stack->top] = value;
    return;
}

void pop(STACK stack) {
    if (is_empty(stack)) {
        printf("Stack Underflow\n");
        return;
    }
    stack->top--;
    return;
}

int top(STACK stack) {
    if (is_empty(stack)) {
        printf("Stack is Empty!\n");
        return -1;
    }
    return stack->data[stack->top];
}

void print_stack(STACK stack) {
    for (int i = 0; i <= stack->top; i++) {
        printf("%d ", stack->data[i]);
    }
    printf("<--TOP \n");
}

void delete_stack(STACK stack) {
    free(stack->data);
    free(stack);
}