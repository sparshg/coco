#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tokens.h"

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
        int j = 1;
        char* token = strtok(line, " =\n");
        while (token != NULL) {
            int symbolID = get(symbol_map, token, strlen(token));
            // printf("%s:%d ", token, symbolID);
            grammar_rules[rule][j++] = symbolID;
            token = strtok(NULL, " =\n");
        }
        // printf("\n");
        grammar_rules[rule][0] = j - 1;
        rule++;
    }
    fclose(fd);
    return grammar_rules;
}

ParseEntry** create_parse_table() {
    ParseEntry** parse_table = (ParseEntry**)malloc(NT_LEN * sizeof(ParseEntry*));
    for (int i = 0; i < NT_LEN; i++) {
        // TOKENS_LEN + 1 for $
        parse_table[i] = (ParseEntry*)malloc((TOKENS_LEN + 1) * sizeof(ParseEntry));
        for (int j = 0; j < TOKENS_LEN + 1; j++) {
            parse_table[i][j].isFirst = 0;
            parse_table[i][j].isFollow = 0;
            parse_table[i][j].rule_no = -1;
        }
    }
    return parse_table;
}

void print_parse_table(ParseEntry** parse_table) {
    printf("%23s ", " ");
    for (int i = 0; i < TOKENS_LEN; i++) {
        printf("%3.3d ", i);
    }
    printf("057\n");
    printf("%23s ", " ");
    for (int i = 0; i < TOKENS_LEN; i++) {
        printf("%3.3s ", (symbols[i] + 3));
    }
    printf("  $\n");
    for (int i = 0; i < NT_LEN; i++) {
        printf("%23s ", symbols[i + SYMBOLS_LEN - NT_LEN]);
        for (int j = 0; j < TOKENS_LEN + 1; j++) {
            if (parse_table[i][j].rule_no == -1) {
                printf("--- ");
                continue;
            }
            printf("%3d ", parse_table[i][j].rule_no + 1);
            // printf("%3d ", parse_table[i][j].isFollow);
        }
        printf("\n");
    }
}

ParseEntry** get_parse_table(int** grammar_rules, HASHMAP symbol_map) {
    ParseEntry** parse_table = create_parse_table();
    FILE* fd = fopen("firstfollow.txt", "r");
    if (fd == NULL) {
        printf("Error opening first and follow file\n");
        exit(1);
    }
    const int SIZE = 256;
    char line[SIZE];
    int rule = 0;
    int epsilon = string_to_symbol("#", symbol_map);
    // printf("\n%d ", rule + 1);

    while (fgets(line, SIZE, fd)) {
        int can_eps = 0;
        char* token = strtok(line, " ");
        while (*token != ':') {
            int tokenID = string_to_symbol(token, symbol_map);
            // printf("%s[%d][%d] ", token, grammar_rules[rule][1] - SYMBOLS_LEN + NT_LEN, tokenID);
            token = strtok(NULL, " ");
            if (tokenID == epsilon) {
                can_eps = 1;
                continue;
            }
            ParseEntry* entry = &parse_table[grammar_rules[rule][1] - SYMBOLS_LEN + NT_LEN][tokenID];
            if (entry->rule_no != -1) {
                // printf("Not LL1\n");
                exit(1);
            }
            entry->isFirst = 1;
            entry->rule_no = rule;
        }
        token = strtok(NULL, " \n");
        // if (can_eps) {
        // printf(": ");
        while (token != NULL) {
            int tokenID = string_to_symbol(token, symbol_map);
            // printf("%s[%d][%d] ", token, grammar_rules[rule][1] - SYMBOLS_LEN + NT_LEN, tokenID);
            token = strtok(NULL, " \n");
            ParseEntry* entry = &parse_table[grammar_rules[rule][1] - SYMBOLS_LEN + NT_LEN][tokenID];
            if (can_eps) {
                if (entry->rule_no != -1) {
                    // printf("Not LL1\n");
                    exit(1);
                }
                entry->rule_no = rule;
            }
            entry->isFollow = 1;
        }
        // }
        rule++;
        // printf("\n%d ", rule + 1);
    }

    fclose(fd);
    return parse_table;
}

int is_rule_nullable(int** grammar_rules, int rule_no, HASHMAP symbol_map) {
    if (grammar_rules[rule_no][0] == 2 && grammar_rules[rule_no][2] == string_to_symbol("#", symbol_map)) {
        return 1;
    }
    return 0;
}

void push_rule_to_stack(STACK stack, int** grammar_rules, HASHMAP symbol_map, int rule_no) {
    if (is_rule_nullable(grammar_rules, rule_no, symbol_map)) {
        pop(stack);
        return;
    }

    int* rule = grammar_rules[rule_no];
    pop(stack);
    for (int i = rule[0]; i > 1; i--) {
        push(stack, rule[i]);
    }
}

void init_stack(STACK stack, HASHMAP symbol_map) {
    push(stack, string_to_symbol("$", symbol_map));
    push(stack, string_to_symbol("program", symbol_map));
}

int is_non_terminal(int symbol) {
    return symbol >= SYMBOLS_LEN - NT_LEN;
}

int is_end_symbol(int symbol) {
    return symbol == TOKENS_LEN;
}

// int getSetTable() {
//     int** symbolTable = (int**)malloc(NT_LEN * sizeof(int*));
//     for (int i = 0; i < NT_LEN; i++) {
//         symbolTable[i] = (int*)malloc(15 * sizeof(int));
//         for (int j = 0; j < 15; j++) {
//             symbolTable[i][j] = 0;
//         }
//     }
//     return symbolTable;
// }

// void getFirstFollowSets(int** firstSets, int** followSets, HASHMAP symbol_map) {
//     FILE* fd = fopen("firstfollowsets.txt", "r");

//     char* token;
//     const int BUFFER_SIZE = 200;
//     char buffer[BUFFER_SIZE];

//     while (fgets(buffer, BUFFER_SIZE, fd)) {
//         // Get the non-terminal
//         token = strtok(buffer, " =");

//         int nonTerminal = get(symbol_map, token, strlen(token));

//         // Get the first set till :
//         token = strtok(NULL, " =\n");
//         while (*token != ':') {
//             printf("%s ", token);
//             int terminal = get(symbol_map, token, strlen(token));
//             firstSets[nonTerminal][terminal] = 1;
//             token = strtok(NULL, " \n");
//         }
//         token = strtok(NULL, " \n");
//         while (token) {
//             printf("%s ", token);
//             token = strtok(NULL, " \n");
//         }
//     }

//     return;
// }