#pragma once
#include "Node.h"
#include "MinHeap.h"
#include <stdio.h>
#include <fstream>
#include <unordered_map>


Node* tregen(MinHeap* heap) {
	while (heap->heapPtr > 1) {
		Node* node1 = heap->extractMin();
		Node* node2 = heap->extractMin();

		int hPtr = heap->heapPtr;

		heap->arr[hPtr].letters = node1->letters + node2->letters;
		heap->arr[hPtr].freq = node1->freq + node2->freq;
		heap->arr[hPtr].left = node1;
		heap->arr[hPtr].right = node2;

		heap->orderInsertedNode();
	}
	return heap->extractMin();
}

bool genFreqTable(std::string filePath, int* freqTable, int* size) {
	std::ifstream file(filePath);
	if (!file.is_open()) return false;

	char ch;
	while (file.get(ch))
		if (ch >= 0 && ch < 128) {
			freqTable[ch]++;
			(*size)++;
		}

	return true;
}
bool saveStringToFile(std::string path, std::string str) {
	std::ofstream file(path);
	if (!file) return false;

	file << str;

	return true;
}

bool saveMapToFile(std::string filePath, std::unordered_map<std::string, std::string> map) {
	std::ofstream file(filePath);
	if (!file) return false;
	file << "{";

	for (auto& pair : map) {
		file << "\"" + pair.first + "\":\"";
		if (pair.second == "\n") file << "\\n";
		else file << pair.second;
		file << "\",";
	}

	file.seekp(-1, std::ios::end);

	file << "}";
	return true;
}

using json = nlohmann::json;

json nodeToJson(Node* node) {
	if (node == nullptr) {
		return nullptr;
	}

	json j;
	j["letters"] = node->letters;
	j["freq"] = node->freq;
	j["left"] = nodeToJson(node->left);
	j["right"] = nodeToJson(node->right);

	return j;
}
void writeTreeToJsonFile(Node* root, const std::string& filename) {
	json j = nodeToJson(root);
	std::ofstream file(filename);
	file << j.dump(4); 
	file.close();
}
