#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "hashmap.h"
#include "lexer.h"
#include "parser.h"
#include "stack.h"
#include "tokens.h"
#include "tree.h"

int checknull(int* nullable, int symbolId) {
    for (int i = 0; i < 15; i++) {
        if (nullable[i] == symbolId) {
            return 1;
        }
    }
    return 0;
}

int main(int argc, char* argv[]) {
    // remove_comments("hi.txt", "clean.txt");
    BUF b = read_file("t6.txt");

    HASHMAP table = create_keyword_table();
    HASHMAP symbol_map = create_symbol_map();

    FILE* fd = fopen("ParsedTree.txt", "w");

    int** grammar_rules = get_grammar_rules(symbol_map);
    ParseEntry** parse_table = get_parse_table(grammar_rules, symbol_map);

    STACK stack = create_stack();
    init_stack(stack, symbol_map);

    int line = 1;
    line += skip_whitespace(b);

    // int rules_used[500];
    // for (int i = 0; i < 500; i++) {
    //     rules_used[i] = -1;
    // }
    // int currentRule = 0;

    int nullable[] = {string_to_symbol("otherFunctions", symbol_map), string_to_symbol("output_par", symbol_map), string_to_symbol("remaining_list", symbol_map), string_to_symbol("typeDefinitions", symbol_map), string_to_symbol("moreFields", symbol_map), string_to_symbol("declarations", symbol_map), string_to_symbol("global_or_not", symbol_map), string_to_symbol("otherStmts", symbol_map), string_to_symbol("option_single", symbol_map), string_to_symbol("moreExpansion", symbol_map), string_to_symbol("outputParameters", symbol_map), string_to_symbol("expression'", symbol_map), string_to_symbol("term'", symbol_map), string_to_symbol("optionalReturn", symbol_map), string_to_symbol("more_ids", symbol_map)};

    // print_parse_table(parse_table);

    while (current(b) != EOF) {
        clear_saves(b);
        int n = push_state(b);
        int token = try_all(b, table);
        int new_lexeme_req = 0;
        if (token < 0) {
            switch (token) {
                case WRONG_SYMBOL:
                    printf("Line %-3d| Unknown symbol %c\n", line, current(b));
                    next(b);
                    break;
                case VAR_LEN_EXCEED:
                    printf("Line %-3d| Variable Identifier is longer than the prescribed length of 20 characters.\n", line);
                    break;
                case FUN_LEN_EXCEED:
                    printf("Line %-3d| Function Identifier is longer than the prescribed length of 30 characters.\n", line);
                    break;
                default:
                    back(b);
                    printf("Line %-3d| Unknown pattern %s\n", line, string_from(b, n));
                    break;
            }
            line += skip_whitespace(b);
            continue;
        }

        if (token == TK_COMMENT) {
            // printf("Line no. %-3d Lexeme %-23s Token %s\n", line, "%", token_to_string(token));
            line++;
            line += skip_whitespace(b);
            continue;
        }
        // printf("Line no. %-3d Lexeme %-23s Token %s\n", line, string_from(b, n), token_to_string(token));

    label:
        if (is_non_terminal(top(stack))) {
            ParseEntry rule = parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token];
            if (rule.rule_no == -1) {
                // if(parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token].isFirst{
                if (checknull(nullable, top(stack))) {
                    // printf("Popped: %s\n", symbols[top(stack)]);
                    pop(stack);
                    goto label;
                }
                printf("Line %-3d| Invalid token %s encountered with value %s stack top %s\n", line, token_to_string(token), string_from(b, n), symbols[top(stack)]);

                // }
                if (!rule.isFollow) {
                    line += skip_whitespace(b);
                    continue;
                }
                pop(stack);
                goto label;
            } else {
                push_rule_to_stack(stack, grammar_rules, symbol_map, rule.rule_no);
                fprintf(fd, "Rule Used: %d and lexeme: %s\n", rule.rule_no, string_from(b, n));
                // rules_used[currentRule++] = rule.rule_no;
                goto label;
            }
        }

        else {
            if (top(stack) == token) {
                // printf("Matched: %s\n", token_to_string(token));
                pop(stack);
            } else {
                printf("Line %-3d| Error: The token %s for lexeme %s does not match with the expected token %s\n", line, token_to_string(token), string_from(b, n), token_to_string(top(stack)));
                pop(stack);
                goto label;
            }
        }

        line += skip_whitespace(b);
    }

    if (top(stack) == string_to_symbol("$", symbol_map)) {
        if (current(b) == EOF) {
            printf("Parsing Successful\n");
        } else {
            printf("Parsing Error: Extra tokens after the end of the program\n");
        }
        // printf("Parsing Successful\n");
    }

    // TREENODE parseTree = newNode(string_to_symbol("program", symbol_map));
    // int rule_index = 0;
    // for(int i=0;i<currentRule;i++){
    //     printf("%d\n", rules_used[i]);
    // }
    // populateNode(parseTree, rules_used, &rule_index, grammar_rules);
    // printf("TreeDone\n");
    // printTree(parseTree, 1);
    close_buf(b);
    fclose(fd);

    return 0;
}