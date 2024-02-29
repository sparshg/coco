#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "buffer.h"
#include "hashmap.h"
#include "lexer.h"
#include "parser.h"
#include "tokens.h"
#include "stack.h"

int main(int argc, char* argv[]) {
    remove_comments("t3.txt", "clean.txt");
    BUF b = read_file("clean.txt");

    HASHMAP table = create_keyword_table();
    HASHMAP symbol_map = create_symbol_map();

    FILE* fd = fopen("ParsedTreet3.txt", "w");

    int** grammar_rules = get_grammar_rules(symbol_map);
    int** parse_table = get_parse_table(grammar_rules, symbol_map);

    // for (int i = 0; i < PROD_RULES_LEN; i++) {
    // printf("%d ", i);
    //     for (int j = 0; j < PROD_RULE_LINE_LEN; j++) {
    //         if (grammar_rules[i][j] == -1)
    //             break;
    //         printf("%d ", grammar_rules[i][j]);
    //     }
    //     printf("\n");
    // }
    

    STACK stack = createStack();
    initStack(stack, symbol_map);

    while (current(b) != EOF) {
        clear_saves(b);
        int n = push_state(b);
        int token = try_all(b, table);
        if (token == -1) {
            printf("Unknown: %s\n", string_from(b, n));
            printf("LexError at %c (Hex: %x, Dec: %d)\n", current(b), current(b), current(b));
            break;
        } 
        
        else {
            printf("Found: %d %s, %s\n", token, token_to_string(token), string_from(b, n));
            
            int topSymbol = top(stack);
            // printf("Top of Stack: %d\n", topSymbol);
            if(topSymbol == string_to_symbol("$", symbol_map)){
                printf("Parsing Successful\n");
                break;
            }

            //First handle case to stop parsing when top of stack is $ and token is also $

            while(isNonTerminal(top(stack))){
                topSymbol = top(stack);
                int rule_no = parse_table[topSymbol - SYMBOLS_LEN + NT_LEN][token];
                // printf("Row: %d\n", topSymbol  - SYMBOLS_LEN + NT_LEN);
                // printf("Rule No: %d\n", rule_no);
                if(rule_no == -1){
                    printf("ParseError at %c (Hex: %x, Dec: %d)\n", current(b), current(b), current(b));
                    break;
                }
                push_rule_to_stack(stack, grammar_rules, symbol_map, rule_no);
                // printf("Rule Used: %d\n", rule_no);
                fprintf(fd, "Rule Used: %d\n", rule_no);
            }


            // printf("Now stack is: \n");
            // printStack(stack);


            if(top(stack) == token){
                printf("Matched: %s\n", token_to_string(token));
                pop(stack);
            }


            else{
                printf("SomeError at %c (Hex: %x, Dec: %d)\n", current(b), current(b), current(b));
                break;
            }

        }
        // printf("Now at %c (Hex: %x, Dec: %d, Diff: %ld)\n", *str, *str, *str, str - start);
        skip_whitespace(b);
        // printf("After whitespace at %c (Hex: %x, Dec: %d, Diff: %ld)\n\n", *str, *str, *str, str - start);
    }


    closeFile(b);
    fclose(fd);

    return 0;
}