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
};

class SuperAdmin {
public:

    BinarySearchTree tree;  

    void tambahKaryawan();
    void tampilkanKaryawan();
    void cariKaryawan();
};

class MenuSuperAdmin {
public:
    SuperAdmin Sadmin;
    void tampilkanMenu();
};

#endif
