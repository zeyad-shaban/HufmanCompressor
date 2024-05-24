#pragma once
#include <iostream>
#include "Node.h"
#include <vector>

class Compressor {
private:
public:
	void compressSegment(char* inputData, char* segmentBuffer, unsigned int segmentSize, long long start, long long end, std::string charsTable[]);
	long long compressing(Node* root, std::string filePath, std::string outPath, float* progress);
	bool decompressing(Node* root, std::string compressedFilePath, std::string outputFilePath, int prevSize = 300);
};
