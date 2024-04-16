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
	std::cout << "->Generating frequency-table...\n";
	if (genFreqTable(filePath, freqTable, &size)) {
		std::cout << "frequency-table Generated";
	}
	else {
		std::cout << "frequency-table faild:can't open file\n";
	}
	std::ofstream file("frequency_table.txt");
	for (int i = 0; i < 128; ++i) {
		if (freqTable[i] != 0) {

			file << char(i) << ":" << freqTable[i] << " "; // Write each element followed by a space
		}
	}
	file.close();

	std::cout << "->Creating MinHeap with size of:" << size<< "\n";
	// Create a min heap for Huffman tree
	MinHeap* heap = new MinHeap(size);
	bool overflow = false;
	std::cout << "MinHeap Created\n";

	std::cout << "->Creating a MinHeap with the frequency-table...\n";
	for (int i = 0; i < 128; i++)
		if (freqTable[i])
			heap->insertValues(std::string(1, char(i)), freqTable[i]);
	std::cout << "MinHeap Created ";

	// Generate Huffman tree
	std::cout << "->Creating HuffmanTree from MinHeap...\n";
	Node* root = tregen(heap);
	delete heap;
	std::cout << "HuffmanTree created\n MinHeap deleted\n";

	writeTreeToJsonFile(root, "huffman_tree.json");

	// Compress the file
	compress* compressor = new compress();
	bool validPath = true;
	std::cout << "->compressing using HuffmanTree\n";
	compressor->createMaps(root);
	string codedTextPrev = compressor->compressing(filePath,"compressed_file.com", &validPath);


	// Save compressed data and map
	saveMapToFile(string("decoder_map.json"), compressor->decoder);

	std::cout << "Compressed and saved 2 files\n";

	// Cleanup
	delete root;



	return 0;
}