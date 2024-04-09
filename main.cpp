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

    delete heap;
    return 0;
}