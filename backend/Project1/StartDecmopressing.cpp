#include "StartDecompressing.h"


bool StartDecompressing() {
	string compressedFilePath = "./data/input_compressed.bin";
	string dirPath = ".";
	string decoderPath = "decoder_map.json";

	string base_filename = compressedFilePath.substr(compressedFilePath.find_last_of("/\\") + 1);
	string::size_type const p(base_filename.find_last_of('.'));
	string file_without_extension = base_filename.substr(0, p);


	Compressor* compressor = new Compressor();
	ifstream decoderFile(decoderPath);

	if (!decoderFile.is_open()) {
		// TODO HANDLE INVALID DECODER PATH
	}

	nlohmann::json jsonDecoder = nlohmann::json::parse(decoderFile);
	for (auto& element : jsonDecoder.items())
		compressor->decoder[element.key()] = element.value();


	bool validPath = false;
	string decodedTextPrev = compressor->decompressing(compressedFilePath, "decompressed.txt", &validPath);

	if (!validPath) {
		// TODO HANDLE INVALID PATH HERE
	}

	Node* root = nullptr;


	return true;
}