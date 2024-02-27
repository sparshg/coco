#ifndef PARSER_H
#define PARSER_H

#include "tokens.h"

#define PROD_RULES_LEN 90
#define PROD_RULE_LINE_LEN 12

int** create_parse_table();
int** get_grammar_rules(HASHMAP grammarIndex);
int** get_parse_table(int** grammar_rules, HASHMAP symbol_map);

#endif