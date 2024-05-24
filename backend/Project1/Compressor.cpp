#include "Compressor.h"
#include <Windows.h>
#include "utils.h"
#include "Node.h"

void closeMMap() {
	// todo to fircken make the memory map look nicer
}

void generateAsciiTable(Node* root, std::string* charsTable, std::string code = "") {
	if (!root->left && !root->right) {
		charsTable[root->letter] = code == "" ? "0" : code;
		return;
	}

	generateAsciiTable(root->left, charsTable, code + '0');
	generateAsciiTable(root->right, charsTable, code + '1');
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

    unsigned long long outIndex = 0;
    std::string charsTable[256];
    generateAsciiTable(root, charsTable);

    unsigned char bitBuffer = 0;
    int currBit = 0;
    double dblFileSize = (double)inFileSize.QuadPart;

    for (long long i = 0; i < dblFileSize; i++) {
        *progress = (i / dblFileSize);
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
    else {
        outData[outIndex++] = 8;
    }

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
bool Compressor::decompressing(Node* root, std::string compressedFilePath, std::string outputFilePath, float* progress) {
    HANDLE compressedFile = CreateFileA(compressedFilePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (compressedFile == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open compressed file\n";
        return false;
    }

    LARGE_INTEGER compressedFileSize;
    if (!GetFileSizeEx(compressedFile, &compressedFileSize)) {
        std::cout << "Failed to get file size\n";
        CloseHandle(compressedFile);
        return false;
    }

    HANDLE compressedFileMap = CreateFileMappingA(compressedFile, NULL, PAGE_READONLY, compressedFileSize.HighPart, compressedFileSize.LowPart, NULL);
    if (!compressedFileMap) {
        std::cout << "Failed to create file mapping for compressed file\n";
        CloseHandle(compressedFile);
        return false;
    }

    char* compressedData = (char*)MapViewOfFile(compressedFileMap, FILE_MAP_READ, 0, 0, compressedFileSize.QuadPart);
    if (!compressedData) {
        std::cout << "Failed to map view of compressed file\n";
        CloseHandle(compressedFileMap);
        CloseHandle(compressedFile);
        return false;
    }

    HANDLE outFile = CreateFileA(outputFilePath.c_str(), GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
    if (outFile == INVALID_HANDLE_VALUE) {
        std::cout << "Failed to open output file\n";
        UnmapViewOfFile(compressedData);
        CloseHandle(compressedFileMap);
        CloseHandle(compressedFile);
        return false;
    }

    LONGLONG allocatedFileSize = 1024 * 1024 * 1024;  // Start with 1GB
    HANDLE outFileMap = CreateFileMappingA(outFile, NULL, PAGE_READWRITE, (DWORD)(allocatedFileSize >> 32), (DWORD)allocatedFileSize, NULL);
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
    LONGLONG outIndex = -1;

    time_t start, end;
    time(&start);

    Node* nodeIt = root;
    bool rootOnly = (!nodeIt->left && !nodeIt->right);
    double compressedSize = (double)compressedFileSize.QuadPart;
    for (compressedIndex = 0; compressedIndex < compressedFileSize.QuadPart - 2; ++compressedIndex) {
        *progress = (float)(compressedIndex / compressedSize);

        for (int j = 7; j >= 0; --j) {
            if (!rootOnly) nodeIt = (compressedData[compressedIndex] >> j) & 1 ? nodeIt->right : nodeIt->left;

            if (!nodeIt->left && !nodeIt->right) {
                if (outIndex + 1 >= allocatedFileSize) {
                    UnmapViewOfFile(outData);
                    CloseHandle(outFileMap);

                    allocatedFileSize *= 2;
                    outFileMap = CreateFileMappingA(outFile, NULL, PAGE_READWRITE, (DWORD)(allocatedFileSize >> 32), (DWORD)allocatedFileSize, NULL);
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

    LARGE_INTEGER outIndexPlusOne;
    outIndexPlusOne.QuadPart = outIndex + 1;
    SetFilePointerEx(outFile, outIndexPlusOne, NULL, FILE_BEGIN);
    SetEndOfFile(outFile);
    CloseHandle(outFile);

    return true;
}
