#include "BinNode.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

class PriorityQueue {
  public:
    Box *front;
    Box *rear;

    PriorityQueue() : front(NULL), rear(NULL) {}

    void enQueue(BinNode *node = NULL) {
        Box *tmp = new Box(node);

        if (this->rear == NULL) {
            this->front = this->rear = tmp;
            return;
        }

        this->rear->next = tmp;
        this->rear = tmp;
    }

    Box *deQueue(bool *underflow) { // ! THIS CAN'T AUTO FREE DATA, MUST BE FREED FROM USER
        if (this->front == NULL) {
            *underflow = true;
            return NULL;
        }

        Box *tmp = this->front;
        this->front = this->front->next;

        if (this->front == NULL)
            this->rear = NULL;

        *underflow = false;
        return tmp;
    }
};
