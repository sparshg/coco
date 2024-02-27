#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

// -2 for $ and #
const int NT_LEN = SYMBOLS_LEN - TOKENS_LEN - 2;

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
    const int SIZE = 256;
    char line[SIZE];
    int rule = 0;
    while (fgets(line, SIZE, fd)) {
        int j = 0;
        char* token = strtok(line, " =\n");
        while (token != NULL) {
            int symbolID = get(symbol_map, token, strlen(token));
            // printf("%s:%d ", token, symbolID);
            grammar_rules[rule][j++] = symbolID;
            token = strtok(NULL, " =\n");
        }
        // printf("\n");
        rule++;
    }
    fclose(fd);
    return grammar_rules;
}

int** create_parse_table() {
    int** parse_table = (int**)malloc(NT_LEN * sizeof(int*));
    for (int i = 0; i < NT_LEN; i++) {
        // TOKENS_LEN + 1 for $
        parse_table[i] = (int*)malloc((TOKENS_LEN + 1) * sizeof(int));
        for (int j = 0; j < TOKENS_LEN; j++) {
            parse_table[i][j] = -1;
        }
    }
    return parse_table;
}

int** get_parse_table(int** grammar_rules, HASHMAP symbol_map) {
    int** parse_table = create_parse_table();
    FILE* fd = fopen("firstfollow.txt", "r");
    if (fd == NULL) {
        printf("Error opening first and follow file\n");
        exit(1);
    }
    const int SIZE = 256;
    char line[SIZE];
    int rule = 0;
    int epsilon = string_to_symbol("#", symbol_map);
    printf("\n");
    while (fgets(line, SIZE, fd)) {
        int can_eps = 0;
        char* token = strtok(line, " ");
        while (*token != ':') {
            int tokenID = string_to_symbol(token, symbol_map);
            printf("%s[%d][%d] ", token, grammar_rules[rule][0] - SYMBOLS_LEN + NT_LEN, tokenID);
            token = strtok(NULL, " ");
            if (tokenID == epsilon) {
                can_eps = 1;
                continue;
            }
            parse_table[grammar_rules[rule][0] - SYMBOLS_LEN + NT_LEN][tokenID] = rule;
        }
        token = strtok(NULL, " \n");
        if (can_eps) {
            printf(": ");
            while (token != NULL) {
                int tokenID = string_to_symbol(token, symbol_map);
                printf("%s[%d][%d] ", token, grammar_rules[rule][0] - SYMBOLS_LEN + NT_LEN, tokenID);
                token = strtok(NULL, " \n");
                parse_table[grammar_rules[rule][0] - SYMBOLS_LEN + NT_LEN][tokenID] = rule;
            }
        }
        printf("\n");
        rule++;
    }

    // print parse table
    for (int i = 0; i < NT_LEN; i++) {
        for (int j = 0; j <= TOKENS_LEN; j++) {
            printf("%d ", parse_table[i][j]);
        }
        printf("\n");
    }

    fclose(fd);
    return parse_table;
}