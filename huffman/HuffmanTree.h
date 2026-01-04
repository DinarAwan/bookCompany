#ifndef HUFFMANTREE_H
#define HUFFMANTREE_H

#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include "HuffmanNode.h"
using namespace std;

// Struct untuk frequency table (linked list)
struct FreqNode {
    char character;
    int frequency;
    FreqNode* next;
    
    FreqNode(char c, int f) : character(c), frequency(f), next(NULL) {}
};

// Struct untuk Huffman codes (linked list)
struct CodeNode {
    char character;
    string code;
    CodeNode* next;
    
    CodeNode(char c, string cd) : character(c), code(cd), next(NULL) {}
};

class HuffmanTree {
private:
    HuffmanNode* root;
    FreqNode* freqTableHead;
    CodeNode* codesHead;
    int totalChars;
    int originalSize;
    int compressedSize;
    
    // Helper functions
    void insertFreqNode(char c);
    FreqNode* findFreqNode(char c);
    void insertPQ(PriorityQueueNode*& head, HuffmanNode* node);
    HuffmanNode* extractMin(PriorityQueueNode*& head);
    void generateCodes(HuffmanNode* node, string code);
    string getCode(char c);
    void displayTreeHelper(HuffmanNode* node, string prefix, bool isLeft);
    void deleteTree(HuffmanNode* node);
    void clearFreqTable();
    void clearCodes();
    
public:
    HuffmanTree();
    ~HuffmanTree();
    
    // Main operations
    void buildFrequencyTable(const string& text);
    void buildHuffmanTree();
    string compress(const string& text);
    string decompress(const string& compressedBits);
    
    // Display & analysis
    void displayFrequencyTable();
    void displayCodes();
    void visualizeTree();
    void displayCompressionStats(int origSize, int compSize);
    double getCompressionRatio(int origSize, int compSize);
    
    // File operations
    void saveCompressed(const string& filename, const string& compressedData);
    string loadCompressed(const string& filename);
    
    // Getters
    HuffmanNode* getRoot() { return root; }
    int getOriginalSize() { return originalSize; }
    int getCompressedSize() { return compressedSize; }
};

#endif
