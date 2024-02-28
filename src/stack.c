#include "stack.h"



STACK createStack() {
    STACK stack = (STACK)malloc(sizeof(Stack));
    stack->data = (int*)malloc(sizeof(int) * MAX_STACK_SIZE);
    stack->top = -1;
    return stack;
}

int isEmpty(STACK stack) {
    return stack->top == -1;
}

int size(STACK stack) {
    return stack->top + 1;
}

void push(STACK stack, int value) {
    if(stack->top == MAX_STACK_SIZE - 1) {
        printf("Stack Overflow\n");
        return;
    }
    stack->data[++stack->top] = value;
    return;
}

void pop(STACK stack) {
    if (isEmpty(stack)) {
        printf("Stack Underflow\n");
        return;
    }
    stack->data[stack->top--];
    return;
}

int top(STACK stack) {
    if (isEmpty(stack)) {
        printf("Stack is Empty!\n");
        return -1;
    }
    return stack->data[stack->top];
}



void printStack(STACK stack){
    for(int i=0;i<=stack->top;i++){
        printf("%d ",stack->data[i]);
    }
    printf("<--TOP \n");
}
// int main() {
//     STACK stack = createStack();    

//     push(stack, 10);
//     push(stack, 20);
//     push(stack, 30);
//     push(stack,40);
//     push(stack,50);
//     push(stack,60);

//     printf("Top element: %d\n", top(stack));
//     pop(stack);
//     printf("Popped element: %d\n", top(stack));
//     pop(stack);
//     printf("Popped element: %d\n", top(stack));
//     pop(stack);
//     printf("Popped element: %d\n", top(stack));

//     printf("Is stack empty? %s\n", isEmpty(stack) ? "Yes" : "No");

//     return 0;
// }
