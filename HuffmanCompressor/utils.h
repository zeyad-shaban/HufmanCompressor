#pragma once
#include "Node.h"
#include "MinHeap.h"
#include <stdio.h>
#include <fstream>


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

bool saveStringToFile(const char* filename, const char* str) {
	FILE* file = fopen(filename, "w");
	if (!file) return false;

	fputs(str, file);
	fclose(file);

	return true;
}