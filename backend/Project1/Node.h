#pragma once
#include <iostream>

class Node {
public:
    unsigned char letter;
    int freq;
    Node* left = NULL;
    Node* right = NULL;

    Node(char letter = NULL, int freq = 0, Node* left = NULL, Node* right = NULL);

    void update(char letter, int freq);
};
