#include "BinNode.h"
#include <stdio.h>

BinNode *binNodeGenerator(char letter, int frequency) {
    char *letters = new char[2];
    letters[0] = letter;
    letters[1] = '\0';
    BinNode *node = new BinNode(letters, frequency);
    return node;
}