#include "StartDecompressing.h"


bool StartDecompressing(string compressedFilePath, string treePath, string dirPath, unordered_map<string, string>* decoderPtr, string* textPrevPtr) {
	std::string base_filename = compressedFilePath.substr(compressedFilePath.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);

	Compressor* compressor = new Compressor();

	Node* root = readTreeFromJsonFile(treePath);

	bool success = compressor->decompressing(root, compressedFilePath, dirPath + "/" + file_without_extension + "_decompressed.txt");

	//if (decoderPtr) *decoderPtr = compressor->decoder;

	return true;
}