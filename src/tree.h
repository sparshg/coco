#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>

#include "tokens.h"

typedef struct TreeNode TreeNode;
typedef struct TreeNode* TREENODE;

struct TreeNode {
    // leftmost in array = rightmost in tree
    TREENODE* children;
    char* lexeme;
    int line, symbol, child_len;
};

TREENODE create_node(int symbolId, char* lexeme, int line);
void add_child(TREENODE node, TREENODE child);
void print_tree(TREENODE node, int parent_sym);

#endif