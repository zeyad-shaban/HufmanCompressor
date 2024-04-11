#include "MinHeap.h"
#include <iostream>
#include <stdio.h>

Node* tregen(MinHeap *heap) {
    while (heap->heapPtr > 1) {
        Node *node1 = heap->extractMin();
        Node *node2 = heap->extractMin();
        Node *nNode = new Node((node1->letters + node2->letters), (node1->freq + node2->freq), node1, node2);

        heap->insertNode(*nNode);
        delete (nNode);
    }
    return heap->extractMin();
}

int main() {
    MinHeap *heap = new MinHeap(10);
    bool overflow = false;

    // TODO IMPLEMENT FREQ TABLE FROM FILE
    int freqTable[128];
    freqTable['a'] = 10;
    freqTable['b'] = 15;
    freqTable['c'] = 5;
    freqTable['d'] = 1;

    heap->insertValues("a", 10);
    heap->insertValues("b", 15);
    heap->insertValues("c", 5);
    heap->insertValues("d", 1);

    Node* root = tregen(heap);

    delete heap;
    return 0;
}