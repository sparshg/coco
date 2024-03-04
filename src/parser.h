#ifndef PARSER_H
#define PARSER_H

#include "buffer.h"
#include "lexer.h"
#include "stack.h"
#include "tokens.h"
#include "tree.h"

#define PROD_RULES_LEN 93
#define PROD_RULE_LINE_LEN 12
#define MAX_RULES_USED 750

// -2 for $ and #
// const int NT_LEN = SYMBOLS_LEN - TOKENS_LEN - 2;

typedef struct ParseEntry {
    int rule_no;
    int isFirst;
    int isFollow;
} ParseEntry;

ParseEntry** create_parse_table();
int** get_grammar_rules(HASHMAP grammarIndex);
ParseEntry** get_parse_table(int** grammar_rules, HASHMAP symbol_map, int* nullable);
void print_parse_table(ParseEntry** parse_table);
void push_rule_to_stack(STACK stack, int** grammar_rules, HASHMAP symbol_map, int rule_no);
void init_stack(STACK stack, HASHMAP symbol_map);
TREENODE parse_input_source_code(BUF b, HASHMAP keyword_table, HASHMAP symbol_map, int** grammar_rules, ParseEntry** parse_table, int* nullable);

#endif