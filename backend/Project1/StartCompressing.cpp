#include "StartCompressing.h"

int startCompressing(string filePath, string dirPath, unordered_map<string, string>* encoderPtr, Node* rootPtr, string* textPrevPtr) {
	std::string base_filename = filePath.substr(filePath.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);


	// Generate frequency table
	int freqTable[128] = { 0 };
	std::cout << "->Generating frequency-table...\n";
	if (genFreqTable(filePath, freqTable)) {
		std::cout << "frequency-table Generated";
	}
	else {
		std::cout << "frequency-table faild:can't open file\n";
	}

	std::cout << "->Creating MinHeap with size of:" << 128 << "\n";
	// Create a min heap for Huffman tree
	MinHeap* heap = new MinHeap(128);
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

	writeTreeToJsonFile(root, dirPath + "/" + file_without_extension + "_tree.json");

	// Compress the file
	compress* compressor = new compress();
	bool validPath = true;
	std::cout << "->compressing using HuffmanTree\n";
	compressor->createMaps(root);
	string codedTextPrev = compressor->compressing(filePath, dirPath + "/" + file_without_extension + "_compressed.bin", &validPath);


	// Save compressed data and map
	saveMapToFile(dirPath + "/" + file_without_extension + "_decoder.json", compressor->decoder);

	std::cout << "Compressed and saved 2 files\n";

	// Cleanup
	if (encoderPtr) *encoderPtr = compressor->encoder;
	if (rootPtr) *rootPtr = *root;
	if (textPrevPtr) *textPrevPtr = codedTextPrev;

	delete root;


	return 0;
}