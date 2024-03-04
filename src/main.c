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

// int driver() {

//     BUF b = read_file("t5.txt");

//     HASHMAP table = create_keyword_table();
//     HASHMAP symbol_map = create_symbol_map();

//     FILE* fd = fopen("ParsedTree.txt", "w");

//     int** grammar_rules = get_grammar_rules(symbol_map);
//     ParseEntry** parse_table = get_parse_table(grammar_rules, symbol_map);

//     STACK stack = create_stack();
//     init_stack(stack, symbol_map);

//     int line = 1;
//     line += skip_whitespace(b);

//     int rules_used[500];
//     for (int i = 0; i < 500; i++) {
//         rules_used[i] = -1;
//     }
//     int currentRule = 0;

//     int nullable[] = {string_to_symbol("otherFunctions", symbol_map), string_to_symbol("output_par", symbol_map), string_to_symbol("remaining_list", symbol_map), string_to_symbol("typeDefinitions", symbol_map), string_to_symbol("moreFields", symbol_map), string_to_symbol("declarations", symbol_map), string_to_symbol("global_or_not", symbol_map), string_to_symbol("otherStmts", symbol_map), string_to_symbol("option_single", symbol_map), string_to_symbol("moreExpansion", symbol_map), string_to_symbol("outputParameters", symbol_map), string_to_symbol("expression'", symbol_map), string_to_symbol("term'", symbol_map), string_to_symbol("optionalReturn", symbol_map), string_to_symbol("more_ids", symbol_map)};

//     // print_parse_table(parse_table);

//     while (current(b) != EOF) {
//         clear_saves(b);
//         int n = push_state(b);
//         int token = get_next_token(b, table);
//         if (token < 0) {
//             switch (token) {
//                 case WRONG_SYMBOL:
//                     printf("Line %-3d| Unknown symbol %c\n", line, current(b));
//                     next(b);
//                     break;
//                 case VAR_LEN_EXCEED:
//                     printf("Line %-3d| Variable Identifier is longer than the prescribed length of 20 characters.\n", line);
//                     break;
//                 case FUN_LEN_EXCEED:
//                     printf("Line %-3d| Function Identifier is longer than the prescribed length of 30 characters.\n", line);
//                     break;
//                 default:
//                     back(b);
//                     printf("Line %-3d| Unknown pattern %s\n", line, string_from(b, n));
//                     break;
//             }
//             line += skip_whitespace(b);
//             continue;
//         }

//         if (token == TK_COMMENT) {
//             // printf("Line no. %-3d Lexeme %-23s Token %s\n", line, "%", token_to_string(token));
//             line++;
//             line += skip_whitespace(b);
//             continue;
//         }
//         printf("Line no. %-3d Lexeme %-23s Token %s\n", line, string_from(b, n), token_to_string(token));

//     label:
//         if (is_non_terminal(top(stack))) {
//             ParseEntry rule = parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token];
//             if (rule.rule_no == -1) {
//                 // if(parse_table[top(stack) - SYMBOLS_LEN + NT_LEN][token].isFirst{
//                 if (checknull(nullable, top(stack))) {
//                     // printf("Popped: %s\n", symbols[top(stack)]);
//                     pop(stack);
//                     goto label;
//                 }
//                 printf("Line %-3d| Invalid token %s encountered with value %s stack top %s\n", line, token_to_string(token), string_from(b, n), symbols[top(stack)]);

//                 // }
//                 if (!rule.isFollow) {
//                     line += skip_whitespace(b);
//                     continue;
//                 }
//                 pop(stack);
//                 goto label;
//             } else {
//                 push_rule_to_stack(stack, grammar_rules, symbol_map, rule.rule_no);
//                 fprintf(fd, "Rule Used: %d and lexeme: %s\n", rule.rule_no, string_from(b, n));
//                 rules_used[currentRule++] = rule.rule_no;
//                 goto label;
//             }
//         }

//         else {
//             if (top(stack) == token) {
//                 // printf("Matched: %s\n", token_to_string(token));
//                 pop(stack);
//             } else {
//                 printf("Line %-3d| Error: The token %s for lexeme %s does not match with the expected token %s\n", line, token_to_string(token), string_from(b, n), token_to_string(top(stack)));
//                 pop(stack);
//                 goto label;
//             }
//         }

//         line += skip_whitespace(b);
//     }

//     // if (top(stack) == string_to_symbol("$", symbol_map)) {
//     //     if (current(b) == EOF) {
//     //         printf("Parsing Successful\n");
//     //     } else {
//     //         printf("Parsing Error: Extra tokens after the end of the program\n");
//     //     }
//     //     // printf("Parsing Successful\n");
    
//     // }

//     TREENODE parseTree = newNode(string_to_symbol("program", symbol_map));
//     int rule_index = 0;
//     for(int i=0;i<currentRule;i++){
//         printf("%d\n", rules_used[i]);
//     }
//     populateNode(parseTree, rules_used, &rule_index, grammar_rules);
//     // printf("TreeDone\n");
//     // printTree(parseTree, 1);

//     close_buf(b);
//     fclose(fd);

// }




int main(int argc, char* argv[]) {
    // if(argc != 3){
    //     printf("Usage: ./compiler <input_file> <parse_tree_out_file>\n");
    //     return 1;
    // }
    printf("=================================================================================\n");
    printf("--------------------------Welcome to CoCo Compiler v1.0--------------------------\n");
    printf("=================================================================================\n\n");
    printf("Dear User,\n");
    printf("This sophisticated compiler has been meticulously crafted to facilitate the compilation process of programs written in CoCo, ensuring precision, efficiency, and reliability.\n");

    printf("As you engage with our compiler, we encourage adherence to best practices in software development, leveraging the robust capabilities embedded within our framework to unleash the full potential of your programming endeavors.");


    HASHMAP table = create_keyword_table();
    HASHMAP symbol_map = create_symbol_map();
    int** grammar_rules = get_grammar_rules(symbol_map);
    ParseEntry** parse_table = get_parse_table(grammar_rules, symbol_map);

    


    int option;
    TREENODE parseTree;

    do {
        printf("\n\nChoose an option:\n");
        printf("0 : For exit\n");
        printf("1 : For removal of comments\n");
        printf("2 : For printing the token list generated by the lexer\n");
        printf("3 : For parsing to verify the syntactic correctness and printing the parse tree\n");
        printf("4 : For printing the total CPU time\n");

        printf("Enter option: ");
        scanf("%d", &option);

        switch (option) {
            case 0:
                printf("Exiting...\n");
                break;
            case 1:
                remove_comments("t5.txt");
                break;
            case 2:
                print_lexer_output("t5.txt");
                break;
            case 3:
                // TREENODE parseTree;
                parseTree = parse_input_source_code("t5.txt", table, symbol_map, grammar_rules, parse_table);
                printf("\n");
                printf("_______________________________________________________________________________________\n");
                printTree(parseTree, 1);
                break;
            case 4:
                // print_total_time();
                break;
            default:
                printf("Invalid option. Please choose again.\n");
                break;
        }
    } while (option != 0);

    printf("=================================================================================\n");
    printf("----------------------------Thank you for using CoCo!----------------------------\n");
    printf("=================================================================================\n\n");
    printf("Group Memebers:\n");
    printf("Rishi Gupta     (2019A7PS0690P)\n");
    printf("Sparsh Goenka   (2019A7PS2413P)\n");
    printf("Utkarsh Sharma  (2019A7PS0693P)\n");
    printf("Saumya Sharma   (2019A7PS0544P)\n");
    printf("Akshat Bajpai   (2019A7PS0573P)\n");

    delete_hashmap(table);
    delete_hashmap(symbol_map);

    return 0;
}