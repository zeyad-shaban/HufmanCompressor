#include "Compressor.h"
#include <Windows.h>

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

bool Compressor::compressing(string filePath, string outPath) {
	HANDLE inputFile = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE inputFileMap = CreateFileMappingA(inputFile, NULL, PAGE_READONLY, 0, 0, NULL);
	char* inputData = (char*)MapViewOfFile(inputFileMap, FILE_MAP_READ, 0, 0, 0);

	LARGE_INTEGER inFileSize;
	if (!GetFileSizeEx(inputFile, &inFileSize)) {
		std::cout << "Failed to get file size\n";
		UnmapViewOfFile(inputData);
		CloseHandle(inputFile);
		CloseHandle(inputFileMap);
		return false;
	}

	HANDLE outFile = CreateFileA(outPath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE outFileMap = CreateFileMappingA(outFile, NULL, PAGE_READWRITE, 0, inFileSize.QuadPart, NULL);
	if (!outFileMap) {
		DWORD dwError = GetLastError();
		std::cout << "Failed to create file mapping. Error: " << dwError << "\n";
		return false;
	}
	char* outData = (char*)MapViewOfFile(outFileMap, FILE_MAP_WRITE, 0, 0, inFileSize.QuadPart);
	unsigned long long outIndex = 0;

	// TODO HANDLE FAILING FOR EITHER ORIGNAL OR OUT FILE

	string charsTable[128];
	for (int i = 0; i < 128; i++)
		if (encoder.find(i) != encoder.end())
			charsTable[i] = encoder[i];

	unsigned char bitBuffer = 0;
	int currBit = 0;


	for (LONGLONG i = 0; i < inFileSize.QuadPart; i++) {
		for (char bit : charsTable[inputData[i]]) {
			bitBuffer = (bitBuffer << 1) | (bit - '0');
			currBit++;
			if (currBit >= 8) {
				outData[outIndex++] = bitBuffer;
				currBit = 0;
				bitBuffer = 0;
			}
		}

	}
	if (currBit > 0) {
		bitBuffer <<= (8 - currBit);
		outData[outIndex++] = bitBuffer;
		
		outData[outIndex++] = currBit;
	}


	UnmapViewOfFile(inputData);
	CloseHandle(inputFile);
	CloseHandle(inputFileMap);

	
	UnmapViewOfFile(outData);
	CloseHandle(outFileMap);
	SetFilePointer(outFile, outIndex, NULL, FILE_BEGIN);
	SetEndOfFile(outFile);
	CloseHandle(outFile);

	return true;
}


string Compressor::decompressing(string compressedFilePath, string outputFilePath, int prevSize) {
	// Open the files
	FILE* compressedFile = nullptr;
	bool errCompressed = fopen_s(&compressedFile, compressedFilePath.c_str(), "rb");
	FILE* outputFile = nullptr;
	bool errOut = fopen_s(&outputFile, outputFilePath.c_str(), "w");

	if (errCompressed || errOut) {
		if (compressedFile) fclose(compressedFile);
		if (outputFile) fclose(outputFile);
		return "Failed to open file";
	}

	// Get the size of the compressed file
	fseek(compressedFile, 0, SEEK_END);
	long fileSize = ftell(compressedFile);
	fseek(compressedFile, 0, SEEK_SET);

	// Read the number of valid bits in the last byte
	fseek(compressedFile, -1, SEEK_END);
	unsigned char validBits;
	fread(&validBits, 1, 1, compressedFile);
	fseek(compressedFile, 0, SEEK_SET);

	time_t start, end;
	time(&start);

	string code = "";
	unsigned char byte;
	for (long i = 0; i < fileSize - 2; i++) {  // Don't read the last two bytes
		fread(&byte, 1, 1, compressedFile);
		for (int j = 7; j >= 0; j--) {
			code += (byte >> j) & 1 ? '1' : '0';
			if (decoder.find(code) != decoder.end()) {
				fputc(decoder[code][0], outputFile);
				code = "";
			}
		}
	}
	//Read until the valid bits
	fread(&byte, 1, 1, compressedFile);
	for (int j = 7; j >= 8 - validBits; j--) { 
		code += (byte >> j) & 1 ? '1' : '0';
		if (decoder.find(code) != decoder.end()) {
			fputc(decoder[code][0], outputFile);
			code = "";
		}
	}

	time(&end);
	cout << "TIME TAKEN TO DECOMPRESS: " << end - start << endl;

	fclose(compressedFile);
	fclose(outputFile);

	return "Decompression completed successfully!";


}

void Compressor::printEncoder() {
	for (auto it = encoder.begin(); it != encoder.end(); it++) {
		cout << it->first << " " << it->second << endl;
	}
}
