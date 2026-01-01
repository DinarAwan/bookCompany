#ifndef MENUSUPERADMIN_H
#define MENUSUPERADMIN_H

#include <string>
#include "../auth/ManajemenUser.h"
#include "../KTP/ktp.h"
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
    }
    ~SuperAdmin() {
        delete manajemenUser;
    }
    
    void simpanKaryawanKeFile(const string &filename);
    void tambahKaryawan();
    void tampilkanKaryawan();
    void cariKaryawan();
    void loadKaryawanDariFile(const string &filename);
    
    // Fitur baru: Tambah user dengan KTP lengkap
    void tambahUserKaryawan();
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