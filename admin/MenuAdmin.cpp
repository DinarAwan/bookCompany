#include "MenuAdmin.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

const string FILE_TUGAS = "tugas_satpam.txt";
const string FILE_TUGAS_OB = "tugas_ob.txt"; 

// ===================== STRUKTUR DATA QUEUE (LINKED LIST) =====================

struct NodeTugas {
    string deskripsi;
    NodeTugas* next;
};

// Pointer untuk Queue Linked List
NodeTugas* front = nullptr;
NodeTugas* rear = nullptr;

// ===================== HELPER QUEUE (LOAD & SAVE) =====================

// Bersihkan memori Linked List agar tidak numpuk saat reload
void bersihkanQueue() {
    while (front != nullptr) {
        NodeTugas* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
}

// Enqueue ke Linked List (Helper function)
void enqueueLinkedList(string tugas) {
    NodeTugas* baru = new NodeTugas;
    baru->deskripsi = tugas;
    baru->next = nullptr;

    if (front == nullptr) {
        front = rear = baru;
    } else {
        rear->next = baru;
        rear = baru;
    }
}

// Baca file txt -> Masukkan ke Linked List
void muatAntrian() {
    bersihkanQueue(); // Reset dulu
    ifstream file(FILE_TUGAS_OB);
    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            enqueueLinkedList(line);
        }
    }
    file.close();
}

// Tulis Linked List -> Timpa ke file txt
void simpanAntrian() {
    ofstream file(FILE_TUGAS_OB, ios::trunc);
    NodeTugas* temp = front;
    while (temp != nullptr) {
        file << temp->deskripsi << "\n";
        temp = temp->next;
    }
    file.close();
}

// ===================== MENU ADMIN IMPL =====================

void MenuAdmin::tampilkanMenu() {
    // Load data antrian ke Linked List saat menu dibuka
    muatAntrian();

    int pilihan;
    string array[6]; 
    array[0] = "1. Tambah Data Buku";
    array[1] = "2. Lihat Data Buku";
    array[2] = "3. Hapus Data Buku";
    array[3] = "4. Kelola Tugas Satpam (CRUD)";
    array[4] = "5. Kelola Tugas OB (Queue Linked List)"; 
    array[5] = "0. Logout";

    do {
        cout << "========== MENU ADMIN ==========\n";
        for (int i = 0; i < 6; i++) cout << array[i] << endl;
        cout << "Pilih menu: ";
        
        while (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
            case 1: cout << "Menambah data buku...\n"; break;
            case 2: cout << "Menampilkan data buku...\n"; break;
            case 3: cout << "Menghapus data buku...\n"; break;
            case 4: kelolaTugasSatpam(); break;
            case 5: kelolaTugasOb(); break;
            case 0: cout << "Logout berhasil!\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
        cout << endl;
    } while (pilihan != 0);
}

// ===================== LOGIKA TUGAS OB (QUEUE) =====================

void MenuAdmin::kelolaTugasOb() {
    int pilihan;
    do {
        cout << "\n--- Kelola Tugas OB (Queue via Linked List) ---\n";
        cout << "1. Tambah Tugas ke Antrian (Enqueue)\n";
        cout << "2. Lihat Daftar Antrian (Traversal)\n";
        cout << "0. Kembali\n";
        cout << "Pilih menu: ";
        
        cin >> pilihan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
            case 1: tambahTugasOb(); break;
            case 2: lihatTugasOb(); break;
            case 0: break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);
}

// ENQUEUE: Menambah Node di REAR (Belakang)
void MenuAdmin::tambahTugasOb() {
    string deskripsi;
    cout << "Masukkan tugas untuk OB: ";
    getline(cin, deskripsi);

    // 1. Masukkan ke Linked List (Memory)
    enqueueLinkedList(deskripsi);

    // 2. Simpan Linked List ke File (Persistence)
    simpanAntrian();

    cout << "Tugas berhasil ditambahkan ke antrian (Linked List)!\n";
}

// VIEW: Traversal Linked List dari FRONT ke REAR
void MenuAdmin::lihatTugasOb() {
    if (front == nullptr) {
        cout << "[INFO] Antrian kosong. Linked List kosong.\n";
        return;
    }

    cout << "\n=== Daftar Antrian Tugas OB (Linked List) ===\n";
    NodeTugas* temp = front;
    int no = 1;
    
    while (temp != nullptr) {
        cout << "Antrian " << no++ << ": " << temp->deskripsi << endl;
        temp = temp->next;
    }
    cout << "Total Antrian: " << (no - 1) << endl;
}

// ==========================================================
//                 BAGIAN TUGAS SATPAM (CRUD BIASA)
// ==========================================================

void MenuAdmin::kelolaTugasSatpam() {
    int pilihan;
    do {
        cout << "\n--- Kelola Tugas Satpam ---\n";
        cout << "1. Tambah Tugas Baru\n";
        cout << "2. Lihat Semua Tugas\n";
        cout << "3. Edit Tugas\n";
        cout << "4. Hapus Tugas\n";
        cout << "0. Kembali\n";
        cout << "Pilih menu: ";
        cin >> pilihan;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch (pilihan) {
            case 1: tambahTugasSatpam(); break;
            case 2: lihatTugasSatpam(); break;
            case 3: editTugasSatpam(); break;
            case 4: hapusTugasSatpam(); break;
            case 0: break;
            default: cout << "Pilihan tidak valid.\n";
        }
    } while (pilihan != 0);
}

int MenuAdmin::getNextTaskId() {
    ifstream file(FILE_TUGAS);
    string line;
    int maxId = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        try {
            int id = stoi(idStr);
            if (id > maxId) maxId = id;
        } catch (...) {}
    }
    return maxId + 1;
}

void MenuAdmin::tambahTugasSatpam() {
    string deskripsi;
    int id = getNextTaskId();
    cout << "Masukkan deskripsi tugas baru: ";
    getline(cin, deskripsi);
    ofstream file(FILE_TUGAS, ios::app);
    if (!file.is_open()) {
        cout << "Gagal membuka file.\n";
        return;
    }
    file << id << "," << deskripsi << "\n";
    cout << "Tugas (ID: " << id << ") berhasil ditambahkan!\n";
}

void MenuAdmin::lihatTugasSatpam() {
    ifstream file(FILE_TUGAS);
    if (!file.is_open()) {
        cout << "Belum ada tugas.\n";
        return;
    }
    cout << "\n--- Daftar Tugas Satpam ---\n";
    string line;
    bool ada = false;
    while (getline(file, line)) {
        ada = true;
        stringstream ss(line);
        string id, desc;
        getline(ss, id, ',');
        getline(ss, desc);
        cout << "ID: " << id << " | Tugas: " << desc << endl;
    }
    if (!ada) cout << "Belum ada tugas.\n";
}

void MenuAdmin::editTugasSatpam() {
    lihatTugasSatpam();
    int idEdit;
    cout << "\nMasukkan ID tugas yang ingin diedit: ";
    cin >> idEdit;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    const int MAX = 500;
    string data[MAX];
    int count = 0;
    ifstream fileIn(FILE_TUGAS);
    string line;
    bool ditemukan = false;
    while (getline(fileIn, line) && count < MAX) {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        try {
            int id = stoi(idStr);
            if (id == idEdit) {
                ditemukan = true;
                string baru;
                cout << "Masukkan deskripsi baru: ";
                getline(cin, baru);
                data[count] = to_string(idEdit) + "," + baru;
            } else {
                data[count] = line;
            }
        } catch (...) {
            data[count] = line;
        }
        count++;
    }
    fileIn.close();
    if (!ditemukan) {
        cout << "ID tidak ditemukan.\n";
        return;
    }
    ofstream fileOut(FILE_TUGAS, ios::trunc);
    for (int i = 0; i < count; i++) {
        fileOut << data[i] << "\n";
    }
    cout << "Tugas berhasil diperbarui.\n";
}

void MenuAdmin::hapusTugasSatpam() {
    lihatTugasSatpam();
    int idHapus;
    cout << "\nMasukkan ID tugas yang ingin dihapus: ";
    cin >> idHapus;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    const int MAX = 500;
    string data[MAX];
    int count = 0;
    ifstream fileIn(FILE_TUGAS);
    string line;
    bool ditemukan = false;
    while (getline(fileIn, line) && count < MAX) {
        stringstream ss(line);
        string idStr;
        getline(ss, idStr, ',');
        try {
            int id = stoi(idStr);
            if (id == idHapus) {
                ditemukan = true;
                continue; 
            }
        } catch (...) {}
        data[count] = line;
        count++;
    }
    fileIn.close();
    if (!ditemukan) {
        cout << "ID tidak ditemukan.\n";
        return;
    }
    ofstream fileOut(FILE_TUGAS, ios::trunc);
    for (int i = 0; i < count; i++) {
        fileOut << data[i] << "\n";
    }
    cout << "Tugas berhasil dihapus.\n";
}