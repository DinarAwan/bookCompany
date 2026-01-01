#include "MenuSuperAdmin.h"
#include <iostream>
#include <fstream>
#include <sstream>
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
        } 
        else if (kar.id > root->data.id) {
            root->right = tambahdata(root->right, kar);
        } 
        else {
            cout << "ID karyawan sudah ada!\n";
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

//binary search tree inorder
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

//simpan ke file
void  BinarySearchTree::simpankefile(Node *root, ofstream &file) {
  if (!root) return;
    simpankefile(root->left, file);

    file << root->data.id << "|"
         << root->data.nama << "|"
         << root->data.role << endl;

    simpankefile(root->right, file);

}

void SuperAdmin::simpanKaryawanKeFile(const string &filekaryawan) {
    ofstream file(filekaryawan);
    if (!file.is_open()) {
        cout << "Gagal membuka file untuk menyimpan data karyawan.\n";
        return;
    }

    tree.simpankefile(tree.root, file);
    file.close();
    cout << "Data karyawan berhasil disimpan ke " << filekaryawan << endl;
}

void SuperAdmin::loadKaryawanDariFile(const string &filekaryawan) {
    string line;
    karyawan kar;
    string idtostr;

    ifstream file(filekaryawan);
    if (!file.is_open()) {
        cout << "Gagal membuka file untuk memuat data karyawan.\n";
        return;
    }

    while (getline(file, line)){
        stringstream ss(line);
        getline(ss, idtostr, '|');
        getline(ss, kar.nama, '|');
        getline(ss, kar.role, '|');
        kar.id = stoi(idtostr);
        tree.tambahDataKaryawan(kar);
    }
     file.close();
     cout << "Data karyawan berhasil dimuat dari " << filekaryawan << endl;
}

// Fungsi baru: Tambah User Karyawan dengan data KTP lengkap
void SuperAdmin::tambahUserKaryawan() {
    KTP userBaru;
    
    cout << "\n========== TAMBAH USER/KARYAWAN ==========\n";
    
    // Input NIK
    cout << "Masukkan NIK: ";
    cin >> userBaru.nik;
    
    // Cek apakah NIK sudah ada
    if (manajemenUser->isNIKExists(userBaru.nik)) {
        cout << "Error: NIK sudah terdaftar!\n";
        cin.ignore(1000, '\n');
        return;
    }
    
    cin.ignore(1000, '\n');
    
    // Input Nama
    cout << "Masukkan Nama: ";
    getline(cin, userBaru.nama);
    
    // Input Tanggal Lahir
    cout << "Masukkan Tanggal Lahir (hari bulan tahun): ";
    cin >> userBaru.tanggalLahir.hari 
        >> userBaru.tanggalLahir.bulan 
        >> userBaru.tanggalLahir.tahun;
    
    // Input Jenis Kelamin
    cout << "Masukkan Jenis Kelamin (1: LAKI-LAKI, 2: PEREMPUAN): ";
    int jk;
    cin >> jk;
    userBaru.jenisKelamin = (jk == 1) ? LAKI_LAKI : PEREMPUAN;
    cin.ignore(1000, '\n');
    
    // Input Alamat
    cout << "Masukkan Alamat: ";
    getline(cin, userBaru.alamat);
    
    // Input Agama
    cout << "Masukkan Agama (1:ISLAM, 2:KRISTEN, 3:KATOLIK, 4:HINDU, 5:BUDDHA, 6:KHONGHUCU): ";
    int ag;
    cin >> ag;
    userBaru.agama = (Agama)ag;
    
    // Input Status Perkawinan
    cout << "Masukkan Status Perkawinan (1: BELUM KAWIN, 2: KAWIN): ";
    int st;
    cin >> st;
    userBaru.statusPerkawinan = (st == 1) ? BELUM_KAWIN : KAWIN;
    cin.ignore(1000, '\n');
    
    // Input Pekerjaan
    cout << "Masukkan Pekerjaan: ";
    getline(cin, userBaru.pekerjaan);
    
    // Input Kewarganegaraan
    cout << "Kewarganegaraan (1: WNI, 2: WNA): ";
    int kw;
    cin >> kw;
    userBaru.kewarganegaraan = (kw == 1) ? WNI : WNA;
    cin.ignore(1000, '\n');
    
    // Input Role
    cout << "\nPilih Role:\n";
    cout << "1. Admin\n";
    cout << "2. Super Admin\n";
    cout << "4. Satpam\n";
    cout << "5. Karyawan\n";
    cout << "6. Kasir\n";
    cout << "7. OB\n";
    cout << "Masukkan Role: ";
    cin >> userBaru.role;
    cin.ignore(1000, '\n');
    
    // Input Password
    cout << "Masukkan Password: ";
    getline(cin, userBaru.password);
    
    // Tambahkan user
    if (manajemenUser->tambahUser(userBaru)) {
        cout << "\nUser/Karyawan berhasil ditambahkan!\n";
        cout << "NIK: " << userBaru.nik << "\n";
        cout << "Nama: " << userBaru.nama << "\n";
    } else {
        cout << "\nGagal menambahkan user!\n";
    }
}


void MenuSuperAdmin::tampilkanMenu() {
    
    int pilihan;
    do {
        cout << "====== MENU SUPER ADMIN ======\n";
        cout << "1. Tambah User/Karyawan (KTP Lengkap)\n";
        cout << "2. Lihat Semua User\n";
        cout << "3. Build BST (Sort User by NIK)\n";
        cout << "4. Lihat User Terurut (BST)\n";
        cout << "5. Cari User by NIK (BST)\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                Sadmin.tambahUserKaryawan();
                break;
            case 2:
                Sadmin.manajemenUser->displayAllUsers();
                break;
            case 3:
                Sadmin.manajemenUser->buildBSTFromList();
                break;
            case 4:
                Sadmin.manajemenUser->displayUsersSorted();
                break;
            case 5: {
                long long int nikCari;
                cout << "Masukkan NIK yang dicari: ";
                cin >> nikCari;
                
                KTP* result = Sadmin.manajemenUser->searchUserBST(nikCari);
                if (result != nullptr) {
                    cout << "\n========== USER DITEMUKAN ==========\n";
                    cout << "NIK           : " << result->nik << "\n";
                    cout << "Nama          : " << result->nama << "\n";
                    cout << "Alamat        : " << result->alamat << "\n";
                    cout << "Pekerjaan     : " << result->pekerjaan << "\n";
                    cout << "Role          : ";
                    switch(result->role) {
                        case 1: cout << "Admin"; break;
                        case 2: cout << "Super Admin"; break;
                        case 3: cout << "Pengguna"; break;
                        case 4: cout << "Satpam"; break;
                        case 5: cout << "Karyawan"; break;
                        case 6: cout << "Kasir"; break;
                        case 7: cout << "OB"; break;
                        default: cout << "Unknown"; break;
                    }
                    cout << "\n====================================\n";
                } else {
                    cout << "User dengan NIK " << nikCari << " tidak ditemukan.\n";
                }
                break;
            }
            case 0:
                 Sadmin.simpanKaryawanKeFile("karyawan.txt");
                cout << "Logout berhasil!\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
        cout << endl;
    } while (pilihan != 0);
}
