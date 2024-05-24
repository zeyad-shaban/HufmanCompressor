#include "Compressor.h"
#include <Windows.h>
#include "utils.h"
#include <thread>
#include <stdlib.h>
#include "Node.h"

void generateAsciiTable(Node* root, std::string* charsTable, std::string code = "") {
	if (!root->left && !root->right) {
		charsTable[root->letter] = code == "" ? "0" : code;
		return;
	}

	generateAsciiTable(root->left, charsTable, code + '0');
	generateAsciiTable(root->right, charsTable, code + '1');
}

void Compressor::compressSegment(char* inputData, char* segmentBuffer, unsigned int segmentSize, long long start, long long end, std::string charsTable[]) {
	unsigned int bufferIdx = -1;
	unsigned int allocatedSize = segmentSize;

	for (long long i = start; i < end; i++) {
		for (char bit : charsTable[(unsigned char)inputData[i]]) {
			if (bufferIdx + 1 >= allocatedSize){
				allocatedSize *= 2;
				realloc(segmentBuffer, allocatedSize);
			}
			segmentBuffer[++bufferIdx] = bit;
		}
	}
	segmentBuffer[++bufferIdx] = '\0';
}


long long Compressor::compressing(Node* root, std::string filePath, std::string outPath, float* progress) {
	HANDLE inputFile = CreateFileA(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (inputFile == INVALID_HANDLE_VALUE)
		return -1;

	HANDLE inputFileMap = CreateFileMappingA(inputFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (!inputFileMap) {
		CloseHandle(inputFile);
		return -1;
	}

	char* inputData = (char*)MapViewOfFile(inputFileMap, FILE_MAP_READ, 0, 0, 0);
	if (!inputData) {
		CloseHandle(inputFileMap);
		CloseHandle(inputFile);
		return -1;
	}

	LARGE_INTEGER inFileSize;
	if (!GetFileSizeEx(inputFile, &inFileSize)) {
		UnmapViewOfFile(inputData);
		CloseHandle(inputFileMap);
		CloseHandle(inputFile);
		return -1;
	}

	HANDLE outFile = CreateFileA(outPath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (outFile == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to open output file\n";
		UnmapViewOfFile(inputData);
		CloseHandle(inputFileMap);
		CloseHandle(inputFile);
		return -1;
	}

	HANDLE outFileMap = CreateFileMappingA(outFile, NULL, PAGE_READWRITE, 0, inFileSize.QuadPart, NULL);
	if (!outFileMap) {
		std::cout << "Failed to create file mapping for output file\n";
		UnmapViewOfFile(inputData);
		CloseHandle(inputFileMap);
		CloseHandle(inputFile);
		CloseHandle(outFile);
		return -1;
	}

	char* outData = (char*)MapViewOfFile(outFileMap, FILE_MAP_WRITE, 0, 0, inFileSize.QuadPart);
	if (!outData) {
		std::cout << "Failed to map view of output file\n";
		CloseHandle(outFileMap);
		CloseHandle(outFile);
		UnmapViewOfFile(inputData);
		CloseHandle(inputFileMap);
		CloseHandle(inputFile);
		return -1;
	}

	std::string charsTable[256];
	generateAsciiTable(root, charsTable);

	unsigned long long outIndex = 0;
	double dblFileSize = (double)inFileSize.QuadPart;
	int numThreads = std::thread::hardware_concurrency();
	long long chunkSize = 1024 * 1024;
	long long totalChunks = (inFileSize.QuadPart + chunkSize - 1) / chunkSize;

	std::vector<std::thread> threads;
	std::vector<char*> buffers(numThreads);
	bool didAllocateBuffers = false;

	unsigned char currentByte = 0;
	int bitCount = 0;


	for (long long chunkIndex = 0; chunkIndex < totalChunks; ++chunkIndex) {
		long long chunkStart = chunkIndex * chunkSize;
		long long chunkEnd = chunkStart + chunkSize < inFileSize.QuadPart ? chunkStart + chunkSize : inFileSize.QuadPart;

		unsigned int segmentSize = (chunkEnd - chunkStart + numThreads - 1) / numThreads;

		for (int i = 0; i < numThreads && !didAllocateBuffers; ++i)
			buffers[i] = (char*) malloc(segmentSize * 3);
		didAllocateBuffers = true; 

		for (int i = 0; i < numThreads; ++i) {
			long long start = chunkStart + i * segmentSize;
			long long end = chunkStart + (i + 1) * segmentSize < chunkEnd ? chunkStart + (i + 1) * segmentSize : chunkEnd;
			threads.emplace_back(&Compressor::compressSegment, this, inputData, buffers[i], segmentSize * 5, start, end, charsTable);
		}

		for (auto& thread : threads)
			thread.join();

		outIndex = chunkStart;

		for (char* buffer : buffers) {
			for (int i = 0; buffer[i] != '\0'; ++i) {
				currentByte = (currentByte << 1) | (buffer[i] - '0');
				bitCount++;
				if (bitCount == 8) {
					if (outIndex < inFileSize.QuadPart) {
						outData[outIndex++] = currentByte;
						*progress = outIndex / dblFileSize;
					}
					currentByte = 0;
					bitCount = 0;
				}
			}
		}
	}

	for (char* buffer : buffers)
		delete[] buffer;

	if (bitCount > 0) {
		currentByte <<= (8 - bitCount);
		outData[outIndex++] = currentByte;
		outData[outIndex++] = bitCount;
	}
	else
		outData[outIndex++] = 8;

	UnmapViewOfFile(inputData);
	CloseHandle(inputFileMap);
	CloseHandle(inputFile);

	UnmapViewOfFile(outData);
	CloseHandle(outFileMap);

	SetFilePointer(outFile, outIndex, NULL, FILE_BEGIN);
	SetEndOfFile(outFile);
	CloseHandle(outFile);

	return inFileSize.QuadPart;
}


bool Compressor::decompressing(Node* root, std::string compressedFilePath, std::string outputFilePath, int prevSize) {
	HANDLE compressedFile = CreateFileA(compressedFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (compressedFile == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to open compressed file\n";
		return false;
	}

	HANDLE compressedFileMap = CreateFileMappingA(compressedFile, NULL, PAGE_READONLY, 0, 0, NULL);
	if (!compressedFileMap) {
		std::cout << "Failed to create file mapping for compressed file\n";
		CloseHandle(compressedFile);
		return false;
	}

	char* compressedData = (char*)MapViewOfFile(compressedFileMap, FILE_MAP_READ, 0, 0, 0);
	if (!compressedData) {
		std::cout << "Failed to map view of compressed file\n";
		CloseHandle(compressedFileMap);
		CloseHandle(compressedFile);
		return false;
	}

	LARGE_INTEGER compressedFileSize;
	if (!GetFileSizeEx(compressedFile, &compressedFileSize)) {
		std::cout << "Failed to get file size\n";
		UnmapViewOfFile(compressedData);
		CloseHandle(compressedFileMap);
		CloseHandle(compressedFile);
		return false;
	}

	LONGLONG allocatedFileSize = compressedFileSize.QuadPart;

	HANDLE outFile = CreateFileA(outputFilePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (outFile == INVALID_HANDLE_VALUE) {
		std::cout << "Failed to open output file\n";
		UnmapViewOfFile(compressedData);
		CloseHandle(compressedFileMap);
		CloseHandle(compressedFile);
		return false;
	}

	HANDLE outFileMap = CreateFileMappingA(outFile, NULL, PAGE_READWRITE, 0, allocatedFileSize, NULL);
	if (!outFileMap) {
		std::cout << "Failed to create file mapping for output file\n";
		CloseHandle(outFile);
		UnmapViewOfFile(compressedData);
		CloseHandle(compressedFileMap);
		CloseHandle(compressedFile);
		return false;
	}

	char* outData = (char*)MapViewOfFile(outFileMap, FILE_MAP_WRITE, 0, 0, allocatedFileSize);
	if (!outData) {
		std::cout << "Failed to map view of output file\n";
		CloseHandle(outFileMap);
		CloseHandle(outFile);
		UnmapViewOfFile(compressedData);
		CloseHandle(compressedFileMap);
		CloseHandle(compressedFile);
		return false;
	}

	LONGLONG compressedIndex = 0;
	unsigned long long outIndex = -1;

	time_t start, end;
	time(&start);

	Node* nodeIt = root;
	bool rootOnly = (!nodeIt->left && !nodeIt->right);

	for (compressedIndex = 0; compressedIndex < compressedFileSize.QuadPart - 2; ++compressedIndex) {
		for (int j = 7; j >= 0; --j) {
			if (!rootOnly) nodeIt = (compressedData[compressedIndex] >> j) & 1 ? nodeIt->right : nodeIt->left;

			if (!nodeIt->left && !nodeIt->right) {
				if (outIndex + 1 >= allocatedFileSize) {
					UnmapViewOfFile(outData);
					CloseHandle(outFileMap);

					allocatedFileSize *= 2;
					outFileMap = CreateFileMappingA(outFile, NULL, PAGE_READWRITE, 0, allocatedFileSize, NULL);
					if (!outFileMap) {
						std::cout << "Failed to create file mapping for expanded output file\n";
						UnmapViewOfFile(compressedData);
						CloseHandle(compressedFileMap);
						CloseHandle(compressedFile);
						CloseHandle(outFile);
						return false;
					}
					outData = (char*)MapViewOfFile(outFileMap, FILE_MAP_WRITE, 0, 0, allocatedFileSize);
					if (!outData) {
						std::cout << "Failed to map view of expanded output file\n";
						CloseHandle(outFileMap);
						CloseHandle(outFile);
						UnmapViewOfFile(compressedData);
						CloseHandle(compressedFileMap);
						CloseHandle(compressedFile);
						return false;
					}
				}

				outData[++outIndex] = nodeIt->letter;
				nodeIt = root;
			}
		}
	}

	unsigned char validBits = compressedData[compressedIndex + 1];
	for (int j = 7; j >= 8 - validBits; j--) {
		if (!rootOnly) nodeIt = (compressedData[compressedIndex] >> j) & 1 ? nodeIt->right : nodeIt->left;
		if (!nodeIt->left && !nodeIt->right) {
			outData[++outIndex] = nodeIt->letter;
			nodeIt = root;
		}
	}

	time(&end);
	std::cout << "TIME TAKEN TO DECOMPRESS: " << end - start << " seconds\n";

	UnmapViewOfFile(compressedData);
	CloseHandle(compressedFileMap);
	CloseHandle(compressedFile);

	UnmapViewOfFile(outData);
	CloseHandle(outFileMap);

	SetFilePointer(outFile, outIndex + 1, NULL, FILE_BEGIN);
	SetEndOfFile(outFile);
	CloseHandle(outFile);

	return true;
}