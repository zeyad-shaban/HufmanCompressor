#include "StartDecompressing.h"
#include <cstdio> // Include for fopen, fwrite
#include "json.hpp" // Assuming this header is included for nlohmann::json

bool StartDecompressing(string compressedFilePath, string decoderPath, string dirPath, unordered_map<string, string>* decoderPtr, string* textPrevPtr) {
    std::string base_filename = compressedFilePath.substr(compressedFilePath.find_last_of("/\\") + 1);
    std::string::size_type const p(base_filename.find_last_of('.'));
    std::string file_without_extension = base_filename.substr(0, p);

    Compressor* compressor = new Compressor();
    FILE* decoderFile = fopen(decoderPath.c_str(), "r");

    if (!decoderFile) {
        // TODO HANDLE INVALID DECODER PATH
    }

    // Read JSON content from the file
    fseek(decoderFile, 0, SEEK_END);
    long fileSize = ftell(decoderFile);
    rewind(decoderFile);
    char* buffer = new char[fileSize + 1];
    size_t bytesRead = fread(buffer, 1, fileSize, decoderFile);
    buffer[bytesRead] = '\0'; // Null-terminate the string
    fclose(decoderFile);

    // Parse JSON content
    nlohmann::json jsonDecoder = nlohmann::json::parse(buffer);
    delete[] buffer;

    // Populate compressor decoder from JSON
    for (auto& element : jsonDecoder.items())
        compressor->decoder[element.key()] = element.value();

    string decodedTextPrev = compressor->decompressing(compressedFilePath, dirPath + "/" + file_without_extension + "_decompressed.txt");

    if (decoderPtr) *decoderPtr = compressor->decoder;
    //if (textPrevPtr) *textPrevPtr = codedTextPrev;

    return true;
}
