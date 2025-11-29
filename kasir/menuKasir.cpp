#include "menuKasir.h"
#include <iostream>
#include <fstream>
#include <limits>
#include <string>

using namespace std;

// --- STRUKTUR DATA UTAMA ---
struct Buku {
    int id;
    string judul;
    string penulis;
    int tahun;
    string isbn;
    string kategori;
    string status; 
};

// --- STRUKTUR UNTUK STACK (HISTORI) ---
struct Riwayat {
    string judulBuku;
    string aktivitas; // Contoh: "Dipinjam" atau "Dikembalikan"
};

class ManajemenBukuKasir {
private:
    Buku* bukuDaftar;
    int jumlahBuku;
    int kapasitas;

    // --- VARIABEL STACK ARRAY ---
    static const int MAX_STACK = 50; // Maksimal histori yang disimpan sementara
    Riwayat stackRiwayat[MAX_STACK]; // Array untuk stack
    int top; // Penunjuk elemen teratas

    void perbesarKapasitas() {
        kapasitas *= 2;
        Buku* bukuBaru = new Buku[kapasitas];
        for (int i = 0; i < jumlahBuku; i++) {
            bukuBaru[i] = bukuDaftar[i];
        }
        delete[] bukuDaftar;
        bukuDaftar = bukuBaru;
    }

    void muatDariFile() {
        ifstream file("buku.txt");
        if (!file.is_open()) return; 

        while (true) { 
            Buku buku;
            if (!(file >> buku.id)) break; 
            file.ignore(numeric_limits<streamsize>::max(), '\n'); 
            
            if (!getline(file, buku.judul)) break;
            if (!getline(file, buku.penulis)) break;
            
            if (!(file >> buku.tahun)) break;
            file.ignore(numeric_limits<streamsize>::max(), '\n'); 
            
            if (!getline(file, buku.isbn)) break;
            if (!getline(file, buku.kategori)) break;
            
            if (!getline(file, buku.status)) {
                 buku.status = "Tersedia"; 
            }
            
            if (buku.status.empty() || (buku.status != "Tersedia" && buku.status != "Dipinjam") ) {
                buku.status = "Tersedia";
            }

            if (jumlahBuku >= kapasitas) {
                perbesarKapasitas();
            }
            bukuDaftar[jumlahBuku++] = buku;
        }
        file.close();
    }

    void simpanKeFile() {
        ofstream file("buku.txt", ios::trunc);
        for (int i = 0; i < jumlahBuku; i++) {
            file << bukuDaftar[i].id << "\n";
            file << bukuDaftar[i].judul << "\n";
            file << bukuDaftar[i].penulis << "\n";
            file << bukuDaftar[i].tahun << "\n";
            file << bukuDaftar[i].isbn << "\n";
            file << bukuDaftar[i].kategori << "\n";
            file << bukuDaftar[i].status << "\n"; 
        }
        file.close();
    }

public:
    ManajemenBukuKasir() {
        kapasitas = 10;
        jumlahBuku = 0;
        bukuDaftar = new Buku[kapasitas];
        
        // Inisialisasi Stack
        top = -1; // Menandakan stack kosong
        
        muatDariFile(); 
    }

    ~ManajemenBukuKasir() {
        simpanKeFile(); 
        delete[] bukuDaftar;
    }

    // --- FUNGSI STACK: PUSH (Menambah Histori) ---
    void pushRiwayat(string judul, string aksi) {
        if (top >= MAX_STACK - 1) {
            cout << "[Info] Histori penuh (Stack Overflow), data lama tidak tersimpan." << endl;
            return;
        }
        
        top++; // Naikkan penunjuk
        stackRiwayat[top].judulBuku = judul;
        stackRiwayat[top].aktivitas = aksi;
    }

    // --- FUNGSI STACK BARU: POP (Menghapus Histori Terakhir) ---
    void popRiwayat() {
        // Cek Underflow (Apakah stack kosong?)
        if (top == -1) {
            cout << "\n[INFO] Histori kosong! Tidak ada record yang bisa di-pop." << endl;
            return;
        }

        // Ambil data sebelum dihapus (hanya untuk ditampilkan ke user)
        string judulDihapus = stackRiwayat[top].judulBuku;
        string aksiDihapus = stackRiwayat[top].aktivitas;

        // LAKUKAN POP
        top--; 

        cout << "\n[SUKSES] Menghapus record terakhir dari Stack:" << endl;
        cout << "Buku: " << judulDihapus << " | Status: " << aksiDihapus << endl;
        cout << "(Catatan: Ini hanya menghapus log histori, status di database buku tetap)." << endl;
    }

    // --- FUNGSI STACK: DISPLAY (Lihat Histori - LIFO) ---
    void lihatHistori() {
        cout << "============= Histori Aktivitas (Stack LIFO) =============" << endl;
        if (top == -1) {
            cout << "Stack Kosong. Belum ada aktivitas sesi ini." << endl;
            return;
        }

        // Loop dari TOP ke 0
        for (int i = top; i >= 0; i--) {
            cout << "No. " << (top - i + 1) << " (TOP-" << (top - i) << ") | "; 
            cout << "Buku: " << stackRiwayat[i].judulBuku << " | ";
            cout << "Status: " << stackRiwayat[i].aktivitas << endl;
        }
        cout << "==========================================================" << endl;
    }

    void lihatbuku() {
        if (jumlahBuku == 0) {
            cout << "Tidak ada data buku tersedia" << endl;
            return;
        }

        cout << "============= Daftar Buku =============" << endl;
        for (int i = 0; i < jumlahBuku; i++) {
            cout << "ID: " << bukuDaftar[i].id << " | " << bukuDaftar[i].judul << " (" << bukuDaftar[i].status << ")" << endl;
        }
        cout << "---------------------------------------" << endl;
    }

    void pinjamBuku() {
        int idPinjam;
        cout << "============= Kasir: Proses Peminjaman =============" << endl;
        cout << "Masukkan ID Buku yang akan dipinjam: ";
        if (!(cin >> idPinjam)) {
            cout << "Input tidak valid.\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < jumlahBuku; i++) {
            if (bukuDaftar[i].id == idPinjam) {
                if (bukuDaftar[i].status == "Tersedia") {
                    bukuDaftar[i].status = "Dipinjam";
                    simpanKeFile(); 
                    
                    // --> PUSH KE STACK
                    pushRiwayat(bukuDaftar[i].judul, "DIPINJAM"); 
                    
                    cout << "\n[SUKSES] Buku berhasil dipinjam." << endl;
                } else {
                    cout << "\n[GAGAL] Buku sedang dipinjam." << endl;
                }
                return; 
            }
        }
        cout << "\n[ERROR] Buku tidak ditemukan." << endl;
    }

    void kembalikanBuku() {
        int idKembali;
        cout << "============= Kasir: Proses Pengembalian =============" << endl;
        cout << "Masukkan ID Buku yang dikembalikan: ";
        if (!(cin >> idKembali)) {
            cout << "Input tidak valid.\n"; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); return;
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < jumlahBuku; i++) {
            if (bukuDaftar[i].id == idKembali) {
                if (bukuDaftar[i].status == "Dipinjam") {
                    bukuDaftar[i].status = "Tersedia";
                    simpanKeFile();
                    
                    // --> PUSH KE STACK
                    pushRiwayat(bukuDaftar[i].judul, "DIKEMBALIKAN");

                    cout << "\n[SUKSES] Buku berhasil dikembalikan." << endl;
                } else {
                    cout << "\n[INFO] Buku sudah berstatus tersedia." << endl;
                }
                return; 
            }
        }
        cout << "\n[ERROR] Buku tidak ditemukan." << endl;
    }
};

void MenuKasir::tampilkanMenu() {
    int pilihan;
    ManajemenBukuKasir manajemenKasir; 

    do {
        cout << "========== MENU KASIR ==========\n";
        cout << "1. Lihat Daftar Buku\n"; 
        cout << "2. Pinjam Buku\n"; 
        cout << "3. Kembalikan Buku\n";
        cout << "4. Lihat Histori (STACK)\n"; 
        cout << "5. Hapus Histori Terakhir (POP)\n"; // <-- MENU BARU
        cout << "0. Logout\n";
        cout << "Pilih menu: ";

        while (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka: "; cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n'); cout << "Pilih menu: "; 
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
            case 1:
                manajemenKasir.lihatbuku();
                break;
            case 2:
                manajemenKasir.pinjamBuku();
                break;
            case 3:
                manajemenKasir.kembalikanBuku();
                break;
            case 4: 
                manajemenKasir.lihatHistori();
                break;
            case 5: // <-- PANGGIL FUNGSI POP
                manajemenKasir.popRiwayat();
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