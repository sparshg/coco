#ifndef PARSER_H
#define PARSER_H

#include "stack.h"
#include "tokens.h"

#define PROD_RULES_LEN 93
#define PROD_RULE_LINE_LEN 12

// -2 for $ and #
// const int NT_LEN = SYMBOLS_LEN - TOKENS_LEN - 2;

typedef struct ParseEntry {
    int rule_no;
    int isFirst;
    int isFollow;
} ParseEntry;

ParseEntry** create_parse_table();
int** get_grammar_rules(HASHMAP grammarIndex);
ParseEntry** get_parse_table(int** grammar_rules, HASHMAP symbol_map);
void print_parse_table(ParseEntry** parse_table);
void push_rule_to_stack(STACK stack, int** grammar_rules, HASHMAP symbol_map, int rule_no);
void init_stack(STACK stack, HASHMAP symbol_map);
int is_rule_nullable(int** grammar_rules, int rule_no, HASHMAP symbol_map);

#endif