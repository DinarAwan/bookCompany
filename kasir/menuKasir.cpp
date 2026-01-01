#include "menuKasir.h"
#include <iostream>
#include <fstream>
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
    string aktivitas;
};

class ManajemenBukuKasir {
private:
    // ========== SINGLE LINKED LIST UNTUK BUKU ==========
    struct NodeBuku {
        Buku data;
        NodeBuku* next;
    };

    NodeBuku* head;

    // ========== SINGLE LINKED LIST UNTUK STACK HISTORI ==========
    struct NodeRiwayat {
        Riwayat data;
        NodeRiwayat* next;
    };

    NodeRiwayat* topStack;

    void muatDariFile() {
        ifstream file("buku.txt");
        if (!file.is_open()) return; 

        while (true) { 
            Buku buku;
            if (!(file >> buku.id)) break; 
            file.ignore(1000, '\n'); 
            
            if (!getline(file, buku.judul)) break;
            if (!getline(file, buku.penulis)) break;
            
            if (!(file >> buku.tahun)) break;
            file.ignore(1000, '\n'); 
            
            if (!getline(file, buku.isbn)) break;
            if (!getline(file, buku.kategori)) break;
            
            if (!getline(file, buku.status)) {
                 buku.status = "Tersedia"; 
            }
            
            if (buku.status.empty() || (buku.status != "Tersedia" && buku.status != "Dipinjam") ) {
                buku.status = "Tersedia";
            }

            // Tambahkan ke linked list
            NodeBuku* nodeBaru = new NodeBuku;
            nodeBaru->data = buku;
            nodeBaru->next = nullptr;

            if (head == nullptr) {
                head = nodeBaru;
            } else {
                NodeBuku* temp = head;
                while (temp->next != nullptr) {
                    temp = temp->next;
                }
                temp->next = nodeBaru;
            }
        }
        file.close();
    }

    void simpanKeFile() {
        ofstream file("buku.txt", ios::trunc);
        NodeBuku* temp = head;
        while (temp != nullptr) {
            file << temp->data.id << "\n";
            file << temp->data.judul << "\n";
            file << temp->data.penulis << "\n";
            file << temp->data.tahun << "\n";
            file << temp->data.isbn << "\n";
            file << temp->data.kategori << "\n";
            file << temp->data.status << "\n"; 
            temp = temp->next;
        }
        file.close();
    }

    // --- FUNGSI MENYIMPAN HISTORI KE TXT ---
    void simpanHistori() {
        ofstream file("histori.txt", ios::trunc);
        if (!file.is_open()) return;

        // Hitung jumlah node di stack
        int count = 0;
        NodeRiwayat* temp = topStack;
        while (temp != nullptr) {
            count++;
            temp = temp->next;
        }

        file << count << "\n";

        // Simpan data dari top ke bottom
        temp = topStack;
        while (temp != nullptr) {
            file << temp->data.judulBuku << "\n";
            file << temp->data.aktivitas << "\n";
            temp = temp->next;
        }
        file.close();
    }

    // --- FUNGSI MEMUAT HISTORI DARI TXT ---
    void muatHistori() {
        ifstream file("histori.txt");
        if (!file.is_open()) return;

        int count;
        if (!(file >> count)) return; 
        file.ignore(1000, '\n');

        if (count <= 0) {
            file.close();
            return;
        }

        // Baca semua data ke array temporary
        Riwayat* tempArray = new Riwayat[count];
        for (int i = 0; i < count; i++) {
            if (!getline(file, tempArray[i].judulBuku)) {
                delete[] tempArray;
                file.close();
                return;
            }
            if (!getline(file, tempArray[i].aktivitas)) {
                delete[] tempArray;
                file.close();
                return;
            }
        }
        file.close();

        // Push dari belakang ke depan agar urutan tetap sama
        for (int i = count - 1; i >= 0; i--) {
            NodeRiwayat* nodeBaru = new NodeRiwayat;
            nodeBaru->data = tempArray[i];
            nodeBaru->next = topStack;
            topStack = nodeBaru;
        }

        delete[] tempArray;
    }

public:
    ManajemenBukuKasir() {
        head = nullptr;
        topStack = nullptr;
        
        muatDariFile(); 
        muatHistori();
    }

    ~ManajemenBukuKasir() {
        simpanKeFile(); 
        simpanHistori();
        
        // Hapus semua node buku
        while (head != nullptr) {
            NodeBuku* temp = head;
            head = head->next;
            delete temp;
        }

        // Hapus semua node stack
        while (topStack != nullptr) {
            NodeRiwayat* temp = topStack;
            topStack = topStack->next;
            delete temp;
        }
    }

    // --- FUNGSI STACK: PUSH ---
    void pushRiwayat(string judul, string aksi) {
        NodeRiwayat* nodeBaru = new NodeRiwayat;
        nodeBaru->data.judulBuku = judul;
        nodeBaru->data.aktivitas = aksi;
        nodeBaru->next = topStack;
        topStack = nodeBaru;
    }

    // --- FUNGSI STACK: POP ---
    void popRiwayat() {
        if (topStack == nullptr) {
            cout << "\n[INFO] Histori kosong! Tidak ada record yang bisa di-pop." << endl;
            return;
        }

        string judulDihapus = topStack->data.judulBuku;
        string aksiDihapus = topStack->data.aktivitas;

        NodeRiwayat* temp = topStack;
        topStack = topStack->next;
        delete temp;

        cout << "\n[SUKSES] Menghapus record terakhir dari Stack:" << endl;
        cout << "Buku: " << judulDihapus << " | Status: " << aksiDihapus << endl;
        cout << "(Catatan: Ini menghapus log dari memori dan akan diperbarui di file saat logout)." << endl;
    }

    // --- FUNGSI STACK: DISPLAY ---
    void lihatHistori() {
        cout << "============= Histori Aktivitas (Stack LIFO) =============" << endl;
        if (topStack == nullptr) {
            cout << "Stack Kosong. Belum ada aktivitas." << endl;
            return;
        }

        NodeRiwayat* temp = topStack;
        int counter = 1;
        while (temp != nullptr) {
            cout << "No. " << counter << " (TOP";
            if (counter > 1) cout << "-" << (counter - 1);
            cout << ") | ";
            cout << "Buku: " << temp->data.judulBuku << " | ";
            cout << "Status: " << temp->data.aktivitas << endl;
            temp = temp->next;
            counter++;
        }
        cout << "==========================================================" << endl;
    }

    void lihatbuku() {
        if (head == nullptr) {
            cout << "Tidak ada data buku tersedia" << endl;
            return;
        }

        cout << "============= Daftar Buku =============" << endl;
        NodeBuku* temp = head;
        while (temp != nullptr) {
            cout << "ID: " << temp->data.id << " | " << temp->data.judul << " (" << temp->data.status << ")" << endl;
            temp = temp->next;
        }
        cout << "---------------------------------------" << endl;
    }

    void pinjamBuku() {
        int idPinjam;
        cout << "============= Kasir: Proses Peminjaman =============" << endl;
        cout << "Masukkan ID Buku yang akan dipinjam: ";
        if (!(cin >> idPinjam)) {
            cout << "Input tidak valid.\n"; 
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            return;
        }
        cin.ignore(1000, '\n');

        NodeBuku* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == idPinjam) {
                if (temp->data.status == "Tersedia") {
                    temp->data.status = "Dipinjam";
                    simpanKeFile(); 
                    pushRiwayat(temp->data.judul, "DIPINJAM"); 
                    cout << "\n[SUKSES] Buku berhasil dipinjam." << endl;
                } else {
                    cout << "\n[GAGAL] Buku sedang dipinjam." << endl;
                }
                return; 
            }
            temp = temp->next;
        }
        cout << "\n[ERROR] Buku tidak ditemukan." << endl;
    }

    void kembalikanBuku() {
        int idKembali;
        cout << "============= Kasir: Proses Pengembalian =============" << endl;
        cout << "Masukkan ID Buku yang dikembalikan: ";
        if (!(cin >> idKembali)) {
            cout << "Input tidak valid.\n"; 
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            return;
        }
        cin.ignore(1000, '\n');

        NodeBuku* temp = head;
        while (temp != nullptr) {
            if (temp->data.id == idKembali) {
                if (temp->data.status == "Dipinjam") {
                    temp->data.status = "Tersedia";
                    simpanKeFile();
                    pushRiwayat(temp->data.judul, "DIKEMBALIKAN");
                    cout << "\n[SUKSES] Buku berhasil dikembalikan." << endl;
                } else {
                    cout << "\n[INFO] Buku sudah berstatus tersedia." << endl;
                }
                return; 
            }
            temp = temp->next;
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
        cout << "5. Hapus Histori Terakhir (POP)\n"; 
        cout << "0. Logout\n";
        cout << "Pilih menu: ";

        while (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka: "; 
            cin.clear(); 
            cin.ignore(1000, '\n'); 
            cout << "Pilih menu: "; 
        }
        cin.ignore(1000, '\n');

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
            case 5:
                manajemenKasir.popRiwayat();
                break;
            case 0:
                cout << "Logout berhasil! (Data tersimpan)\n";
                break;
            default:
                cout << "Pilihan tidak valid!\n";
        }
        cout << endl;
    } while (pilihan != 0);
}