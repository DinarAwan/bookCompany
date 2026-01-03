#ifndef MENUSUPERADMIN_H
#define MENUSUPERADMIN_H

#include <string>
#include "../auth/ManajemenUser.h"
#include "../KTP/ktp.h"
#include "../huffman/HuffmanTree.h"  // NEW: Huffman compression
using namespace std;

struct karyawan {
    int id;
    string nama;
    string role;
};

//tree
struct Node {
    karyawan data;
    Node* left;
    Node* right;
};

class BinarySearchTree {
public:
    Node* root;

    BinarySearchTree();

    Node* buatNode(karyawan kar);
    Node* tambahdata(Node* root, karyawan kar);
    void tambahDataKaryawan (karyawan kar);

    Node* search(Node* root, int id);
    void inorder(Node* root);
    void simpankefile(Node *root, ofstream &file);
};

class SuperAdmin {
public:
    ManajemenUser* manajemenUser;

    BinarySearchTree tree;  
    SuperAdmin() {
        manajemenUser = new ManajemenUser();
        huffmanTree = new HuffmanTree();  // NEW: Initialize Huffman
    }
    ~SuperAdmin() {
        delete manajemenUser;
        delete huffmanTree;  // NEW: Cleanup
    }
    
    void simpanKaryawanKeFile(const string &filename);
    void tambahKaryawan();
    void tampilkanKaryawan();
    void cariKaryawan();
    void loadKaryawanDariFile(const string &filename);
    
    // Fitur baru: Tambah user dengan KTP lengkap
    void tambahUserKaryawan();
    
    // NEW: Huffman & Laporan features
    void menuManajemenLaporan();
    void bacaLaporanKasir();
    void bacaLaporanKaryawan();
    void bacaSemuaLaporan();
    void kompresiLaporan();
    void dekompresiLaporan();
    void analisisFrequensi();
    void visualisasiHuffmanTree();
    void statistikKompresi();
    
private:
    HuffmanTree* huffmanTree;  // NEW: Huffman tree instance
};

class MenuSuperAdmin {
public:
    MenuSuperAdmin(){
        Sadmin.loadKaryawanDariFile("karyawan.txt");
    }
    SuperAdmin Sadmin;
    void tampilkanMenu();

};

#endif