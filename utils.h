#pragma once
#include <stdio.h>

Node *tregen(MinHeap *heap) {
    while (heap->heapPtr > 1) {
        Node *node1 = heap->extractMin();
        Node *node2 = heap->extractMin();

        int hPtr = heap->heapPtr;

        heap->arr[hPtr].letters = node1->letters + node2->letters;
        heap->arr[hPtr].freq = node1->freq + node2->freq;
        heap->arr[hPtr].left = node1;
        heap->arr[hPtr].right = node2;

        heap->orderInsertedNode();
    }
    return heap->extractMin();
}

bool genFreqTable(char *filePath, int *freqTable, int *size) {
    FILE *file = fopen(filePath, "r");
    if (!file) return false;

    char ch;
    do {
        (*size)++;
        ch = fgetc(file);
        if (ch >= 0 && ch < 128) freqTable[ch]++;
    } while (ch != EOF);
}