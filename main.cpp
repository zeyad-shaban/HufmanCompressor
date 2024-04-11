#include "MinHeap.h"
#include <iostream>
#include <stdio.h>

int main() {
    MinHeap *heap = new MinHeap(10);
    bool overflow = false;

    // TODO IMPLEMENT FREQ TABLE FROM FILE
    int freqTable[255];
    freqTable['a'] = 10;
    freqTable['b'] = 15;
    freqTable['c'] = 5;
    freqTable['d'] = 1;

    heap->insert("a", 10, &overflow);
    heap->insert("b", 9, &overflow);
    heap->insert("b", 9, &overflow);
    heap->insert("b", 1, &overflow);

    if (overflow)
        std::cout << "Some values are missimng due to the limited size" << std::endl;

    Node min = heap->extractMin();
    std::cout << heap->traverse(0) << std::endl;


    delete heap;
    return 0;
}