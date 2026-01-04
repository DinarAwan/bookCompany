#include "HuffmanTree.h"
#include <iostream>
using namespace std;

// Constructor
HuffmanTree::HuffmanTree() {
    root = NULL;
    freqTableHead = NULL;
    codesHead = NULL;
    totalChars = 0;
    originalSize = 0;
    compressedSize = 0;
}

// Destructor
HuffmanTree::~HuffmanTree() {
    deleteTree(root);
    clearFreqTable();
    clearCodes();
}

void HuffmanTree::deleteTree(HuffmanNode* node) {
    if (node == NULL) return;
    deleteTree(node->left);
    deleteTree(node->right);
    delete node;
}

void HuffmanTree::clearFreqTable() {
    FreqNode* current = freqTableHead;
    while (current != NULL) {
        FreqNode* temp = current;
        current = current->next;
        delete temp;
    }
    freqTableHead = NULL;
}

void HuffmanTree::clearCodes() {
    CodeNode* current = codesHead;
    while (current != NULL) {
        CodeNode* temp = current;
        current = current->next;
        delete temp;
    }
    codesHead = NULL;
}

// Insert or update frequency node
void HuffmanTree::insertFreqNode(char c) {
    FreqNode* existing = findFreqNode(c);
    if (existing != NULL) {
        existing->frequency++;
    } else {
        FreqNode* newNode = new FreqNode(c, 1);
        newNode->next = freqTableHead;
        freqTableHead = newNode;
    }
    totalChars++;
}

// Find frequency node
FreqNode* HuffmanTree::findFreqNode(char c) {
    FreqNode* current = freqTableHead;
    while (current != NULL) {
        if (current->character == c) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Build frequency table from text
void HuffmanTree::buildFrequencyTable(const string& text) {
    // Clear previous data
    clearFreqTable();
    totalChars = 0;
    
    // Count frequency of each character
    for (size_t i = 0; i < text.length(); i++) {
        insertFreqNode(text[i]);
    }
    
    originalSize = text.length();
}

// Insert node into priority queue (sorted by frequency)
void HuffmanTree::insertPQ(PriorityQueueNode*& head, HuffmanNode* node) {
    PriorityQueueNode* newPQNode = new PriorityQueueNode(node);
    
    // Empty queue or insert at beginning
    if (head == NULL || node->frequency < head->node->frequency) {
        newPQNode->next = head;
        head = newPQNode;
        return;
    }
    
    // Find correct position
    PriorityQueueNode* current = head;
    while (current->next != NULL && current->next->node->frequency <= node->frequency) {
        current = current->next;
    }
    
    newPQNode->next = current->next;
    current->next = newPQNode;
}

// Extract minimum from priority queue
HuffmanNode* HuffmanTree::extractMin(PriorityQueueNode*& head) {
    if (head == NULL) return NULL;
    
    PriorityQueueNode* temp = head;
    HuffmanNode* minNode = head->node;
    head = head->next;
    delete temp;
    
    return minNode;
}

// Build Huffman Tree
void HuffmanTree::buildHuffmanTree() {
    if (freqTableHead == NULL) {
        cout << "Error: Frequency table is empty!\n";
        return;
    }
    
    // Clear previous tree
    deleteTree(root);
    root = NULL;
    
    // Create priority queue from frequency table
    PriorityQueueNode* pqHead = NULL;
    
    FreqNode* current = freqTableHead;
    while (current != NULL) {
        HuffmanNode* newNode = new HuffmanNode(current->character, current->frequency);
        insertPQ(pqHead, newNode);
        current = current->next;
    }
    
    // Build tree
    while (pqHead != NULL && pqHead->next != NULL) {
        // Extract two minimum nodes
        HuffmanNode* left = extractMin(pqHead);
        HuffmanNode* right = extractMin(pqHead);
        
        // Create parent node
        HuffmanNode* parent = new HuffmanNode('\0', left->frequency + right->frequency);
        parent->left = left;
        parent->right = right;
        
        // Insert parent back to queue
        insertPQ(pqHead, parent);
    }
    
    // Last node is root
    if (pqHead != NULL) {
        root = pqHead->node;
        delete pqHead;
    }
    
    // Generate Huffman codes
    clearCodes();
    if (root != NULL) {
        // Special case: only one unique character
        if (root->isLeaf()) {
            CodeNode* code = new CodeNode(root->data, "0");
            code->next = codesHead;
            codesHead = code;
        } else {
            generateCodes(root, "");
        }
    }
}

// Generate Huffman codes recursively
void HuffmanTree::generateCodes(HuffmanNode* node, string code) {
    if (node == NULL) return;
    
    if (node->isLeaf()) {
        // Store code for this character
        CodeNode* newCode = new CodeNode(node->data, code);
        newCode->next = codesHead;
        codesHead = newCode;
        return;
    }
    
    generateCodes(node->left, code + "0");
    generateCodes(node->right, code + "1");
}

// Get Huffman code for a character
string HuffmanTree::getCode(char c) {
    CodeNode* current = codesHead;
    while (current != NULL) {
        if (current->character == c) {
            return current->code;
        }
        current = current->next;
    }
    return "";
}

// Compress text to binary string
string HuffmanTree::compress(const string& text) {
    if (root == NULL) {
        cout << "Error: Huffman tree not built!\n";
        return "";
    }
    
    string compressed = "";
    for (size_t i = 0; i < text.length(); i++) {
        compressed += getCode(text[i]);
    }
    
    compressedSize = (compressed.length() + 7) / 8; // Convert bits to bytes
    return compressed;
}

// Decompress binary string to text
string HuffmanTree::decompress(const string& compressedBits) {
    if (root == NULL) {
        cout << "Error: Huffman tree not built!\n";
        return "";
    }
    
    string decompressed = "";
    HuffmanNode* current = root;
    
    // Special case: only one unique character
    if (root->isLeaf()) {
        for (size_t i = 0; i < compressedBits.length(); i++) {
            decompressed += root->data;
        }
        return decompressed;
    }
    
    for (size_t i = 0; i < compressedBits.length(); i++) {
        if (compressedBits[i] == '0') {
            current = current->left;
        } else {
            current = current->right;
        }
        
        if (current->isLeaf()) {
            decompressed += current->data;
            current = root;
        }
    }
    
    return decompressed;
}

// Display frequency table
void HuffmanTree::displayFrequencyTable() {
    cout << "\n========== FREQUENCY TABLE ==========\n";
    cout << "Character | Frequency | Percentage\n";
    cout << "----------|-----------|------------\n";
    
    FreqNode* current = freqTableHead;
    while (current != NULL) {
        char displayChar = current->character;
        string charStr;
        
        if (displayChar == '\n') charStr = "\\n";
        else if (displayChar == '\r') charStr = "\\r";
        else if (displayChar == '\t') charStr = "\\t";
        else if (displayChar == ' ') charStr = "SPACE";
        else charStr = displayChar;
        
        double percentage = (double)current->frequency / totalChars * 100.0;
        
        cout << left << setw(10) << charStr
             << "| " << setw(10) << current->frequency
             << "| " << fixed << setprecision(2) << percentage << "%\n";
        
        current = current->next;
    }
    cout << "=====================================\n";
    cout << "Total Characters: " << totalChars << "\n";
}

// Display Huffman codes
void HuffmanTree::displayCodes() {
    cout << "\n========== HUFFMAN CODES ==========\n";
    cout << "Character | Huffman Code\n";
    cout << "----------|-------------\n";
    
    CodeNode* current = codesHead;
    while (current != NULL) {
        char displayChar = current->character;
        string charStr;
        
        if (displayChar == '\n') charStr = "\\n";
        else if (displayChar == '\r') charStr = "\\r";
        else if (displayChar == '\t') charStr = "\\t";
        else if (displayChar == ' ') charStr = "SPACE";
        else charStr = displayChar;
        
        cout << left << setw(10) << charStr
             << "| " << current->code << "\n";
        
        current = current->next;
    }
    cout << "===================================\n";
}

// Visualize tree helper
void HuffmanTree::displayTreeHelper(HuffmanNode* node, string prefix, bool isLeft) {
    if (node == NULL) return;
    
    cout << prefix;
    cout << (isLeft ? "├──" : "└──");
    
    if (node->isLeaf()) {
        char displayChar = node->data;
        if (displayChar == '\n') cout << "'\\n'";
        else if (displayChar == '\r') cout << "'\\r'";
        else if (displayChar == '\t') cout << "'\\t'";
        else if (displayChar == ' ') cout << "' '";
        else cout << "'" << displayChar << "'";
        cout << " [" << node->frequency << "]\n";
    } else {
        cout << "[*]" << " [" << node->frequency << "]\n";
    }
    
    if (!node->isLeaf()) {
        displayTreeHelper(node->left, prefix + (isLeft ? "│   " : "    "), true);
        displayTreeHelper(node->right, prefix + (isLeft ? "│   " : "    "), false);
    }
}

// Visualize tree
void HuffmanTree::visualizeTree() {
    if (root == NULL) {
        cout << "Tree is empty!\n";
        return;
    }
    
    cout << "\n========== HUFFMAN TREE STRUCTURE ==========\n";
    cout << "[ROOT]\n";
    
    if (root->isLeaf()) {
        cout << "└── '" << root->data << "' [" << root->frequency << "]\n";
    } else {
        displayTreeHelper(root->left, "", true);
        displayTreeHelper(root->right, "", false);
    }
    cout << "==========================================\n";
}

// Display compression statistics
void HuffmanTree::displayCompressionStats(int origSize, int compSize) {
    cout << "\n========== STATISTIK KOMPRESI ==========\n";
    cout << "Ukuran Original  : " << origSize << " bytes\n";
    cout << "Ukuran Compressed: " << compSize << " bytes\n";
    cout << "Space Saved      : " << (origSize - compSize) << " bytes\n";
    
    double ratio = getCompressionRatio(origSize, compSize);
    cout << "Compression Ratio: " << fixed << setprecision(2) << ratio << "%\n";
    cout << "========================================\n";
}

// Calculate compression ratio
double HuffmanTree::getCompressionRatio(int origSize, int compSize) {
    if (origSize == 0) return 0.0;
    return ((double)(origSize - compSize) / origSize) * 100.0;
}

// Save compressed data to file
void HuffmanTree::saveCompressed(const string& filename, const string& compressedData) {
    ofstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Error: Cannot create file " << filename << "\n";
        return;
    }
    
    // Write magic number
    file << "HUFF";
    
    // Write original size
    file << originalSize << "\n";
    
    // Write compressed bit string length
    file << compressedData.length() << "\n";
    
    // Write compressed data
    file << compressedData << "\n";
    
    // Save frequency table for decompression
    FreqNode* current = freqTableHead;
    while (current != NULL) {
        file << current->character << "|" << current->frequency << "\n";
        current = current->next;
    }
    
    file.close();
}

// Load compressed data from file
string HuffmanTree::loadCompressed(const string& filename) {
    ifstream file(filename, ios::binary);
    if (!file.is_open()) {
        cout << "Error: Cannot open file " << filename << "\n";
        return "";
    }
    
    string magic;
    getline(file, magic);
    if (magic != "HUFF") {
        cout << "Error: Invalid compressed file format!\n";
        file.close();
        return "";
    }
    
    // Read original size
    file >> originalSize;
    file.ignore();
    
    // Read compressed length
    int compLen;
    file >> compLen;
    file.ignore();
    
    // Read compressed data
    string compressedData;
    getline(file, compressedData);
    
    // Read frequency table
    clearFreqTable();
    totalChars = 0;
    
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;
        
        size_t pos = line.find('|');
        if (pos != string::npos) {
            char c = line[0];
            int freq = stoi(line.substr(pos + 1));
            
            FreqNode* node = new FreqNode(c, freq);
            node->next = freqTableHead;
            freqTableHead = node;
            totalChars += freq;
        }
    }
    
    file.close();
    
    // Rebuild tree from frequency table
    buildHuffmanTree();
    
    return compressedData;
}
