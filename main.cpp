#include "PriorityQueue.h"
#include "generateHuffTree.h"
#include "utils.h"
#include <stdio.h>

int main() {
    PriorityQueue *freqsQ = new PriorityQueue();
    freqsQ->enQueue(binNodeGenerator('a', 1));
    freqsQ->enQueue(binNodeGenerator('b', 2));
    freqsQ->enQueue(binNodeGenerator('c', 3));

    BinNode *huffRoot = generateHuffTree(freqsQ);
    return 0;
}