#include "MinHeap.h"
#include "compress.h"
#include "utils.h"
#include <iostream>
#include <stdio.h>
#include <chrono>
using namespace std;

int main() {


    std::string filePath = "./data/input.txt";

    std::string base_filename = filePath.substr(filePath.find_last_of("/\\") + 1);
    std::string::size_type const p(base_filename.find_last_of('.'));
    std::string file_without_extension = base_filename.substr(0, p);

    // TOOD DISPLAY A LITTLE LOAD BAR

    int freqTable[128] = {0};
    int size = 0;
    genFreqTable(filePath, freqTable, &size);

    MinHeap *heap = new MinHeap(size);
    bool overflow = false;

    for (int i = 0; i < 128; i++)
        if (freqTable[i])
            heap->insertValues(std::string(1, char(i)), freqTable[i]);

    Node *root = tregen(heap);
    delete heap;

   compress compressor = compress();

    compressor.createMaps(root, "");
       auto start = std::chrono::high_resolution_clock::now();
    string codedtext = compressor.compressing(filePath, file_without_extension);
 auto stop = std::chrono::high_resolution_clock::now();
   // string original = compressor.decompressing(codedtext, file_without_extension);

auto duration = std::chrono::duration_cast<std::chrono::seconds>(stop - start);
std::cout << "program took : "
          << duration.count() << " seconds" << std::endl;

    return 0;
}