#include "MenuAdmin.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <limits>

using namespace std;

const string FILE_TUGAS = "tugas_satpam.txt";
const string FILE_TUGAS_OB = "tugas_ob.txt"; // File untuk Queue OB

void MenuAdmin::tampilkanMenu() {
    int pilihan;
    // Ubah ukuran array menu karena ada menu baru
    string array[6]; 
    array[0] = "1. Tambah Data Buku";
    array[1] = "2. Lihat Data Buku";
    array[2] = "3. Hapus Data Buku";
    array[3] = "4. Kelola Tugas Satpam (CRUD)";
    array[4] = "5. Kelola Tugas OB (Queue)"; // Menu Baru
    array[5] = "0. Logout";

    do {
        cout << "========== MENU ADMIN ==========\n";
        for (int i = 0; i < 6; i++) cout << array[i] << endl;
        cout << "Pilih menu: ";
        
        // Validasi input agar tidak error jika memasukkan huruf
        while (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
            case 1:
                cout << "Menambah data buku...\n";
                break;
            case 2:
                cout << "Menampilkan data buku...\n";
                break;
            case 3:
                cout << "Menghapus data buku...\n";
                break;
            case 4:
                kelolaTugasSatpam();
                break;
            case 5:
                kelolaTugasOb(); // Panggil fungsi baru
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

// ==========================================================
//                 BAGIAN TUGAS OB (QUEUE SYSTEM)
// ==========================================================

void MenuAdmin::kelolaTugasOb() {
    int pilihan;
    do {
        cout << "\n--- Kelola Tugas OB (Sistem Antrian/Queue) ---\n";
        cout << "1. Tambah Tugas ke Antrian (Enqueue)\n";
        cout << "2. Lihat Daftar Antrian\n";
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

// ENQUEUE: Menambahkan tugas ke urutan paling belakang
void MenuAdmin::tambahTugasOb() {
    string deskripsi;
    
    cout << "Masukkan tugas untuk OB: ";
    getline(cin, deskripsi);

    // Mode ios::app (Append) memastikan data masuk ke paling akhir file
    // Ini mensimulasikan 'Rear' pada Queue
    ofstream file(FILE_TUGAS_OB, ios::app);
    if (!file.is_open()) {
        cout << "Gagal membuka file database OB.\n";
        return;
    }

    // Kita simpan formatnya: Deskripsi
    // Tidak butuh ID acak, karena urutan berdasarkan baris (FIFO)
    file << deskripsi << "\n";
    file.close();

    cout << "Tugas berhasil dimasukkan ke antrian paling belakang!\n";
}

// VIEW QUEUE: Melihat antrian dari depan (Front) ke belakang (Rear)
void MenuAdmin::lihatTugasOb() {
    ifstream file(FILE_TUGAS_OB);
    if (!file.is_open()) {
        cout << "Belum ada antrian tugas untuk OB.\n";
        return;
    }

    cout << "\n=== Daftar Antrian Tugas OB (Priority: Atas = Dikerjakan Duluan) ===\n";
    string line;
    int antrianKe = 1;
    bool ada = false;

    while (getline(file, line)) {
        if (!line.empty()) {
            cout << "Antrian " << antrianKe++ << ": " << line << endl;
            ada = true;
        }
    }

    if (!ada) cout << "Antrian kosong. OB sedang santai.\n";
    else cout << "Total antrian: " << (antrianKe - 1) << endl;

    file.close();
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
                continue; // SKIP baris ini (DELETE)
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