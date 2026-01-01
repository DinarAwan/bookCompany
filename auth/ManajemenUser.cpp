#include "ManajemenUser.h"
#include <iostream>
#include <fstream>

using namespace std;

ManajemenUser::ManajemenUser() {
    head = nullptr;
    bstRoot = nullptr;
    loadDariFile();
}

ManajemenUser::~ManajemenUser() {
    simpanKeFile();
    
    // Hapus semua node linked list
    while (head != nullptr) {
        UserNode* temp = head;
        head = head->next;
        delete temp;
    }
    
    // Hapus BST
    clearBST(bstRoot);
}

void ManajemenUser::loadDariFile() {
    ifstream file("users.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        // Format: NIK|Nama|TglLahir(hari-bulan-tahun)|JenisKelamin|Alamat|Agama|StatusPerkawinan|Pekerjaan|Kewarganegaraan|Role|Password
        size_t pos = 0;
        size_t next_pos;
        KTP user;

        // Parse NIK
        next_pos = line.find('|', pos);
        user.nik = stoll(line.substr(pos, next_pos - pos));
        pos = next_pos + 1;

        // Parse Nama
        next_pos = line.find('|', pos);
        user.nama = line.substr(pos, next_pos - pos);
        pos = next_pos + 1;

        // Parse Tanggal Lahir (format: hari-bulan-tahun)
        next_pos = line.find('|', pos);
        string tglLahir = line.substr(pos, next_pos - pos);
        size_t dash1 = tglLahir.find('-');
        size_t dash2 = tglLahir.find('-', dash1 + 1);
        user.tanggalLahir.hari = stoi(tglLahir.substr(0, dash1));
        user.tanggalLahir.bulan = stoi(tglLahir.substr(dash1 + 1, dash2 - dash1 - 1));
        user.tanggalLahir.tahun = stoi(tglLahir.substr(dash2 + 1));
        pos = next_pos + 1;

        // Parse Jenis Kelamin
        next_pos = line.find('|', pos);
        user.jenisKelamin = (JenisKelamin)stoi(line.substr(pos, next_pos - pos));
        pos = next_pos + 1;

        // Parse Alamat
        next_pos = line.find('|', pos);
        user.alamat = line.substr(pos, next_pos - pos);
        pos = next_pos + 1;

        // Parse Agama
        next_pos = line.find('|', pos);
        user.agama = (Agama)stoi(line.substr(pos, next_pos - pos));
        pos = next_pos + 1;

        // Parse Status Perkawinan
        next_pos = line.find('|', pos);
        user.statusPerkawinan = (StatusPerkawinan)stoi(line.substr(pos, next_pos - pos));
        pos = next_pos + 1;

        // Parse Pekerjaan
        next_pos = line.find('|', pos);
        user.pekerjaan = line.substr(pos, next_pos - pos);
        pos = next_pos + 1;

        // Parse Kewarganegaraan
        next_pos = line.find('|', pos);
        user.kewarganegaraan = (Kewarganegaraan)stoi(line.substr(pos, next_pos - pos));
        pos = next_pos + 1;

        // Parse Role
        next_pos = line.find('|', pos);
        user.role = stoi(line.substr(pos, next_pos - pos));
        pos = next_pos + 1;

        // Parse Password (sisa string)
        user.password = line.substr(pos);

        // Tambahkan ke linked list
        UserNode* newNode = new UserNode;
        newNode->data = user;
        newNode->next = nullptr;

        if (head == nullptr) {
            head = newNode;
        } else {
            UserNode* temp = head;
            while (temp->next != nullptr) {
                temp = temp->next;
            }
            temp->next = newNode;
        }
    }
    file.close();
}

void ManajemenUser::simpanKeFile() {
    ofstream file("users.txt", ios::trunc);
    if (!file.is_open()) {
        cout << "Error: Tidak dapat menyimpan data user.\n";
        return;
    }

    UserNode* temp = head;
    while (temp != nullptr) {
        // Format: NIK|Nama|TglLahir|JenisKelamin|Alamat|Agama|StatusPerkawinan|Pekerjaan|Kewarganegaraan|Role|Password
        file << temp->data.nik << "|"
             << temp->data.nama << "|"
             << temp->data.tanggalLahir.hari << "-" 
             << temp->data.tanggalLahir.bulan << "-" 
             << temp->data.tanggalLahir.tahun << "|"
             << temp->data.jenisKelamin << "|"
             << temp->data.alamat << "|"
             << temp->data.agama << "|"
             << temp->data.statusPerkawinan << "|"
             << temp->data.pekerjaan << "|"
             << temp->data.kewarganegaraan << "|"
             << temp->data.role << "|"
             << temp->data.password << "\n";
        temp = temp->next;
    }
    file.close();
}

bool ManajemenUser::tambahUser(const KTP& user) {
    // Cek apakah NIK sudah ada
    if (isNIKExists(user.nik)) {
        return false;
    }

    UserNode* newNode = new UserNode;
    newNode->data = user;
    newNode->next = nullptr;

    if (head == nullptr) {
        head = newNode;
    } else {
        UserNode* temp = head;
        while (temp->next != nullptr) {
            temp = temp->next;
        }
        temp->next = newNode;
    }

    simpanKeFile();
    return true;
}

KTP* ManajemenUser::cariByNIK(long long int nik) {
    UserNode* temp = head;
    while (temp != nullptr) {
        if (temp->data.nik == nik) {
            return &(temp->data);
        }
        temp = temp->next;
    }
    return nullptr;
}

bool ManajemenUser::verifikasiLogin(long long int nik, const string& password, int& role) {
    KTP* user = cariByNIK(nik);
    if (user == nullptr) {
        return false;
    }
    
    if (user->password == password) {
        role = user->role;
        return true;
    }
    
    return false;
}

void ManajemenUser::displayAllUsers() {
    if (head == nullptr) {
        cout << "Belum ada user terdaftar.\n";
        return;
    }

    cout << "\n========== DAFTAR USER ==========\n";
    UserNode* temp = head;
    int counter = 1;
    
    while (temp != nullptr) {
        cout << "\n--- User " << counter++ << " ---\n";
        cout << "NIK      : " << temp->data.nik << "\n";
        cout << "Nama     : " << temp->data.nama << "\n";
        cout << "Role     : ";
        switch(temp->data.role) {
            case 1: cout << "Admin"; break;
            case 2: cout << "Super Admin"; break;
            case 3: cout << "Pengguna"; break;
            case 4: cout << "Satpam"; break;
            case 5: cout << "Karyawan"; break;
            case 6: cout << "Kasir"; break;
            case 7: cout << "OB"; break;
            default: cout << "Unknown"; break;
        }
        cout << "\n";
        temp = temp->next;
    }
    cout << "==================================\n";
}

bool ManajemenUser::isNIKExists(long long int nik) {
    return (cariByNIK(nik) != nullptr);
}

// ==================== BST IMPLEMENTATION ====================

// Insert node ke BST (sorted by NIK)
ManajemenUser::BSTNode* ManajemenUser::insertBST(BSTNode* root, const KTP& user) {
    if (root == nullptr) {
        BSTNode* newNode = new BSTNode;
        newNode->data = user;
        newNode->left = nullptr;
        newNode->right = nullptr;
        return newNode;
    }
    
    if (user.nik < root->data.nik) {
        root->left = insertBST(root->left, user);
    } else if (user.nik > root->data.nik) {
        root->right = insertBST(root->right, user);
    }
    // Jika sama, skip (duplicate)
    
    return root;
}

// Search user by NIK di BST
ManajemenUser::BSTNode* ManajemenUser::searchBST(BSTNode* root, long long int nik) {
    if (root == nullptr || root->data.nik == nik) {
        return root;
    }
    
    if (nik < root->data.nik) {
        return searchBST(root->left, nik);
    } else {
        return searchBST(root->right, nik);
    }
}

// Inorder traversal (sorted)
void ManajemenUser::inorderBST(BSTNode* root) {
    if (root == nullptr) return;
    
    inorderBST(root->left);
    
    // Display data
    cout << "\n===========================================\n";
    cout << "NIK              : " << root->data.nik << "\n";
    cout << "Nama             : " << root->data.nama << "\n";
    cout << "Tanggal Lahir    : " << root->data.tanggalLahir.hari << "-"
         << root->data.tanggalLahir.bulan << "-"
         << root->data.tanggalLahir.tahun << "\n";
    cout << "Alamat           : " << root->data.alamat << "\n";
    cout << "Pekerjaan        : " << root->data.pekerjaan << "\n";
    cout << "Role             : ";
    switch(root->data.role) {
        case 1: cout << "Admin"; break;
        case 2: cout << "Super Admin"; break;
        case 3: cout << "Pengguna"; break;
        case 4: cout << "Satpam"; break;
        case 5: cout << "Karyawan"; break;
        case 6: cout << "Kasir"; break;
        case 7: cout << "OB"; break;
        default: cout << "Unknown"; break;
    }
    cout << "\n";
    
    inorderBST(root->right);
}

// Clear BST (cleanup memory)
void ManajemenUser::clearBST(BSTNode* root) {
    if (root == nullptr) return;
    
    clearBST(root->left);
    clearBST(root->right);
    delete root;
}

// Build BST from linked list
void ManajemenUser::buildBSTFromList() {
    // Clear existing BST
    clearBST(bstRoot);
    bstRoot = nullptr;
    
    // Build BST dari linked list
    UserNode* temp = head;
    while (temp != nullptr) {
        bstRoot = insertBST(bstRoot, temp->data);
        temp = temp->next;
    }
    
    cout << "BST berhasil dibangun dari data user!\n";
}

// Display users in sorted order (by NIK)
void ManajemenUser::displayUsersSorted() {
    if (bstRoot == nullptr) {
        cout << "BST kosong! Silakan build BST terlebih dahulu.\n";
        return;
    }
    
    cout << "\n========== DAFTAR USER (Sorted by NIK) ==========\n";
    inorderBST(bstRoot);
    cout << "===========================================\n";
}

// Search user menggunakan BST
KTP* ManajemenUser::searchUserBST(long long int nik) {
    if (bstRoot == nullptr) {
        cout << "BST kosong! Silakan build BST terlebih dahulu.\n";
        return nullptr;
    }
    
    BSTNode* result = searchBST(bstRoot, nik);
    if (result != nullptr) {
        return &(result->data);
    }
    return nullptr;
}
