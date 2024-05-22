#pragma once
#include <iostream>
#include "Node.h"

class Compressor {
private:
public:
	long long  compressing(Node* root, std::string filePath, std::string outPath, float* progress);
	bool decompressing(Node* root, std::string compressedFilePath, std::string outputFilePath, int prevSize = 300);
};
