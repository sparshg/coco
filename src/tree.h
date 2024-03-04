#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdlib.h>

typedef struct TreeNode TreeNode;
typedef struct TreeNode* TREENODE;

struct TreeNode{
    int symbolId;
    // int isLeaf;
    TREENODE* children;
};

TREENODE newNode(int symbolId);
void populateNode(TREENODE node, int* rules_used, int* rule_index, int** grammar_rules);
void printTree(TREENODE node, int level);






#endif