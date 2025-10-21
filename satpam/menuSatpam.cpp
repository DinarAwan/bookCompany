#include "menuSatpam.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

const string FILE_TUGAS_SATPAM = "tugas_satpam.txt";

void MenuSatpam::tampilkanMenu() {
    int pilihan;
    do {
        cout << "\n========== MENU SATPAM ==========\n";
        cout << "1. Lihat Daftar Tugas\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                lihatTugas();
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

// (READ-ONLY)
void MenuSatpam::lihatTugas() {
    ifstream file(FILE_TUGAS_SATPAM);
    if (!file.is_open()) {
        cout << "Belum ada tugas yang ditambahkan oleh Admin.\n";
        return;
    }

    string line;
    cout << "\n--- Daftar Tugas Anda ---\n";
    bool adaTugas = false;

    while (getline(file, line)) {
        adaTugas = true;
        stringstream ss(line);
        string id, deskripsi;

        getline(ss, id, ',');       // Ambil ID
        getline(ss, deskripsi); // Ambil sisa baris sebagai deskripsi

        // Hanya tampilkan deskripsinya saja agar lebih rapi untuk satpam
        cout << "- " << deskripsi << endl; 
        
        // Alternatif jika ingin menampilkan ID juga:
        // cout << "ID: " << id << " | Tugas: " << deskripsi << endl;
    }

    if (!adaTugas) {
        cout << "Belum ada tugas yang ditambahkan oleh Admin.\n";
    }
    file.close();
}