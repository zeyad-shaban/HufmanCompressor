#pragma once
#include "Node.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

class Compressor {
private:
    int MAX_BUFFER_SIZE = 16000000; // TODO determine good value for this

public:
    unordered_map<string, string> encoder;
    unordered_map<string, string> decoder;

    Compressor();

    void createMaps(Node* root, string code = "");

    string compressing(string filePath, streamoff start, streamoff end);
    string decompressing(string compressedFilePath, string outputFilePath, int prevSize = 300);

    void printEncoder();
};
