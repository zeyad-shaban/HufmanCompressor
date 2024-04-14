#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#pragma once
class serverData {
public:
	std::string compressedText;
	std::string decoderMap;

	serverData() {
		std::ifstream file("compressed_file.txt");
		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			compressedText = buffer.str();
		}
		std::ifstream file("decoder_map.json");

		if (file.is_open()) {
			std::stringstream buffer;
			buffer << file.rdbuf();
			decoderMap = buffer.str();
		}


	}

};