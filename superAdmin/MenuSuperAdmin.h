#ifndef MENUSUPERADMIN_H
#define MENUSUPERADMIN_H

#include <string>
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

    BinarySearchTree tree;  
    void simpanKaryawanKeFile(const string &filename);
    void tambahKaryawan();
    void tampilkanKaryawan();
    void cariKaryawan();
    void loadKaryawanDariFile(const string &filename);
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
