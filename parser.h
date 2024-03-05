// Group 26
// Rishi Gupta     (2021A7PS0690P)
// Sparsh Goenka   (2021A7PS2413P)
// Utkarsh Sharma  (2021A7PS0693P)
// Saumya Sharma   (2021A7PS0544P)
// Akshat Bajpai   (2021A7PS0573P)
#ifndef PARSER_H
#define PARSER_H

#include "buffer.h"
#include "lexer.h"
#include "parserDef.h"
#include "stack.h"
#include "tokens.h"
#include "tree.h"

#define PROD_RULES_LEN 93
#define PROD_RULE_LINE_LEN 12
#define MAX_RULES_USED 750

ParseEntry** create_parse_table();
void delete_parse_table(ParseEntry** parse_table);
int** get_grammar_rules(HASHMAP grammarIndex);
void delete_grammar_table(int** grammar_rules);
ParseEntry** get_parse_table(int** grammar_rules, HASHMAP symbol_map, int* nullable);
void print_parse_table(ParseEntry** parse_table);
void print_parse_tree(char* dest, TREENODE parseTree);
TREENODE parse_input_source_code(BUF b, HASHMAP keyword_table, HASHMAP symbol_map, int** grammar_rules, ParseEntry** parse_table, int* nullable);
#endif