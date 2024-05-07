#include "MinHeap.h"

MinHeap::MinHeap(int size) {
    this->size = size;
    arr = new Node[size];
}

int MinHeap::getParent(int i) { return (i - 1) / 2; }
int MinHeap::getLeft(int i) { return 2 * i + 1; }
int MinHeap::getRight(int i) { return 2 * i + 2; }

void MinHeap::insertValues(char letter, int freq, bool* overflow) {
    if (heapPtr >= size) {
        if (overflow) *overflow = true;
        return;
    }
    arr[heapPtr].update(letter, freq);

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

void MinHeap::orderInsertedNode() {
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

void MinHeap::bubbleDown(int index) {
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

Node* MinHeap::extractMin() {
    Node* min = new Node(arr[0].letter, arr[0].freq, arr[0].left, arr[0].right);

    arr[0] = arr[--heapPtr];
    bubbleDown(0);
    arr[heapPtr].update(NULL, 0);

    return min;
}

std::string MinHeap::traverse(int i) {
    if (!arr[i].freq) return "";
    std::ostringstream oss;
    oss << "{(" << arr[i].letter << "/" << arr[i].freq << "), Left: " << traverse(getLeft(i)) << ", Right: " << traverse(getRight(i)) << "}";
    return oss.str();
}

MinHeap::~MinHeap() {
    delete[] arr;
}