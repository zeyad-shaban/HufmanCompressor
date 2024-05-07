#include "Node.h"

Node::Node(char letter, int freq, Node* left, Node* right) {
    this->letter = letter;
    this->freq = freq;
    this->left = left;
    this->right = right;
}

void Node::update(char letter, int freq) {
    this->letter = letter;
    this->freq = freq;
}
