#include "StartCompressing.h"

int startCompressing(string filePath, string dirPath, unordered_map<string, string>* encoderPtr, Node** rootPtr, string* textPrevPtr) {
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
	Compressor compressor = Compressor();
	bool validPath = true;
	std::cout << "->compressing using HuffmanTree\n";
	compressor.createMaps(root);

	// thread based compression
	ifstream file(filePath, std::ios::ate);
	if (!file.is_open()) {
		// handle file not there
		return 0;
	}

	streamoff fileSize = file.tellg();
	streamoff threadNum = thread::hardware_concurrency();
	streamoff partSize = fileSize / threadNum;
	file.close();

	ofstream outputFile(dirPath + "/" + file_without_extension + "_compressed.bin");

	if (false) { // (fileSize < 10000) { // TODO put max file size before starting to use threads
		// don't perform threads
		outputFile << compressor.compressing(filePath, 0, fileSize);
	}
	else if (partSize > 99999999999999) { // TODO put max part size value instead
		// perform threads with files
	}
	else {
		// thread without files
		vector<future<string>> futures;
		for (int i = 0; i < threadNum; i++) {
			streamoff start = i * partSize;
			streamoff end = i == (threadNum - 1) ? fileSize : start + partSize;

			futures.push_back(std::async(std::launch::async, &Compressor::compressing, compressor,
				filePath, start, end 
			));
		}

		for (auto& future : futures)
			outputFile << future.get();
	}



	// Save compressed data and map
	saveMapToFile(dirPath + "/" + file_without_extension + "_decoder.json", compressor.decoder);

	std::cout << "Compressed and saved 2 files\n";

	// Extra Info
	if (encoderPtr) *encoderPtr = compressor.encoder;
	if (rootPtr) *rootPtr = root;
	//if (textPrevPtr) *textPrevPtr = codedTextPrev;


	return 0;
}