#include "Node.h"

Node::Node(std::string letters, int freq, Node* left, Node* right) {
    this->letters = letters;
    this->freq = freq;
    this->left = left;
    this->right = right;
}

void Node::update(std::string letters, int freq) {
    this->letters = letters;
    this->freq = freq;
}
