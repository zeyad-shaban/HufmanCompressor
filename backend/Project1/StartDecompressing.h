#pragma once
#include "Compressor.h"

bool StartDecompressing(string compressedFilePath, string decoderPath, string dirPath, unordered_map<string, string>* decoderPtr= nullptr, string* textPrevPtr = nullptr);