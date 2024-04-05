#pragma once
#include <stdio.h>

class BinNode {
  public:
    char *letters;
    int freqs;
    BinNode *left = NULL;
    BinNode *right = NULL;

    BinNode(char *letters, int freqs, BinNode *left = NULL, BinNode *right = NULL) {
        this->letters = letters;  
        this->freqs = freqs;
        this->left = left;
        this->right = right;
    }
};

class Box {
  public:
    BinNode *node = NULL;
    Box *next = NULL;
    Box(BinNode *node, Box *next = NULL) {
        this->node = node;
        this->next = next;
    }
};