#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include "stack.h"

#define PROD_RULES_LEN 90
#define PROD_RULE_LINE_LEN 12

// -2 for $ and #
// const int NT_LEN = SYMBOLS_LEN - TOKENS_LEN - 2;

int** create_parse_table();
int** get_grammar_rules(HASHMAP grammarIndex);
int** get_parse_table(int** grammar_rules, HASHMAP symbol_map);
void print_parse_table(int** parse_table);
void push_rule_to_stack(STACK stack, int** grammar_rules, HASHMAP symbol_map, int rule_no);
void initStack(STACK stack, HASHMAP symbol_map);
int isNonTerminal(int symbol);
int isEndSymbol(int symbol);

#endif