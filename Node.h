#pragma once
#include <iostream>

class Node {
  public:
    std::string letters;
    int freq;
    Node *left = NULL;
    Node *right = NULL;

    Node(std::string letters = "", int freq = 0, Node *left = NULL, Node *right = NULL) {
        this->letters = letters;
        this->freq = freq;
        this->left = left;
        this->right = right;
    }

    void update(std::string letters, int freq) {
        this->letters = letters;
        this->freq = freq;
    }
};
