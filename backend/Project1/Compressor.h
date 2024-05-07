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
	bool compressing(Node* root, string filePath, string outPath);
	bool decompressing(string compressedFilePath, string outputFilePath, int prevSize = 300);

	void printEncoder();
};
