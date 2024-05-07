#pragma once
#include "Compressor.h"
#include "utils.h"

bool StartDecompressing(string compressedFilePath, string treePath, string dirPath, unordered_map<string, string>* decoderPtr= nullptr, string* textPrevPtr = nullptr);