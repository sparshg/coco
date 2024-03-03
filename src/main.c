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
    // remove_comments("hi.txt", "clean.txt");
    BUF b = read_file("t5.txt");

    HASHMAP table = create_keyword_table();
    HASHMAP symbol_map = create_symbol_map();

    // FILE* fd = fopen("ParsedTree.txt", "w");

    int** grammar_rules = get_grammar_rules(symbol_map);
    ParseEntry** parse_table = get_parse_table(grammar_rules, symbol_map);

    STACK stack = create_stack();
    init_stack(stack, symbol_map);

    int line = 1;
    line += skip_whitespace(b);

    int rules_used[500];
    for(int i=0;i<500;i++){
        rules_used[i]=-1;
    }
    int currentRule = 0;
    printf("\n");
    // print_parse_table(parse_table);


    while (current(b) != EOF) {
        clear_saves(b);
        int n = push_state(b);
        int token = try_all(b, table);
        int new_lexeme_req = 0;
        if (token < 0) {
            switch (token) {
                case WRONG_SYMBOL:
                    next(b);
                    break;
                case VAR_LEN_EXCEED:
                    printf("Line no. %-3d Variable Identifier is longer than the prescribed length of 20 characters.\n", line);
                    break;
                case FUN_LEN_EXCEED:
                    printf("Line no. %-3d Function Identifier is longer than the prescribed length of 30 characters.\n", line);
                    break;
                default:
                    back(b);
                    printf("Line no. %-3d Unknown %-22s\n", line, string_from(b, n));
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

    label:
        if (is_non_terminal(top(stack))) {
            ParseEntry rule = parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token];
            if (rule.rule_no == -1) {
                printf("Line %d: Invalid token %s encountered with value %s stack top %s\n", line, token_to_string(token), string_from(b, n), symbols[top(stack)]);

                if (!rule.isFollow) {
                    line += skip_whitespace(b);
                    continue;
                }
                pop(stack);
                goto label;
            } 
            else {
                push_rule_to_stack(stack, grammar_rules, symbol_map, rule.rule_no);
                // fprintf(fd, "Rule Used: %d and lexeme: %s\n", rule.rule_no, string_from(b, n));
                rules_used[currentRule++] = rule.rule_no;
                goto label;
            }
        } else {
            if (top(stack) == token) {
                printf("Matched: %s\n", token_to_string(token));
                pop(stack);
                // t = 0;
            } else {
                // if (t == 0)
                printf("Line %d: Error: The token %s for lexeme %s does not match with the expected token %s\n", line, token_to_string(token), string_from(b, n), token_to_string(top(stack)));
                // t = 1;
                pop(stack);
                goto label;
            }
        }
        // printf("Line no. %-3d Lexeme %-23s Token %s\n", line, string_from(b, n), token_to_string(token));

        // First handle case to stop parsing when top of stack is $ and token is also $
        // while (is_non_terminal(top(stack))) {
        //     int rule_no = parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token].rule_no;

        //     if (rule_no == -1) {
        //         // printf("ParseError at %c (Hex: %x, Dec: %d)\n", current(b), current(b), current(b));
        //         // break;
        //         printf("Line %d: Invalid token %s encountered with value %s stack top %s\n", line, token_to_string(token), string_from(b, n), symbols[top(stack)]);

        //         if (parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token].isFollow) {
        //             pop(stack);
        //             // goto label;
        //             continue;
        //         }
        //         // while(!is_empty(stack) && !is_end_symbol(top(stack)) && is_non_terminal(top(stack))
        //         //                                                     && !parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token].isFirst
        //         //                                                     && !parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token].isFollow) {
        //         //     pop(stack);
        //         // }
        //         // if(!is_non_terminal(top(stack))){
        //         //     break;
        //         // }
        //         // if(parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token].isFirst){
        //         //     goto label;
        //         // }
        //         // else{
        //         //     pop(stack);
        //         //     goto label;
        //         // }

        //         else {
        //             // new_lexeme_req = 1;
        //             line += skip_whitespace(b);
        //             break;
        //         }
        //     }
        //     push_rule_to_stack(stack, grammar_rules, symbol_map, rule_no);
        //     // printf("Rule Used: %d\n", rule_no);
        //     fprintf(fd, "Rule Used: %d and lexeme: %s\n", rule_no, string_from(b, n));
        // }

        // // printf("Now stack is: \n");
        // // printStack(stack);

        // // if(new_lexeme_req == 1){
        // //     continue;
        // // }

        // while (!is_non_terminal(top(stack))) {
        //     if (top(stack) == token) {
        //         printf("Matched: %s\n", token_to_string(token));
        //         pop(stack);
        //         t = 0;
        //         break;
        //     } else {
        //         // printf("I GIVE UP: Top of Stack: %s, Token: %s\n", symbols[top(stack)], token_to_string(token));
        //         // break;
        //         if (t == 0)
        //             printf("Line %d: Error: The token %s for lexeme %s does not match with the expected token %s\n", line, token_to_string(token), string_from(b, n), token_to_string(top(stack)));
        //         t = 1;
        //         // break;
        //         pop(stack);
        //         // goto label;
        //     }
        // }
        line += skip_whitespace(b);
    }

    if (top(stack) == string_to_symbol("$", symbol_map)) {
        printf("Parsing Successful\n");
    }

    close_buf(b);
    // fclose(fd);

    return 0;
}