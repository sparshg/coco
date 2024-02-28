#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"
#include "stack.h"

#define PROD_RULES_LEN 90
#define PROD_RULE_LINE_LEN 12

int** create_parse_table();
int** get_grammar_rules(HASHMAP grammarIndex);
int** get_parse_table(int** grammar_rules, HASHMAP symbol_map);
void print_parse_table(int** parse_table);
void push_rule_to_stack(STACK stack, int** grammar_rules, HASHMAP symbol_map, int rule_no);

#endif