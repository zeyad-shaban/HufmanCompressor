#include "StartDecompressing.h"


bool StartDecompressing(string compressedFilePath, string decoderPath, string dirPath, unordered_map<string, string>* decoderPtr, string* textPrevPtr) {
	std::string base_filename = compressedFilePath.substr(compressedFilePath.find_last_of("/\\") + 1);
	std::string::size_type const p(base_filename.find_last_of('.'));
	std::string file_without_extension = base_filename.substr(0, p);

	Compressor* compressor = new Compressor();
	FILE* decoderFile; bool errDecoder = fopen_s(&decoderFile, decoderPath.c_str(), "r");

	if (errDecoder) {
		// TODO HANDLE INVALID DECODER PATH
		return false;
	}

	nlohmann::json jsonDecoder = nlohmann::json::parse(decoderFile);
	for (auto& element : jsonDecoder.items())
		compressor->decoder[element.key()] = element.value();


	bool success = compressor->decompressing(compressedFilePath, dirPath + "/" + file_without_extension + "_decompressed.txt");

	if (decoderPtr) *decoderPtr = compressor->decoder;

	return true;
}