#include "tree.h"

#include "parser.h"
#include "tokens.h"

TREENODE newNode(int symbolId) {
    TREENODE node = (TREENODE)malloc(sizeof(TreeNode));
    node->symbolId = symbolId;
    // node->isLeaf = 0;
    node->children = (TREENODE*)malloc(sizeof(TREENODE) * 15);
    for (int i = 0; i < PROD_RULE_LINE_LEN; i++) {
        node->children[i] = NULL;
    }
    return node;
}

void populateNode(TREENODE node, int* rules_used, int* rule_index, int** grammar_rules) {
    int rule_no = rules_used[*rule_index];
    if (rule_no == -1) {
        return;
    }
    (*rule_index)++;
    int* rule = grammar_rules[rule_no];
    for (int i = 2; i <= rule[0]; i++) {
        node->children[i - 2] = newNode(rule[i]);
    }
    printf("Rule Used: %d\n", rule_no);
    int i = 0;
    while (node->children[i] != NULL) {
        // printf("child: %s\n", symbols[node->children[i]->symbolId]);
        if (is_non_terminal(node->children[i]->symbolId)) {
            populateNode(node->children[i], rules_used, rule_index, grammar_rules);
        }
        i++;
    }
    return;
}

void printTree(TREENODE node, int level) {
    printf("%6d %s\n", level, symbols[node->symbolId]);
    int i = 0;
    while (node->children[i]) {
        printTree(node->children[i], level+1);
        i++;
    }
}
