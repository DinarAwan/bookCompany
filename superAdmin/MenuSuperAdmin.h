#ifndef MENUSUPERADMIN_H
#define MENUSUPERADMIN_H

#include <string>
#include <fstream>
#include "../auth/ManajemenUser.h"
#include "../KTP/ktp.h"

using namespace std;

// Struktur data dasar untuk karyawan
struct karyawan {
    int id;
    string nama;
    string role;
};

// Struktur Node untuk Binary Search Tree
struct Node {
    karyawan data;
    Node* left;
    Node* right;
};

// Kelas untuk mengelola struktur data Binary Search Tree
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

// Kelas logika utama untuk Super Admin
class SuperAdmin {
public:
    ManajemenUser* manajemenUser;
    BinarySearchTree tree;  

    SuperAdmin() {
        manajemenUser = new ManajemenUser();
    }
    
    ~SuperAdmin() {
        delete manajemenUser;
    }
    
    void simpanKaryawanKeFile(const string &filename);
    void tambahKaryawan();
    void tampilkanKaryawan();
    void cariKaryawan();
    void loadKaryawanDariFile(const string &filename);
    
    // Fitur: Tambah user dengan data KTP lengkap
    void tambahUserKaryawan();
};

// Kelas untuk antarmuka Menu
class MenuSuperAdmin {
public:
    MenuSuperAdmin(){
        Sadmin.loadKaryawanDariFile("karyawan.txt");
    }
    SuperAdmin Sadmin;
    void tampilkanMenu();
};

#endif