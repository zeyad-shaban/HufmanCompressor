#include "Compressor.h"

Compressor::Compressor() {
	encoder = unordered_map<string, string>();
	decoder = unordered_map<string, string>();
}

void Compressor::createMaps(Node* root, string code) {
	if (root == NULL) {
		return;
	}
	if (root->left == NULL && root->right == NULL) {
		encoder[root->letters] = code;
		decoder[code] = root->letters;
		return;
	}
	createMaps(root->left, code + "0");
	createMaps(root->right, code + "1");
}

string Compressor::compressing(string filePath, string outPath) {
	FILE* file; fopen_s(&file, filePath.c_str(), "r");

	//FILE* outFile; fopen_s(&outFile, outPath.c_str(), "wb");
	ofstream outFile(outPath);

	string charsTable[128];
	for (int i = 0; i < 128; i++)
		if (encoder.find(std::to_string(i)) != encoder.end())
			charsTable[i] = encoder[std::to_string(i)];

	time_t time_start, time_end;
	time(&time_start);

	char inBuffer[1000000];
	string outBuffer = "";
	int bytesRead;
	while ((bytesRead = fread(inBuffer, sizeof(char), 1000000, file)) > 0) {
		for (int i = 0; i < bytesRead; i++)
			outBuffer += charsTable[inBuffer[i]];

		outFile << outBuffer;
	}

	time(&time_end);

	std::cout << "DONE WITH BUFFER IN " << time_end - time_start << std::endl;
	return "hi";
}

string Compressor::decompressing(string compressedFilePath, string outputFilePath, int prevSize) {
	ifstream compressedFile(compressedFilePath);
	ofstream outputFile(outputFilePath);

	if (!compressedFile.is_open() || !outputFile.is_open())
		return "Failed to open file";

	string decodedTextPrev = "";
	string code = "";
	string buffer = "";

	char ch;
	while (compressedFile.get(ch)) {
		code += ch;
		if (decoder.find(code) != decoder.end()) {
			if (decodedTextPrev.size() <= prevSize) decodedTextPrev += decoder[code];

			buffer += decoder[code];
			code = "";


		}
	}
	outputFile << buffer;

	if (decodedTextPrev.size() >= prevSize) decodedTextPrev += "...";

	return decodedTextPrev;
}

void Compressor::printEncoder() {
	for (auto it = encoder.begin(); it != encoder.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}
}
