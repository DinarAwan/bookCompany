#ifndef HUFFMANNODE_H
#define HUFFMANNODE_H

struct HuffmanNode {
    char data;
    int frequency;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode* next;  // Untuk linked list
    
    HuffmanNode(char d = '\0', int freq = 0) 
        : data(d), frequency(freq), left(NULL), right(NULL), next(NULL) {}
    
    bool isLeaf() const {
        return (left == NULL && right == NULL);
    }
};

// Linked list untuk priority queue manual
struct PriorityQueueNode {
    HuffmanNode* node;
    PriorityQueueNode* next;
    
    PriorityQueueNode(HuffmanNode* n) : node(n), next(NULL) {}
};

#endif
