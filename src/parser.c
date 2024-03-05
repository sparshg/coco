#include "parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

ParseEntry** get_parse_table(int** grammar_rules, HASHMAP symbol_map, int* nullable_nt) {
    ParseEntry** parse_table = create_parse_table();
    FILE* fd = fopen("firstfollow.txt", "r");
    if (fd == NULL) {
        printf("Error opening first and follow file\n");
        exit(1);
    }
    const int SIZE = 256;
    char line[SIZE];
    int rule = 0;
    // printf("\n%d ", rule + 1);

    while (fgets(line, SIZE, fd)) {
        int can_eps = 0;
        char* token = strtok(line, " ");
        while (*token != ':') {
            int tokenID = string_to_symbol(token, symbol_map);
            // printf("%s[%d][%d] ", token, rule_to_nt(rule, grammar_rules), tokenID);
            token = strtok(NULL, " ");
            if (is_epsilon(tokenID)) {
                can_eps = 1;
                nullable_nt[rule_to_nt(rule, grammar_rules)] = 1;
                continue;
            }
            ParseEntry* entry = &parse_table[rule_to_nt(rule, grammar_rules)][tokenID];
            if (entry->rule_no != -1) {
                printf("Not LL1\n");
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
            // printf("%s[%d][%d] ", token, rule_to_nt(rule, grammar_rules), tokenID);
            token = strtok(NULL, " \n");
            ParseEntry* entry = &parse_table[rule_to_nt(rule, grammar_rules)][tokenID];
            if (can_eps) {
                if (entry->rule_no != -1) {
                    printf("Not LL1\n");
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

void push_rule(STACK stack, int** grammar_rules, HASHMAP symbol_map, int rule_no) {
    if (grammar_rules[rule_no][0] == 2 && is_epsilon(grammar_rules[rule_no][2])) {
        add_child(pop(stack), create_node(string_to_symbol("#", symbol_map), NULL, -1));
        return;
    }

    int* rule = grammar_rules[rule_no];
    TREENODE parent = pop(stack);

    // rule[0] is the length of the rule
    for (int i = rule[0]; i > 1; i--) {
        add_child(parent, push(stack, rule[i], NULL, -1));
    }
}

void print_parse_tree(FILE* f, TREENODE parseTree) {
    fprintf(f, "---------------------------------------------------------------------------------------------------------------------------------------\n");
    fprintf(f, "--------------------------------------------------------------Parse Tree--------------------------------------------------------------\n\n");
    fprintf(f, "%25s%10s%15s%15s%25s%20s%25s\n", "LEXEME", "LINE NO.", "TOKEN_NAME", "VALUE_IF_NUM", "PARENT_SYMBOL", "IS_LEAF_NODE", "NODE_SYMBOL");
    fprintf(f, "---------------------------------------------------------------------------------------------------------------------------------------\n");
    print_tree(f, parseTree, "ROOT");
}

TREENODE parse_input_source_code(BUF b, char* dest, HASHMAP keyword_table, HASHMAP symbol_map, int** grammar_rules, ParseEntry** parse_table, int* nullable) {
    STACK stack = create_stack();

    TREENODE parseTree = push(stack, string_to_symbol("program", symbol_map), NULL, -1);

    int last_state, line = 1;
    Token token = get_next_token(b, keyword_table, &line, &last_state, 0);

    while (current(b) != EOF) {
        if (is_empty(stack))
            break;
        // if top of stack is non terminal
        if (symbol_to_nt(top(stack)->symbol) >= 0) {
            ParseEntry rule = parse_table[symbol_to_nt(top(stack)->symbol)][token];
            if (rule.rule_no == -1) {
                if (nullable[symbol_to_nt(top(stack)->symbol)]) {
                    pop(stack);
                    continue;
                }
                char* lexeme = string_from(b, last_state);
                printf("Line %-3d| Invalid token %s encountered with value %s stack top %s\n", line, symbol_to_string(token), lexeme, symbols[top(stack)->symbol]);
                free(lexeme);

                if (!rule.isFollow) {
                    token = get_next_token(b, keyword_table, &line, &last_state, 0);
                    continue;
                }
                pop(stack);
            } else {
                push_rule(stack, grammar_rules, symbol_map, rule.rule_no);
            }
        } else {
            if (top(stack)->symbol == token) {
                // printf("Matched: %s\n", token_to_string(token));
                TREENODE top = pop(stack);
                top->lexeme = string_from(b, last_state);
                top->line = line;
                token = get_next_token(b, keyword_table, &line, &last_state, 0);
            } else {
                char* lexeme = string_from(b, last_state);
                printf("Line %-3d| Error: The token %s for lexeme %s does not match with the expected token %s\n", line, symbol_to_string(token), lexeme, symbol_to_string(top(stack)->symbol));
                free(lexeme);
                pop(stack);
            }
        }
    }

    if (is_empty(stack)) {
        if (current(b) == EOF) {
            printf("Parsing Successful!\n");
        } else {
            printf("Parsing Error: Extra tokens after the end of the program.\n");
        }
    } else {
        printf("Parsing Error: Stack not empty.\n");
    }

    if (dest == NULL) {
        delete_stack(stack);
        return parseTree;
    }

    FILE* f = fopen(dest, "w");
    if (f == NULL) {
        perror("Error opening file");
        return parseTree;
    }
    print_parse_tree(f, parseTree);
    fclose(f);
    print_parse_tree(stdout, parseTree);

    delete_stack(stack);
    return parseTree;
}