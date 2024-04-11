#include "MinHeap.h"
#include "compress.h"
#include "utils.h"
#include <iostream>
#include <stdio.h>

int main() {
    MinHeap *heap = new MinHeap(10);
    bool overflow = false;

    int freqTable[128] = {0};
    genFreqTable("./data/input.txt", freqTable);

    for (int i = 0; i < 128; i++)
        if (freqTable[i])
            heap->insertValues(std::string(1, char(i)), freqTable[i]);

    Node *root = tregen(heap);

    compress compressor = compress();
    compressor.EncoderList(root, "");
    compressor.encode("GUYS I CAME UP WITH THE PERFECT NAME FOR THIS PROJECT, 3A-Z!!!!! yes fr");

    delete heap;
    return 0;
}