#include "MinHeap.h"
#include "compress.h"
#include "utils.h"
#include <iostream>
#include <stdio.h>
using namespace std;

int main() {
    int freqTable[128] = {0};
    int size = 0;
    char *filePath = "./data/input.txt";
    genFreqTable(filePath, freqTable, &size);

    MinHeap *heap = new MinHeap(size);
    bool overflow = false;

    for (int i = 0; i < 128; i++)
        if (freqTable[i])
            heap->insertValues(std::string(1, char(i)), freqTable[i]);

    Node *root = tregen(heap);
    delete heap;

    compress c = compress();
    string text = "test";

    c.createMaps(root, "");
    string codedtext = c.compressing(filePath);
    string original = c.decompressing(codedtext);

    // cout << "original text: " << original << endl;
    // cout << "Coded text: " << codedtext << endl;
    // c.printEncoder();
    std::cout << "the program has ended" << std::endl;
    return 0;
}

// abcdefghijklmnopqrstuvwxyz