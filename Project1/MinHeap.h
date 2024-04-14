#pragma once
#include "Node.h"
#include <sstream>
#include <string>

class MinHeap {
public:
    int heapPtr = 0;
    int size;
    Node* arr;

    MinHeap(int size) {
        this->size = size;
        arr = new Node[size];
    }

    int getParent(int i) { return (i - 1) / 2; }
    int getLeft(int i) { return 2 * i + 1; }
    int getRight(int i) { return 2 * i + 2; }

    void insertValues(std::string letters, int freq, bool* overflow = nullptr) {
        if (heapPtr >= size) {
            if (overflow) *overflow = true;
            return;
        }
        arr[heapPtr].update(letters, freq);

        int i = heapPtr;

        while (true) {
            int parentI = getParent(i);
            if (i == 0 || arr[i].freq > arr[parentI].freq) break;

            Node tmpCurr = arr[i];
            arr[i] = arr[parentI];
            arr[parentI] = tmpCurr;

            i = parentI;
        }

        heapPtr++;
    }

    void orderInsertedNode() {
        int i = heapPtr;

        while (true) {
            int parentI = getParent(i);
            if (i == 0 || arr[i].freq > arr[parentI].freq) break;

            Node tmpCurr = arr[i];
            arr[i] = arr[parentI];
            arr[parentI] = tmpCurr;

            i = parentI;
        }

        heapPtr++;
    }

    void bubbleDown(int index) {
        int i = index;
        while (true) {
            int left = this->getLeft(i);
            int right = this->getRight(i);

            int smaller;

            if ((right >= size || arr[right].freq == 0) && (left >= size || arr[left].freq == 0)) break;

            if (arr[right].freq == 0 || right >= size)
                smaller = left;
            else if (arr[left].freq == 0 || left >= size)
                smaller = right;
            else
                smaller = arr[right].freq < arr[left].freq ? right : left;

            if (arr[i].freq < arr[smaller].freq) break;

            Node tmp = arr[i];
            arr[i] = arr[smaller];
            arr[smaller] = tmp;

            i = smaller;
        }
    }

    Node* extractMin() {
        Node* min = new Node(arr[0].letters, arr[0].freq, arr[0].left, arr[0].right);

        arr[0] = arr[--heapPtr];
        bubbleDown(0);
        arr[heapPtr].update("", 0);

        return min;
    }

    std::string traverse(int i) {
        if (!arr[i].freq) return "";
        std::ostringstream oss;
        oss << "{(" << arr[i].letters << "/" << arr[i].freq << "), Left: " << traverse(getLeft(i)) << ", Right: " << traverse(getRight(i)) << "}";
        return oss.str();
    }

    ~MinHeap() {
        delete[] arr;
    }
};