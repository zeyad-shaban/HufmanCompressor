#pragma once
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>

class Node {
public:
    unsigned char letter;
    int freq;
    Node* left = NULL;
    Node* right = NULL;

    Node(char letter = NULL, int freq = 0, Node* left = NULL, Node* right = NULL);

    void update(char letter, int freq);
};
