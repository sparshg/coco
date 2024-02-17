#ifndef FIRST_H
#define FIRST_H
#include "tokens.h"

struct Grammar {
    int num_productions;
    int num_terminals;
    int num_non_terminals;
    int start_symbol;
    Token** productions;
    Token* terminals;
    char** non_terminals;
};

typedef struct Grammar* GRAMMAR;

#endif