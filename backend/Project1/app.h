#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include <algorithm>
#include "utils.h"
#include "Compress.h"

int startAPP() {
	string filePath = "input.txt";
	string dirPath = "";


	// Generate frequency table
	int freqTable[128] = { 0 };
	int size = 0;
	genFreqTable(filePath, freqTable, &size);

	// Create a min heap for Huffman tree
	MinHeap* heap = new MinHeap(size);
	bool overflow = false;

	for (int i = 0; i < 128; i++)
		if (freqTable[i])
			heap->insertValues(std::string(1, char(i)), freqTable[i]);

	// Generate Huffman tree
	Node* root = tregen(heap);
	delete heap;
	writeTreeToJsonFile(root, "huffman_tree.json");

	// Compress the file
	compress compressor = compress();
	bool validPath = true;
	compressor.createMaps(root);
	string codedText = compressor.compressing(filePath, &validPath);

	if (!validPath) {
		cout << "Warning: Path not valid." << endl;
	}

	if (codedText.empty()) {
		cout << "Warning: File content is empty." << endl;
	}

	// Save compressed data and map
	string fileNamePath = dirPath + "compressed_file.cod";
	bool success = saveStringToFile(fileNamePath, codedText);
	saveMapToFile(dirPath + "decoder_map.json", compressor.decoder);

	if (!success) {
		cout << "Warning: Directory path not valid." << endl;
	}


	// Cleanup
	delete root;



	return 0;
}