#pragma once
#include "Node.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

class Compressor {
private:
public:
    unordered_map<char, string> encoder;
    unordered_map<string, string> decoder;

    Compressor();

    void createMaps(Node* root, string code = "");

    bool compressing(string filePath, string outPath);
    string decompressing(string compressedFilePath, string outputFilePath, int prevSize = 300);

    void printEncoder();
};
