#include "StartCompressing.h"
#include <chrono>
#include <thread>
#include <string>

int startCompressing(string filePath, string dirPath, unordered_map<string, string>* encoderPtr, Node** rootPtr, string* textPrevPtr) {
	std::string base_filename = filePath.substr(filePath.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);


	// Generate frequency table
	int freqTable[256] = { 0 };
	std::cout << "->Generating frequency-table...\n";
	time_t start, end;
	time(&start);
	if (genFreqTable(filePath, freqTable)) {
		time(&end);
		std::cout << "FreqTable Generated In: " << end - start << "sec" << endl;
	}
	else
		std::cout << "FreqTable faild:can't open file\n";


	// Create min heap for Huffman tree
	MinHeap* heap = new MinHeap(256);
	bool overflow = false;

	for (int i = 0; i < 256; i++)
		if (freqTable[i])
			heap->insertValues(i, freqTable[i]);

	// Generate Huffman tree
	Node* root = tregen(heap);
	delete heap;

	writeTreeToJsonFile(root, dirPath + "/" + file_without_extension + "_tree.json");

	// Compress the file
	Compressor compressor = Compressor(); // FUCK THIS WHY IS IT EVEN IN A CLASS JUST PUT IT IN A FUCKEN VOID FUNCTION LIEK ALL FUCKEN NORMAL PEOPLE DO
	bool validPath = true;
	std::cout << "->compressing using HuffmanTree...\n";

	string outPath = dirPath + "/" + file_without_extension + "_compressed.bin";

	time(&start);
	compressor.compressing(root, filePath, outPath);
	time(&end);
	cout << "Done copmressing in " << end - start << "sec" << endl;

	//saveMapToFile(dirPath + "/" + file_without_extension + "_decoder.json", compressor.decoder);

	// Extra Info
	//if (encoderPtr) *encoderPtr = compressor.decoder;
	//if (rootPtr) *rootPtr = root;

	return 0;
}