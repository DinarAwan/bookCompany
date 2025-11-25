#include "MenuAdmin.h"
#include <iostream>
#include <fstream>      // Untuk file I/O (ifstream, ofstream)
#include <string>       // Untuk std::string
#include <vector>       // Untuk std::vector (digunakan saat edit/hapus)
#include <sstream>      // Untuk std::stringstream (mem-parsing data file)
#include <limits>       // Untuk std::numeric_limits

using namespace std;

// Nama file untuk menyimpan tugas
const string FILE_TUGAS = "tugas_satpam.txt";

void MenuAdmin::tampilkanMenu() {
    int pilihan;
    string array[5];
    array[0] = "1. Tambah Data Buku";
    array[1] = "2. Lihat Data Bukuu";
    array[2] = "3. Hapus Data Bukuu";
    array[3] = "4. Kelola Tugas Satpam";
    array[4] = "4. logout";
    
    do {
        cout << "========== MENU ADMIN ==========\n";
        cout << array[0]<<endl;
        cout << array[1]<<endl;
        cout << array[2]<<endl;
        cout << array[3]<<endl;
        cout << array[4]<<endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                cout << "Menambah data buku...\n";
                // ... (logika tambah buku Anda)
                break;
            case 2:
                cout << "Menampilkan data buku...\n";
                // ... (logika lihat buku Anda)
                break;
            case 3:
                cout << "Menghapus data buku...\n";
                // ... (logika hapus buku Anda)
                break;
            case 4:
                kelolaTugasSatpam(); // <-- Panggil fungsi baru
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

// ===================================================
// ==          IMPLEMENTASI FITUR TUGAS SATPAM      ==
// ===================================================

void MenuAdmin::kelolaTugasSatpam() {
    int pilihan;
    do {
        cout << "\n--- Kelola Tugas Satpam ---\n";
        cout << "1. Tambah Tugas Baru\n";
        cout << "2. Lihat Semua Tugas\n";
        cout << "3. Edit Tugas\n";
        cout << "4. Hapus Tugas\n";
        cout << "0. Kembali ke Menu Admin\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        // Membersihkan buffer input
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 

        switch (pilihan) {
            case 1:
                tambahTugasSatpam();
                break;
            case 2:
                lihatTugasSatpam();
                break;
            case 3:
                editTugasSatpam();
                break;
            case 4:
                hapusTugasSatpam();
                break;
            case 0:
                cout << "Kembali ke Menu Admin...\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
        cout << endl;
    } while (pilihan != 0);
}

// Helper untuk mendapatkan ID selanjutnya
int MenuAdmin::getNextTaskId() {
    ifstream file(FILE_TUGAS);
    string line;
    int maxId = 0;
    while (getline(file, line)) {
        stringstream ss(line);
        string segment;
        int id;
        
        getline(ss, segment, ','); // Ambil bagian ID
        try {
            id = stoi(segment);
            if (id > maxId) {
                maxId = id;
            }
        } catch (...) {
            // Abaikan baris yang formatnya salah
        }
    }
    file.close();
    return maxId + 1;
}

// (CREATE)
void MenuAdmin::tambahTugasSatpam() {
    string deskripsi;
    int id = getNextTaskId();

    cout << "Masukkan deskripsi tugas baru: ";
    getline(cin, deskripsi);

    // Buka file dalam mode 'append' (menambahkan di akhir)
    ofstream file(FILE_TUGAS, ios::app); 
    if (!file.is_open()) {
        cout << "Error: Tidak dapat membuka file " << FILE_TUGAS << endl;
        return;
    }

    file << id << "," << deskripsi << "\n";
    file.close();

    cout << "Tugas baru (ID: " << id << ") berhasil ditambahkan!\n";
}

// (READ)
void MenuAdmin::lihatTugasSatpam() {
    ifstream file(FILE_TUGAS);
    if (!file.is_open()) {
        cout << "Belum ada tugas yang ditambahkan.\n";
        return;
    }

    string line;
    cout << "\n--- Daftar Tugas Satpam ---\n";
    bool adaTugas = false;

    while (getline(file, line)) {
        adaTugas = true;
        stringstream ss(line);
        string id, deskripsi;

        getline(ss, id, ',');
        getline(ss, deskripsi); // Ambil sisa baris sebagai deskripsi

        cout << "ID: " << id << " | Tugas: " << deskripsi << endl;
    }

    if (!adaTugas) {
        cout << "Belum ada tugas yang ditambahkan.\n";
    }
    file.close();
}

// (UPDATE)
void MenuAdmin::editTugasSatpam() {
    lihatTugasSatpam(); // Tampilkan tugas dulu agar admin tahu ID-nya
    int idEdit;
    cout << "\nMasukkan ID tugas yang ingin diedit: ";
    cin >> idEdit;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

    vector<string> lines;
    ifstream fileIn(FILE_TUGAS);
    string line;
    bool ditemukan = false;

    // 1. Baca semua data dari file ke vector
    while (getline(fileIn, line)) {
        stringstream ss(line);
        string segment;
        getline(ss, segment, ',');
        
        try {
            if (stoi(segment) == idEdit) {
                ditemukan = true;
                string deskripsiBaru;
                cout << "Masukkan deskripsi tugas yang baru: ";
                getline(cin, deskripsiBaru);
                lines.push_back(to_string(idEdit) + "," + deskripsiBaru);
            } else {
                lines.push_back(line);
            }
        } catch (...) {
            lines.push_back(line); // Jaga baris yg formatnya salah
        }
    }
    fileIn.close();

    if (!ditemukan) {
        cout << "ID Tugas tidak ditemukan.\n";
        return;
    }

    // 2. Tulis ulang file dari vector
    ofstream fileOut(FILE_TUGAS, ios::trunc); // trunc: hapus isi file lama
    for (const string& l : lines) {
        fileOut << l << "\n";
    }
    fileOut.close();

    cout << "Tugas (ID: " << idEdit << ") berhasil diperbarui.\n";
}

// (DELETE)
void MenuAdmin::hapusTugasSatpam() {
    lihatTugasSatpam(); // Tampilkan tugas dulu
    int idHapus;
    cout << "\nMasukkan ID tugas yang ingin dihapus: ";
    cin >> idHapus;
    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Clear buffer

    vector<string> lines;
    ifstream fileIn(FILE_TUGAS);
    string line;
    bool ditemukan = false;

    // 1. Baca semua data, KECUALI yang ingin dihapus
    while (getline(fileIn, line)) {
        stringstream ss(line);
        string segment;
        getline(ss, segment, ',');
        
        try {
            if (stoi(segment) == idHapus) {
                ditemukan = true;
                continue; // Lewati baris ini (jangan tambahkan ke vector)
            } else {
                lines.push_back(line);
            }
        } catch (...) {
            lines.push_back(line);
        }
    }
    fileIn.close();

    if (!ditemukan) {
        cout << "ID Tugas tidak ditemukan.\n";
        return;
    }

    // 2. Tulis ulang file dari vector
    ofstream fileOut(FILE_TUGAS, ios::trunc);
    for (const string& l : lines) {
        fileOut << l << "\n";
    }
    fileOut.close();

    cout << "Tugas (ID: " << idHapus << ") berhasil dihapus.\n";
}