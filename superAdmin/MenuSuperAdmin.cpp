#include "MenuSuperAdmin.h"
#include <iostream>
using namespace std;

BinarySearchTree::BinarySearchTree() {
    root = NULL;
}

Node* BinarySearchTree::buatNode(karyawan kar) {
    Node* baru = new Node;
    baru->data = kar;
    baru->left = NULL;
    baru->right = NULL;
    return baru;
}

Node* BinarySearchTree::tambahdata(Node* root, karyawan kar) {
    if (root == NULL) {
        return buatNode(kar);
    }
    if (kar.id < root->data.id) {
        root->left = tambahdata(root->left, kar);
    } else {
        root->right = tambahdata(root->right, kar);
    }
    return root;
}

void BinarySearchTree::tambahDataKaryawan (karyawan kar) {
    root = tambahdata(root, kar);
}

Node* BinarySearchTree::search(Node* root, int id) {
    if (root == nullptr || root->data.id == id) {
        return root;
    }
    if (id < root->data.id) {
        return search(root->left, id);
    }else {
    return search(root->right, id);
    }
}

void BinarySearchTree::inorder(Node* root) {
    if (!root) return;
        inorder(root->left);
        cout<<endl;
        cout<<" ===================================\n";
        cout << " ID: " << root->data.id << endl <<
        " Nama: " << root->data.nama << endl <<
        " Role: " << root->data.role << endl;
        inorder(root->right);
        cout<<"====================================\n";
    
}

void SuperAdmin::tambahKaryawan() {
    karyawan kar;
    cout << "Masukkan ID Karyawan: ";
    cin >> kar.id;
    cin.ignore();
    cout << "Masukkan Nama Karyawan: ";
    getline(cin, kar.nama);
    cout << "Masukkan Role Karyawan: ";
    getline(cin, kar.role);

    tree.tambahDataKaryawan(kar);
    cout << "Karyawan berhasil ditambahkan!\n";
}

void SuperAdmin::tampilkanKaryawan() {
    cout << "\n=== Daftar Karyawan ===\n";
    tree.inorder(tree.root);
}

void SuperAdmin::cariKaryawan() {
    int idCari;
    cout << "Masukkan ID Karyawan yang dicari: ";
    cin >> idCari;

    Node* hasil = tree.search(tree.root, idCari);
    if (hasil != nullptr) {
        cout << "Karyawan ditemukan dengan id: " << hasil->data.id << endl <<
        " Nama: " << hasil->data.nama << endl <<
        " Role: " << hasil->data.role << endl;
    } else {
        cout << "Karyawan dengan ID " << idCari << " tidak ditemukan.\n";
    }
}

void MenuSuperAdmin::tampilkanMenu() {
    
    int pilihan;
    do {
        cout << "====== MENU SUPER ADMIN ======\n";
        cout << "1. Tambah Karyawan\n";
        cout << "2. Tampilkan Karyawan\n";
        cout << "3. Cari Karyawan\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                Sadmin.tambahKaryawan();
                break;
            case 2:
                Sadmin.tampilkanKaryawan();
                break;
            case 3:
               Sadmin.cariKaryawan();
                break;
            case 0:
                cout << "Logout berhasil!\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
        cout << endl;
    } while (pilihan != 0);
}

