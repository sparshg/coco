#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "hashmap.h"
#include "lexer.h"
#include "parser.h"
#include "stack.h"
#include "tokens.h"

int main(int argc, char* argv[]) {
    remove_comments("hi.txt", "clean.txt");
    BUF b = read_file("hi.txt");

    HASHMAP table = create_keyword_table();
    HASHMAP symbol_map = create_symbol_map();

    FILE* fd = fopen("ParsedTreet3.txt", "w");

    int** grammar_rules = get_grammar_rules(symbol_map);
    int** parse_table = get_parse_table(grammar_rules, symbol_map);

    STACK stack = createStack();
    initStack(stack, symbol_map);

    int line = 1;
    line += skip_whitespace(b);

    while (current(b) != EOF) {
        clear_saves(b);
        int n = push_state(b);
        int token = try_all(b, table);
        if (token < 0) {
            switch (token) {
                case WRONG_SYMBOL:
                    next(b);
                    break;
                case VAR_LEN_EXCEED:
                    // printf("Line no. %-3d Variable Identifier is longer than the prescribed length of 20 characters.\n", line);
                    break;
                case FUN_LEN_EXCEED:
                    // printf("Line no. %-3d Function Identifier is longer than the prescribed length of 30 characters.\n", line);
                    break;
                default:
                    back(b);
                    // printf("Line no. %-3d Unknown %-22s\n", line, string_from(b, n));
                    break;
            }
            line += skip_whitespace(b);
            continue;
        }

        if (token == TK_COMMENT) {
            // printf("Line no. %-3d Lexeme %-23s Token %s\n", line++, "%", token_to_string(token));
            line += skip_whitespace(b);
            continue;
        }

        // printf("Line no. %-3d Lexeme %-23s Token %s\n", line, string_from(b, n), token_to_string(token));
        int topSymbol = top(stack);
        // printf("Top of Stack: %d\n", topSymbol);
        if (topSymbol == string_to_symbol("$", symbol_map)) {
            printf("Parsing Successful\n");
            break;
        }

        // First handle case to stop parsing when top of stack is $ and token is also $
        while (isNonTerminal(top(stack))) {
            topSymbol = top(stack);
            int rule_no = parse_table[topSymbol - SYMBOLS_LEN + NT_LEN][token];
            // printf("Row: %d\n", topSymbol  - SYMBOLS_LEN + NT_LEN);
            // printf("Rule No: %d\n", rule_no);
            if (rule_no == -1) {
                printf("ParseError at %c (Hex: %x, Dec: %d)\n", current(b), current(b), current(b));
                break;
            }
            push_rule_to_stack(stack, grammar_rules, symbol_map, rule_no);
            // printf("Rule Used: %d\n", rule_no);
            fprintf(fd, "Rule Used: %d\n", rule_no);
        }

        // printf("Now stack is: \n");
        // printStack(stack);

        if (top(stack) == token) {
            printf("Matched: %s\n", token_to_string(token));
            pop(stack);
        } else {
            printf("SomeError at %c (Hex: %x, Dec: %d)\n", current(b), current(b), current(b));
            break;
        }

        line += skip_whitespace(b);
    }

    close_buf(b);
    fclose(fd);

    return 0;
}