// tofix

// #pragma once
// #include "BinNode.h"
// #include "PriorityQueue.h"

// BinNode *generateHuffTree(PriorityQueue *freqsQ) {
//     bool underflow = false;

//     while (true) {
//         Box *firstElem = freqsQ->deQueue(&underflow);
//         Box *secElem = freqsQ->deQueue(&underflow);

//         if (underflow) return firstElem->node;

//         int freqTotal = firstElem->node->freqs + secElem->node->freqs;

//         char *letters = new char[strlen(firstElem->node->letters) + strlen(secElem->node->letters) + 1];
//         letters = strcpy(letters, firstElem->node->letters);
//         strcat(letters, secElem->node->letters);

//         BinNode *node = new BinNode(letters, freqTotal);
//         if (firstElem->node->freqs <= secElem->node->freqs) {
//             node->left = firstElem->node;
//             node->right = secElem->node;
//         } else {
//             node->left = secElem->node;
//             node->right = firstElem->node;
//         }

//         delete firstElem; // ! TODO IMPLEMENT DECONSTRUCTOR INTEAD OF THIS
//         delete secElem;
//         freqsQ->enQueue(node);
//     }
// }