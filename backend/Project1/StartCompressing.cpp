#include "StartCompressing.h"
#include <chrono>
#include <thread>
#include <string>

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
	string path1 = "./serverData/frequency_table.txt";
	FILE* file2;  fopen_s(&file2, path1.c_str(), "wb");
	for (int i = 0; i < 128; ++i) {
		if (freqTable[i] != 0) {
			char ch = char(i);
			fwrite(&ch, sizeof(char), 1, file2); // Write the character
			fwrite(":", sizeof(char), 1, file2); // Write the colon
			fwrite(" ", sizeof(char), 1, file2); //  Write the space
		}
	}
	fclose(file2);


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
	int get_file_size_and_split(const std::string & filePath, size_t * fileSize, size_t * threadNum, size_t * partSize){
		FILE* file = fopen(filePath.c_str(), "rb"); 
		if (file == NULL) {
			fprintf(stderr, "Error opening file %s\n", filePath.c_str());
			return 0; // Indicate error
		}

		// Get file size using fseek and ftell
		if (fseek(file, 0, SEEK_END) != 0) {
			fprintf(stderr, "Error seeking in file %s\n", filePath.c_str());
			fclose(file);
			return 0; // Indicate error
		}
		*fileSize = ftell(file);
		*threadNum = std::thread::hardware_concurrency();
		*partSize = *fileSize / *threadNum;
		fclose(file);
		return 1; // Indicate success
	}


	string outPath = dirPath + "/" + file_without_extension + "_compressed.bin";
	string codedTextPrev = compressor.compressing(filePath, outPath);


	

	saveMapToFile(dirPath + "/" + file_without_extension + "_decoder.json", compressor.decoder);

	std::cout << "Compressed and saved 2 files\n";

	// Extra Info
	if (encoderPtr) *encoderPtr = compressor.encoder;
	if (rootPtr) *rootPtr = root;
	if (textPrevPtr) *textPrevPtr = codedTextPrev;


	return 0;
}