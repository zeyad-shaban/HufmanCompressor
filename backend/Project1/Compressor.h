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
	long long  compressing(Node* root, string filePath, string outPath);
	bool decompressing(Node* root, string compressedFilePath, string outputFilePath, int prevSize = 300);
};
