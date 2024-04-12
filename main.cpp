#include "MinHeap.h"
#include "compress.h"
#include "utils.h"
#include <iostream>
#include <stdio.h>
using namespace std;
int main() {
   MinHeap *heap = new MinHeap(10);
    bool overflow = false;

    int freqTable[128] = {0};
    genFreqTable("./data/input.txt", freqTable);

    for (int i = 0; i < 128; i++)
        if (freqTable[i])
            heap->insertValues(std::string(1, char(i)), freqTable[i]);

    Node *root = tregen(heap);

    compress c = compress();
    string text = "life is so cool";
    c.createMaps(root, "");
    string codedtext = c.compressing(text);
    string original = c.decompressing(codedtext);
    cout << "original text: " << original << endl;
    cout << "Coded text: " << codedtext << endl;
   
    c.printEncoder();  


   // delete heap;
    return 0;
}