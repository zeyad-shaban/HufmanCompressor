#include "StartDecompressing.h"


bool StartDecompressing(string compressedFilePath, string decoderPath, string dirPath, unordered_map<string, string>* decoderPtr, string* textPrevPtr) {
	std::string base_filename = compressedFilePath.substr(compressedFilePath.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);

	Compressor* compressor = new Compressor();
	ifstream decoderFile(decoderPath);

	if (!decoderFile.is_open()) {
		// TODO HANDLE INVALID DECODER PATH
	}

	nlohmann::json jsonDecoder = nlohmann::json::parse(decoderFile);
	for (auto& element : jsonDecoder.items())
		compressor->decoder[element.key()] = element.value();


	string decodedTextPrev = compressor->decompressing(compressedFilePath, dirPath + "/" + file_without_extension + "_decompressed.txt");

	if (decoderPtr) *decoderPtr = compressor->decoder;
	//if (textPrevPtr) *textPrevPtr = codedTextPrev;

	return true;
}