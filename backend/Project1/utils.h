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
json nodeToJson(Node* node);
void writeTreeToJsonFile(Node* root, const std::string& filename);
