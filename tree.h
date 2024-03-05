// Group 26
// Rishi Gupta     (2021A7PS0690P)
// Sparsh Goenka   (2021A7PS2413P)
// Utkarsh Sharma  (2021A7PS0693P)
// Saumya Sharma   (2021A7PS0544P)
// Akshat Bajpai   (2021A7PS0573P)
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
void print_tree(FILE* f, TREENODE node, char* parent_sym);

#endif