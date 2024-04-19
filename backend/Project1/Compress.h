#pragma once
#include "Node.h"
#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
using namespace std;

class compress {
private:
    int MAX_BUFFER_SIZE = 16000000;

public:
    unordered_map<string, string> encoder;
    unordered_map<string, string> decoder;

    compress();

    void createMaps(Node* root, string code = "");

    string compressing(string filePath, string outputFilePath, bool* validPath, int MAX_BUFFER_SIZE = 16000000);
    string decompressing(string compressedFilePath, string outputFilePath, bool* validPath, int prevSize = 300);

    void printEncoder();
};
