// Group 26
// Rishi Gupta     (2021A7PS0690P)
// Sparsh Goenka   (2021A7PS2413P)
// Utkarsh Sharma  (2021A7PS0693P)
// Saumya Sharma   (2021A7PS0544P)
// Akshat Bajpai   (2021A7PS0573P)
#include "tree.h"

#include "parser.h"
#include "tokens.h"

TREENODE create_node(int symbolId, char* lexeme, int line) {
    TREENODE node = (TREENODE)malloc(sizeof(TreeNode));
    node->symbol = symbolId;
    node->lexeme = lexeme;
    node->line = line;
    node->child_len = 0;
    node->children = (TREENODE*)malloc(PROD_RULE_LINE_LEN * sizeof(TREENODE));
    for (int i = 0; i < PROD_RULE_LINE_LEN; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void add_child(TREENODE node, TREENODE child) {
    node->children[node->child_len++] = child;
}

void print_tree(FILE* f, TREENODE node, char* parent_sym) {
    if (node->child_len > 0)
        print_tree(f, node->children[node->child_len - 1], symbol_to_string(node->symbol));

    if (symbol_to_nt(node->symbol) >= 0) {
        fprintf(f, "%25s%10s%15s%15s%25s%20s%25s\n", "----", "----", "----", "----", parent_sym, " No", symbol_to_string(node->symbol));
    } else if (is_epsilon(node->symbol)) {
        fprintf(f, "%25s%10s%15s%15s%25s%20s%25s\n", "----", "----", "epsilon", "----", parent_sym, "Yes", "----");
    } else {
        fprintf(f, "%25s%10d%15s%15s%25s%20s%25s\n", node->lexeme, node->line, symbol_to_string(node->symbol), (node->symbol == TK_NUM || node->symbol == TK_RNUM) ? node->lexeme : "----", parent_sym, "Yes", "----");
    }
    // printf("%-12s %-3d %-20s %-20s\n", (node->lexeme) ? node->lexeme : "----", node->line, symbol_to_string(parent_sym), symbol_to_string(node->symbol));

    for (int i = node->child_len - 2; i >= 0; i--)
        print_tree(f, node->children[i], symbol_to_string(node->symbol));
}
