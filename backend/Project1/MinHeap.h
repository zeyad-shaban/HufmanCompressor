#pragma once

#include <iostream>
#include "Node.h"

class MinHeap {
public:
    int heapPtr = 0;
    int size;
    Node* arr;

    MinHeap(int size);

    int getParent(int i);
    int getLeft(int i);
    int getRight(int i);

    void insertValues(char letter, int freq, bool* overflow = nullptr);
    void orderInsertedNode();
    void bubbleDown(int index);
    Node* extractMin();
    std::string traverse(int i);

    ~MinHeap();
};
