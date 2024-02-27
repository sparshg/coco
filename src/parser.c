#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int NT_LEN = SYMBOLS_LEN - TOKENS_LEN;

HASHMAP create_symbol_map() {
    HASHMAP map = create_hashmap(1024);
    for (int i = 0; i < SYMBOLS_LEN; i++) {
        insert(map, (char*)symbols[i], i);
    }
    return map;
}

int** create_parse_table() {
    int** parse_table = (int**)malloc(NT_LEN * sizeof(int*));
    for (int i = 0; i < NT_LEN; i++) {
        parse_table[i] = (int*)malloc(TOKENS_LEN * sizeof(int));
        for (int j = 0; j < TOKENS_LEN; j++) {
            parse_table[i][j] = -1;
        }
    }
    return parse_table;
}

int** create_grammar_table() {
    int** grammar_rules = (int**)malloc(PROD_RULES_LEN * sizeof(int*));
    for (int i = 0; i < PROD_RULES_LEN; i++) {
        grammar_rules[i] = (int*)malloc(PROD_RULE_LINE_LEN * sizeof(int));
        for (int j = 0; j < PROD_RULE_LINE_LEN; j++) {
            grammar_rules[i][j] = -1;
        }
    }
    return grammar_rules;
}

int** get_grammar_rules(HASHMAP symbol_map) {
    int** grammar_rules = create_grammar_table();
    FILE* fd = fopen("grammar.txt", "r");
    if (fd == NULL) {
        printf("Error opening grammar file\n");
        exit(1);
    }
    int RULE_SIZE = 256;
    char line[RULE_SIZE];
    int row = 0;
    while (fgets(line, RULE_SIZE, fd)) {
        int i = 0;
        char* token = strtok(line, " =\n");
        while (token != NULL) {
            int symbolID = get(symbol_map, token, strlen(token));
            // printf("%s:%d ", token, symbolID);
            grammar_rules[row][i++] = symbolID;
            token = strtok(NULL, " =\n");
        }
        // printf("\n");
        row++;
    }
    fclose(fd);
    return grammar_rules;
}
