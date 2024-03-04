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

int is_nt_nullable(int* nullable, int symbolId) {
    for (int i = 0; i < 15; i++) {
        if (nullable[i] == symbolId) {
            return 1;
        }
    }
    return 0;
}

TREENODE parse_input_source_code(BUF b, HASHMAP keyword_table, HASHMAP symbol_map, int** grammar_rules, ParseEntry** parse_table) {
    STACK stack = create_stack();
    init_stack(stack, symbol_map);

    int nullable[] = {string_to_symbol("otherFunctions", symbol_map), string_to_symbol("output_par", symbol_map), string_to_symbol("remaining_list", symbol_map), string_to_symbol("typeDefinitions", symbol_map), string_to_symbol("moreFields", symbol_map), string_to_symbol("declarations", symbol_map), string_to_symbol("global_or_not", symbol_map), string_to_symbol("otherStmts", symbol_map), string_to_symbol("option_single", symbol_map), string_to_symbol("moreExpansion", symbol_map), string_to_symbol("outputParameters", symbol_map), string_to_symbol("expression'", symbol_map), string_to_symbol("term'", symbol_map), string_to_symbol("optionalReturn", symbol_map), string_to_symbol("more_ids", symbol_map)};

    int line = 1;

    int rules_used[MAX_RULES_USED];
    for (int i = 0; i < MAX_RULES_USED; i++) {
        rules_used[i] = -1;
    }

    int rules_used_idx = 0;
    int n;

    Token token = get_next_token(b, keyword_table, &line, &n, 0);
    while (current(b) != EOF) {
        if (top(stack) == string_to_symbol("$", symbol_map) || is_empty(stack))
            break;
        if (is_non_terminal(top(stack))) {
            ParseEntry rule = parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token];
            if (rule.rule_no == -1) {
                if (is_nt_nullable(nullable, top(stack))) {
                    pop(stack);
                    continue;
                }
                printf("Line %-3d| Invalid token %s encountered with value %s stack top %s\n", line, token_to_string(token), string_from(b, n), symbols[top(stack)]);

                if (!rule.isFollow) {
                    token = get_next_token(b, keyword_table, &line, &n, 0);
                    continue;
                }
                pop(stack);
            } else {
                push_rule_to_stack(stack, grammar_rules, symbol_map, rule.rule_no);
                rules_used[rules_used_idx++] = rule.rule_no;
            }
        } else {
            if (top(stack) == token) {
                // printf("Matched: %s\n", token_to_string(token));
                pop(stack);
                token = get_next_token(b, keyword_table, &line, &n, 0);
            } else {
                printf("Line %-3d| Error: The token %s for lexeme %s does not match with the expected token %s\n", line, token_to_string(token), string_from(b, n), token_to_string(top(stack)));
                pop(stack);
            }
        }
    }

    if (top(stack) == string_to_symbol("$", symbol_map)) {
        if (current(b) == EOF) {
            printf("Parsing Successful!\n");
        } else {
            printf("Parsing Error: Extra tokens after the end of the program.\n");
        }
    } else {
        printf("Parsing Error: Some syntactical errors found.\n");
    }

    delete_stack(stack);

    printf("Constructing Parse Tree!\n");
    TREENODE parseTree = newNode(string_to_symbol("program", symbol_map));
    int rule_index = 0;
    populateNode(parseTree, rules_used, &rule_index, grammar_rules);
    printf("Parse Tree Constructed!\n");

    return parseTree;
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