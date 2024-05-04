#include "Compressor.h"

Compressor::Compressor() {
	encoder = unordered_map<char, string>();
	decoder = unordered_map<string, string>();
}

void Compressor::createMaps(Node* root, string code) {
	if (root == NULL) {
		return;
	}
	if (root->left == NULL && root->right == NULL) {
		encoder[root->letters[0]] = code;
		decoder[code] = root->letters;
		return;
	}
	createMaps(root->left, code + "0");
	createMaps(root->right, code + "1");
}

void Compressor::compressing(string filePath, string outPath) {
	FILE* file = nullptr;
	FILE* outFile = nullptr;

	if (fopen_s(&file, filePath.c_str(), "r") || fopen_s(&outFile, outPath.c_str(), "wb")) {
		if (file) fclose(file);
		if (outFile)fclose(outFile);
		return;
	}

	string charsTable[128];
	for (int i = 0; i < 128; i++)
		if (encoder.find(i) != encoder.end())
			charsTable[i] = encoder[i];

	char inBuffer[1000000]; // todo change this very large number to whatever the system needs
	int charsRead;

	int currBit = 0;
	unsigned char bitBuffer = 0;

	while ((charsRead = fread(inBuffer, 1, 1000000, file)) > 0) {
		for (int i = 0; i < charsRead; i++) {
			for (char bit : charsTable[inBuffer[i]]) {
				bitBuffer = (bitBuffer << 1) | (bit - '0');
				currBit++;
				if (currBit >= 8) {
					fwrite(&bitBuffer, 1, 1, outFile);
					currBit = 0;
					bitBuffer = 0;
				}
			}
		}
		if (bitBuffer) fwrite(&bitBuffer, 1, 1, outFile);
	}
	fclose(file);
	fclose(outFile);
	return;
}

string Compressor::decompressing(string compressedFilePath, string outputFilePath, int prevSize) {
	FILE* compressedFile = nullptr;
	bool errCompressed = fopen_s(&compressedFile, compressedFilePath.c_str(), "rb");
	FILE* outputFile = nullptr;
	bool errOut = fopen_s(&outputFile, outputFilePath.c_str(), "w");

	if (errCompressed || errOut) {
		if (compressedFile) fclose(compressedFile);
		if (outputFile) fclose(outputFile);
		return "Failed to open file";
	}

	time_t start, end;

	time(&start);
	string code = "";
	unsigned char byte;
	while (fread(&byte, 1, 1, compressedFile)) {
		for (int i = 7; i >= 0; i--) {
			code += (byte >> i) & 1 ? '1' : '0';
			if (decoder.find(code) != decoder.end()) {
				fputc(decoder[code][0], outputFile);
				code = "";
			}
		}
	}
	time(&end);

	cout << "TIME TAKEN TO DECOMPRESS: " << end - start << endl;
	fclose(compressedFile);
	fclose(outputFile);

	return "hi agian lol";
}

void Compressor::printEncoder() {
	for (auto it = encoder.begin(); it != encoder.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}
}
