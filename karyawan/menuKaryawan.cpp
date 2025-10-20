#include "menuKaryawan.h"
#include <iostream>
#include <fstream>
#include <limits>
using namespace std;

struct Buku {
    int id;
    string judul;
    string penulis;
    int tahun;
    string isbn;
    string kategori;
};

class ManajemenBuku {
private:
    Buku* bukuDaftar;
    int jumlahBuku;
    int kapasitas;

    void perbesarKapasitas() {
        kapasitas *= 2;
        Buku* bukuBaru = new Buku[kapasitas];
        for (int i = 0; i < jumlahBuku; i++) {
            bukuBaru[i] = bukuDaftar[i];
        }
        delete[] bukuDaftar;
        bukuDaftar = bukuBaru;
    }

    // Fungsi untuk memuat data dari file
    void muatDariFile() {
        ifstream file("buku.txt");
        if (!file.is_open()) return; // jika belum ada file, abaikan

        while (!file.eof()) {
            Buku buku;
            file >> buku.id;
            if (file.fail()) break;
            file.ignore();
            getline(file, buku.judul);
            getline(file, buku.penulis);
            file >> buku.tahun;
            file.ignore();
            getline(file, buku.isbn);
            getline(file, buku.kategori);
            if (jumlahBuku >= kapasitas) {
                perbesarKapasitas();
            }
            bukuDaftar[jumlahBuku++] = buku;
        }
        file.close();
    }

    // Fungsi untuk menyimpan data ke file
    void simpanKeFile() {
        ofstream file("buku.txt", ios::trunc);
        for (int i = 0; i < jumlahBuku; i++) {
            file << bukuDaftar[i].id << "\n";
            file << bukuDaftar[i].judul << "\n";
            file << bukuDaftar[i].penulis << "\n";
            file << bukuDaftar[i].tahun << "\n";
            file << bukuDaftar[i].isbn << "\n";
            file << bukuDaftar[i].kategori << "\n";
        }
        file.close();
    }

public:
    ManajemenBuku() {
        kapasitas = 10;
        jumlahBuku = 0;
        bukuDaftar = new Buku[kapasitas];
        muatDariFile(); // load data saat awal
    }

    ~ManajemenBuku() {
        simpanKeFile(); // simpan otomatis saat keluar
        delete[] bukuDaftar;
    }

    void tambahbuku() {
        int jumlah;
        cout << "============= Tambah Buku =============" << endl;
        cout << "Masukkan jumlah buku yang akan ditambahkan: ";
        if (!(cin >> jumlah)) {
            cout << "Input tidak valid. Masukkan angka.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        if (jumlah <= 0) {
            cout << "Jumlah harus lebih dari 0.\n";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < jumlah; i++) {
            if (jumlahBuku >= kapasitas) {
                perbesarKapasitas();
            }

            Buku bukuBaru;
            cout << "\nData buku ke-" << (i + 1) << ":" << endl;

            cout << "Masukkan ID Buku: ";
            while (!(cin >> bukuBaru.id)) {
                cout << "ID harus angka. Coba lagi: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Masukkan Judul Buku: ";
            getline(cin, bukuBaru.judul);

            cout << "Masukkan Penulis Buku: ";
            getline(cin, bukuBaru.penulis);

            cout << "Masukkan Tahun Terbit: ";
            while (!(cin >> bukuBaru.tahun)) {
                cout << "Tahun harus angka. Coba lagi: ";
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            cin.ignore(numeric_limits<streamsize>::max(), '\n');

            cout << "Masukkan ISBN: ";
            getline(cin, bukuBaru.isbn);

            cout << "Masukkan Kategori: ";
            getline(cin, bukuBaru.kategori);

            bukuDaftar[jumlahBuku++] = bukuBaru;
        }
        simpanKeFile();
        cout << "Buku berhasil ditambahkan!" << endl;
    }

    void lihatbuku() {
        if (jumlahBuku == 0) {
            cout << "Tidak ada data buku tersedia" << endl;
            return;
        }

        cout << "============= Daftar Buku =============" << endl;
        for (int i = 0; i < jumlahBuku; i++) {
            cout << "Buku ke-" << (i + 1) << ":\n";
            cout << "ID Buku: " << bukuDaftar[i].id << endl;
            cout << "Judul: " << bukuDaftar[i].judul << endl;
            cout << "Penulis: " << bukuDaftar[i].penulis << endl;
            cout << "Tahun Terbit: " << bukuDaftar[i].tahun << endl;
            cout << "ISBN: " << bukuDaftar[i].isbn << endl;
            cout << "Kategori: " << bukuDaftar[i].kategori << endl;
            cout << "-----------------------------------" << endl;
        }
    }

    void hapusbuku() {
        Buku bukuHapus;
        cout << "Masukkan ID Buku yang akan dihapus: ";
        cin >> bukuHapus.id;

        for (int i = 0; i < jumlahBuku; i++) {
            if (bukuDaftar[i].id == bukuHapus.id) {
                for (int j = i; j < jumlahBuku - 1; j++) {
                    bukuDaftar[j] = bukuDaftar[j + 1];
                }
                jumlahBuku--;
                simpanKeFile();
                cout << "Buku dengan ID " << bukuHapus.id << " berhasil dihapus." << endl;
                return;
            }
        }
        cout << "Buku dengan ID " << bukuHapus.id << " tidak ditemukan" << endl;
    }
};

void MenuKaryawan::tampilkanMenu() {
    int pilihan;
    ManajemenBuku manajemenBuku;
    do {

        cout << "========== MENU Karyawan ==========\n";
        cout << "1. Tambah Data Buku\n";
        cout << "2. Lihat Data Buku\n";
        cout << "3. Hapus Data Buku\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";

        // Validasi input pilihan
        while (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Pilih menu: "; // Tanya lagi
        }

        // !! INI SOLUSI UTAMANYA !!
        // Bersihkan buffer SETELAH membaca 'pilihan', 
        // SEBELUM masuk ke switch-case.
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
            case 1:
                cout << "Menambah data buku...\n\n";
                manajemenBuku.tambahbuku();
                break;
            case 2:
                cout << "Menampilkan data buku...\n\n";
                manajemenBuku.lihatbuku();
                break;
            case 3:
                cout << "Menghapus data buku...\n\n";
                manajemenBuku.hapusbuku();
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