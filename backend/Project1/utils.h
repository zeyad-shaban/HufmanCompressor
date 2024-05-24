#pragma once

 #include "Node.h"
 #include "MinHeap.h"


Node* tregen(MinHeap* heap);
bool genFreqTable(std::string filePath, int* freqTable);
bool saveStringToFile(std::string path, std::string str);
void writeTreeArrToJsonFile(Node* treeArr[], int treeSize, const std::string& filename);
Node** readTreeArrFromJsonFile(const std::string& filename, int* size);
void runExecutable(const char* filePath);
void threadFileGenerator(bool* done, int* state, float* progress, float generatedVal, char* filename);