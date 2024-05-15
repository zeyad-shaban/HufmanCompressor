#pragma once
#include "Node.h"
#include "MinHeap.h"
#include <stdio.h>
#include <fstream>
#include <unordered_map>
#include <iostream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;


Node* tregen(MinHeap* heap);
bool genFreqTable(std::string filePath, int* freqTable);
bool saveStringToFile(std::string path, std::string str);
bool saveMapToFile(std::string filePath, std::unordered_map<std::string, std::string> map);
void writeTreeArrToJsonFile(Node* treeArr[], int treeSize, const std::string& filename);
Node** readTreeArrFromJsonFile(const std::string& filename, int* size);
