#include "Compressor.h"
#include <Windows.h>

void closeMMap() {
	// todo to fircken make the memory map look nicer
}

void generateAsciiTable(Node* root, string* charsTable, string code = "") {
	if (!root->left && !root->right) {
		charsTable[root->letter] = code == "" ? "0" : code;
		return;
	}

	generateAsciiTable(root->left, charsTable, code + '0');
	generateAsciiTable(root->right, charsTable, code + '1');
}


bool Compressor::compressing(Node* root, string filePath, string outPath) {
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

	string charsTable[256];
	generateAsciiTable(root, charsTable);

	unsigned char bitBuffer = 0;
	int currBit = 0;

	for (LONGLONG i = 0; i < inFileSize.QuadPart; i++) {
		for (char bit : charsTable[(unsigned char)inputData[i]]) {
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
	else
		outData[outIndex++] = 8;


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


bool Compressor::decompressing(Node* root, string compressedFilePath, string outputFilePath, int prevSize) {
	HANDLE compressedFile = CreateFileA(compressedFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE compressedFileMap = CreateFileMappingA(compressedFile, NULL, PAGE_READONLY, 0, 0, NULL);
	char* compressedData = (char*)MapViewOfFile(compressedFileMap, FILE_MAP_READ, 0, 0, 0);

	LARGE_INTEGER compressedFileSize;
	if (!GetFileSizeEx(compressedFile, &compressedFileSize)) {
		std::cout << "Failed to get file size\n";
		UnmapViewOfFile(compressedData);
		CloseHandle(compressedFile);
		CloseHandle(compressedFileMap);
		return false;
	}

	LONGLONG allocatedFileSize = compressedFileSize.QuadPart;


	HANDLE outFile = CreateFileA(outputFilePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	HANDLE outFileMap = CreateFileMappingA(outFile, NULL, PAGE_READWRITE, 0, allocatedFileSize, NULL);
	if (!outFileMap) {
		DWORD dwError = GetLastError();
		std::cout << "Failed to create file mapping. Error: " << dwError << "\n";
		return false;
	}
	char* outData = (char*)MapViewOfFile(outFileMap, FILE_MAP_WRITE, 0, 0, allocatedFileSize);
	LONGLONG compressedIndex = 0;
	unsigned long long outIndex = -1;

	// TODO HANDLE OPENING FILE ERRS

	time_t start, end;
	time(&start);

	Node* nodeIt = root;
	bool rootOnly = false;
	if (!nodeIt->left && !nodeIt->right) rootOnly = true;

	for (compressedIndex = 0; compressedIndex < compressedFileSize.QuadPart - 2; ++compressedIndex) {
		for (int j = 7; j >= 0; --j) {
			if (!rootOnly) nodeIt = (compressedData[compressedIndex] >> j) & 1 ? nodeIt->right : nodeIt->left;

			if (nodeIt->letter) {
				if (outIndex + 1 >= allocatedFileSize) {
					CloseHandle(outFileMap);
					UnmapViewOfFile(outData);

					allocatedFileSize *= 2;
					outFileMap = CreateFileMappingA(outFile, NULL, PAGE_READWRITE, 0, allocatedFileSize, NULL);
					outData = (char*)MapViewOfFile(outFileMap, FILE_MAP_WRITE, 0, 0, allocatedFileSize);
				}

				outData[++outIndex] = nodeIt->letter;
				nodeIt = root;
			}
		}
	}

	// compressed index now standing at the before last bit
	char validBits = compressedData[compressedIndex + 1];

	for (int j = 7; j >= 8 - validBits; j--) {
		if (!rootOnly) nodeIt = (compressedData[compressedIndex] >> j) & 1 ? nodeIt->right : nodeIt->left;
		if (nodeIt->letter) {
			outData[++outIndex] = nodeIt->letter;
			nodeIt = root;
		}
	}

	time(&end);
	std::cout << "TIME TAKEN TO DECOMPRESS: " << end - start << "\n";


	UnmapViewOfFile(compressedData);
	CloseHandle(compressedFile);
	CloseHandle(compressedFileMap);


	UnmapViewOfFile(outData);
	CloseHandle(outFileMap);
	SetFilePointer(outFile, outIndex + 1, NULL, FILE_BEGIN);
	SetEndOfFile(outFile);
	CloseHandle(outFile);

	return true;
}