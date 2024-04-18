#pragma once
#include "Node.h"
#include <sstream>
#include <string>

class MinHeap {
public:
    int heapPtr = 0;
    int size;
    Node* arr;

    MinHeap(int size);

    int getParent(int i);
    int getLeft(int i);
    int getRight(int i);

    void insertValues(std::string letters, int freq, bool* overflow = nullptr);
    void orderInsertedNode();
    void bubbleDown(int index);
    Node* extractMin();
    std::string traverse(int i);

    ~MinHeap();
};
