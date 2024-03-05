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

void print_tree(TREENODE node, int parent_sym) {
    if (node->child_len > 0)
        print_tree(node->children[node->child_len - 1], node->symbol);

    if (symbol_to_nt(node->symbol) >= 0) {
        printf("%20s%10s%15s%15s%30s%20s%30s\n", "----", "----", "----", "----", symbol_to_string(parent_sym), " No", symbol_to_string(node->symbol));
    } else if (is_epsilon(node->symbol)) {
        printf("%20s%10s%15s%15s%30s%20s%30s\n", "----", "----", "epsilon", "----", symbol_to_string(parent_sym), "Yes", "----");
    } else {
        printf("%20s%10d%15s%15s%30s%20s%30s\n", node->lexeme, node->line, symbol_to_string(node->symbol), (node->symbol == TK_NUM || node->symbol == TK_RNUM) ? node->lexeme : "----", symbol_to_string(parent_sym), "Yes", "----");
    }
    // printf("%-12s %-3d %-20s %-20s\n", (node->lexeme) ? node->lexeme : "----", node->line, symbol_to_string(parent_sym), symbol_to_string(node->symbol));

    for (int i = node->child_len - 2; i >= 0; i--)
        print_tree(node->children[i], node->symbol);
}
