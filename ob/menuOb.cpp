#include "menuOb.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ===================== IMPLEMENTASI KELAS MENU OB =====================

// ---- Simpan data laporan ke file ----
void MenuOb::simpanKeFile() {
    ofstream file("laporan.txt");
    laporan* temp = head;
    while (temp != nullptr) {
        file << temp->id << "|"
             << temp->tanggal << "|"
             << temp->JenisPekerjaan << "|"
             << temp->LamaPekerjaan << "|"
             << temp->laporanHariIni << "|\n";
        temp = temp->next;
    }
    file.close();
}

// ---- Load data laporan dari file ----
void MenuOb::loadDariFile() {
    ifstream file("laporan.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);
        size_t pos4 = line.find('|', pos3 + 1);
        size_t pos5 = line.find('|', pos4 + 1);

        int id = stoi(line.substr(0, pos1));
        string tanggal = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string jenis = line.substr(pos2 + 1, pos3 - pos2 - 1);
        string lama = line.substr(pos3 + 1, pos4 - pos3 - 1);
        string laporanHarian = line.substr(pos4 + 1, pos5 - pos4 - 1);

        laporan* baru = new laporan(id, tanggal, jenis, lama, laporanHarian);
        if (head == nullptr)
            head = baru;
        else {
            laporan* temp = head;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = baru;
        }
    }
    file.close();
}

// ---- Dapatkan ID baru (otomatis naik) ----
int MenuOb::getNextId() {
    int id = 1;
    laporan* temp = head;
    while (temp != nullptr) {
        id = temp->id + 1;
        temp = temp->next;
    }
    return id;
}

// ---- Konstruktor (otomatis load dari file) ----
MenuOb::MenuOb() {
    head = nullptr;
    loadDariFile();
}

// ---- Tambah laporan baru ----
void MenuOb::tambahLaporan() {
    int id = getNextId();
    cin.ignore();

    string tanggal, jenis, lama, laporanHarian;

    cout << "Masukkan tanggal (DD-MM-YYYY): ";
    getline(cin, tanggal);
    cout << "Masukkan jenis pekerjaan: ";
    getline(cin, jenis);
    cout << "Masukkan lama pekerjaan: ";
    getline(cin, lama);
    cout << "Masukkan laporan hari ini: ";
    getline(cin, laporanHarian);

    laporan* baru = new laporan(id, tanggal, jenis, lama, laporanHarian);

    if (head == nullptr)
        head = baru;
    else {
        laporan* temp = head;
        while (temp->next != nullptr)
            temp = temp->next;
        temp->next = baru;
    }

    simpanKeFile();
    cout << "✅ Laporan berhasil ditambahkan!\n";
}

// ---- Lihat semua laporan ----
void MenuOb::lihatLaporan() {
    if (head == nullptr) {
        cout << "Belum ada laporan.\n";
        return;
    }

    cout << "\n===== Daftar Laporan =====\n";
    laporan* temp = head;
    while (temp != nullptr) {
        cout << "ID        : " << temp->id << endl;
        cout << "Tanggal   : " << temp->tanggal << endl;
        cout << "Jenis     : " << temp->JenisPekerjaan << endl;
        cout << "Lama      : " << temp->LamaPekerjaan << endl;
        cout << "Laporan   : " << temp->laporanHariIni << endl;
        cout << "--------------------------------------\n";
        temp = temp->next;
    }
}

// ---- Hapus laporan berdasarkan ID ----
void MenuOb::hapusLaporan() {
    if (head == nullptr) {
        cout << "Belum ada laporan untuk dihapus.\n";
        return;
    }

    int idHapus;
    cout << "Masukkan ID laporan yang ingin dihapus: ";
    cin >> idHapus;

    laporan* temp = head;
    laporan* prev = nullptr;
    bool ditemukan = false;

    while (temp != nullptr) {
        if (temp->id == idHapus) {
            ditemukan = true;
            if (prev == nullptr)
                head = temp->next;
            else
                prev->next = temp->next;
            delete temp;
            break;
        }
        prev = temp;
        temp = temp->next;
    }

    if (ditemukan) {
        simpanKeFile();
        cout << "✅ Laporan berhasil dihapus!\n";
    } else {
        cout << "❌ Laporan dengan ID tersebut tidak ditemukan.\n";
    }
}

// ---- Menu utama OB ----
void MenuOb::tampilkanMenu() {
    int pilihan;
    do {
        cout << "\n========== MENU OB ==========\n";
        cout << "1. Tambah laporan\n";
        cout << "2. Lihat laporan\n";
        cout << "3. Hapus laporan\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                tambahLaporan();
                break;
            case 2:
                lihatLaporan();
                break;
            case 3:
                hapusLaporan();
                break;
            case 0:
                cout << "Logout berhasil!\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);
}
