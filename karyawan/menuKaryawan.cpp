#include "menuKaryawan.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>

#include "../Buku.h"

using namespace std;

// ===================== CLASS ABSEN KARYAWAN =====================
class AbsenKaryawan {
public:
    struct AbsenK {
        int id;
        string nama;
        string tanggal;
        string waktuMasuk;
        string waktuKeluar;
        AbsenK* next;
    };

    AbsenK* head;
    const string NAMA_FILE_ABSEN = "absenKaryawan.txt"; 

    AbsenKaryawan() {
        head = nullptr;
        loadDariFile();
    }

    string ambilWaktuSekarang() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[10];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", ltm);
        return buffer;
    }

    string ambilTanggalSekarang() {
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
        return buffer;
    }

    void absenMasuk() {
        int id;
        string nama;

        cout << "Masukkan ID Karyawan: ";
        cin >> id;
        cin.ignore(); // Bersihkan buffer

        cout << "Masukkan nama Karyawan: ";
        getline(cin, nama);

        if (cekSudahAbsenMasuk(id)) {
            cout << "Anda sudah melakukan absen masuk hari ini!\n";
            return;
        }

        string tanggal = ambilTanggalSekarang();
        string waktuMasuk = ambilWaktuSekarang();
        string waktuKeluar = "-";

        AbsenK* baru = new AbsenK{id, nama, tanggal, waktuMasuk, waktuKeluar, nullptr};

        if (head == nullptr) {
            head = baru;
            head->next = head; 
        } else {
            AbsenK* temp = head;
            while (temp->next != head)
                temp = temp->next;
            temp->next = baru;
            baru->next = head;
        }

        cout << nama << " berhasil absen masuk pada " << tanggal << " pukul " << waktuMasuk << endl;
        simpankeFile();
    }

    bool cekSudahAbsenMasuk(int idCek) {
        if (head == nullptr) return false;
        AbsenK* temp = head;
        string tglSkrg = ambilTanggalSekarang();
        do {
            if (temp->id == idCek && temp->tanggal == tglSkrg) {
                return true;
            }
            temp = temp->next;
        } while (temp != head);
        return false;
    }

    void tampilAbsenSendiri(int idCari) {
        if (head == nullptr) {
            cout << "Belum ada data absensi.\n";
            return;
        }

        AbsenK* temp = head;
        bool ditemukan = false;

        cout << "\n===== Riwayat Absensi ID: " << idCari << " =====\n";
        do {
            if (temp->id == idCari) {
                cout << "Tanggal      : " << temp->tanggal << endl;
                cout << "Nama         : " << temp->nama << endl;
                cout << "Waktu Masuk  : " << temp->waktuMasuk << endl;
                cout << "Waktu Keluar : " << temp->waktuKeluar << endl;
                cout << "--------------------------------\n";
                ditemukan = true;
            }
            temp = temp->next;
        } while (temp != head);

        if (!ditemukan)
            cout << "Belum ada data absensi untuk ID tersebut.\n";
    }

    void absenKeluar() {
        int idCari;
        if (head == nullptr) {
            cout << "Belum ada data absensi.\n";
            return;
        }

        cout << "Masukkan ID anda: ";
        cin >> idCari;
        AbsenK* temp = head;
        bool ditemukan = false;
        string tglSekarang = ambilTanggalSekarang();

        do {
            if (temp->id == idCari && temp->tanggal == tglSekarang) {
                ditemukan = true;

                if (temp->waktuKeluar != "-") {
                    cout << "Anda sudah absen keluar hari ini pada pukul " << temp->waktuKeluar << endl;
                    return;
                }
                
                temp->waktuKeluar = ambilWaktuSekarang();
                cout << "Anda berhasil absen keluar pada pukul " << temp->waktuKeluar << endl;
                simpankeFile(); 
                return;
            }
            temp = temp->next;
        } while (temp != head);

        if (!ditemukan) {
            cout << "Anda belum absen masuk hari ini atau ID salah." << endl;
        }
    }

    // SIMPAN FORMAT PIPE (|) UNTUK ABSEN (Sesuai original)
    void simpankeFile() {
        ofstream file(NAMA_FILE_ABSEN); 
        if (!file.is_open()) return;

        if (head == nullptr) {
            file.close(); return;
        }

        AbsenK* temp = head;
        do {
            file << temp->id << "|"
                 << temp->nama << "|"
                 << temp->tanggal << "|"
                 << temp->waktuMasuk << "|"
                 << temp->waktuKeluar << endl;
            temp = temp->next;
        } while (temp != head);
        file.close();
    }

    // LOAD FORMAT PIPE (|) UNTUK ABSEN
    void loadDariFile() {
        ifstream file(NAMA_FILE_ABSEN);
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            stringstream ss(line);
            string s_id, s_nama, s_tanggal, s_masuk, s_keluar;

            if (getline(ss, s_id, '|') &&
                getline(ss, s_nama, '|') &&
                getline(ss, s_tanggal, '|') &&
                getline(ss, s_masuk, '|') &&
                getline(ss, s_keluar)) {
                
                try {
                    int id = stoi(s_id);
                    AbsenK* baru = new AbsenK{id, s_nama, s_tanggal, s_masuk, s_keluar, nullptr};

                    if (head == nullptr) {
                        head = baru;
                        head->next = head;
                    } else {
                        AbsenK* temp = head;
                        while (temp->next != head)
                            temp = temp->next;
                        temp->next = baru;
                        baru->next = head;
                    }
                } catch(...) {}
            }
        }
        file.close();
    }
};

// ===================== CLASS MANAJEMEN BUKU =====================
class ManajemenBuku {
private:
    struct NodeBuku {
        Buku data;
        NodeBuku* next;
    };

    NodeBuku* head;
    const string NAMA_FILE_BUKU = "buku.txt";

    // LOAD FORMAT TURUN KE BAWAH (NEWLINE) SESUAI REQUEST
    void muatDariFile() {
        ifstream file(NAMA_FILE_BUKU);
        if (!file.is_open()) return;

        Buku buku;
        // Kita baca ID dulu sebagai pemicu loop
        while (file >> buku.id) {
            file.ignore(1000, '\n'); // PENTING: Skip sisa enter setelah baca angka

            if (!getline(file, buku.judul)) break;
            if (!getline(file, buku.penulis)) break;
            
            if (!(file >> buku.tahun)) break;
            file.ignore(1000, '\n'); 

            if (!getline(file, buku.isbn)) break;
            if (!getline(file, buku.kategori)) break;
            
            // Handle status (bisa kosong atau ada)
            if (!getline(file, buku.status)) buku.status = "Tersedia";
            if (buku.status.empty()) buku.status = "Tersedia";

            if (!(file >> buku.harga)) buku.harga = 0;
            file.ignore(1000, '\n');

            if (!(file >> buku.stok)) buku.stok = 0;
            file.ignore(1000, '\n');

            if (!(file >> buku.totalTerjual)) buku.totalTerjual = 0;
            file.ignore(1000, '\n');

            // Masukkan ke Linked List
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

    // SIMPAN FORMAT TURUN KE BAWAH (NEWLINE)
    void simpanKeFile() {
        ofstream file(NAMA_FILE_BUKU, ios::trunc);
        NodeBuku* temp = head;
        while (temp != nullptr) {
            file << temp->data.id << endl;
            file << temp->data.judul << endl;
            file << temp->data.penulis << endl;
            file << temp->data.tahun << endl;
            file << temp->data.isbn << endl;
            file << temp->data.kategori << endl;
            file << temp->data.status << endl;
            file << temp->data.harga << endl;
            file << temp->data.stok << endl;
            file << temp->data.totalTerjual << endl;
            temp = temp->next;
        }
        file.close();
    }

public:
    ManajemenBuku() {
        head = nullptr;
        muatDariFile();
    }

    ~ManajemenBuku() {
        simpanKeFile();
        while (head != nullptr) {
            NodeBuku* temp = head;
            head = head->next;
            delete temp;
        }
    }

    void tambahbuku() {
        int jumlah;
        cout << "============= Tambah Buku =============" << endl;
        cout << "Masukkan jumlah buku yang akan ditambahkan: ";
        if (!(cin >> jumlah)) {
            cout << "Input tidak valid.\n";
            cin.clear(); cin.ignore(1000, '\n'); return;
        }
        
        if (jumlah <= 0) {
            cout << "Jumlah harus lebih dari 0.\n"; return;
        }

        for (int i = 0; i < jumlah; i++) {
            Buku bukuBaru;
            cout << "\nData buku ke-" << (i + 1) << ":" << endl;

            cout << "Masukkan ID Buku (Angka): ";
            while (!(cin >> bukuBaru.id)) {
                cout << "ID harus angka. Coba lagi: "; cin.clear(); cin.ignore(1000, '\n');
            }
            cin.ignore(1000, '\n'); // Bersihkan enter

            cout << "Masukkan Judul Buku: ";
            getline(cin, bukuBaru.judul);

            cout << "Masukkan Penulis Buku: ";
            getline(cin, bukuBaru.penulis);

            cout << "Masukkan Tahun Terbit: ";
            while (!(cin >> bukuBaru.tahun)) {
                cout << "Tahun harus angka: "; cin.clear(); cin.ignore(1000, '\n');
            }
            cin.ignore(1000, '\n');

            cout << "Masukkan ISBN: ";
            getline(cin, bukuBaru.isbn);

            cout << "Masukkan Kategori: ";
            getline(cin, bukuBaru.kategori);

            cout << "Masukkan Harga Buku (Rp): ";
            while (!(cin >> bukuBaru.harga) || bukuBaru.harga < 0) {
                cout << "Harga valid: "; cin.clear(); cin.ignore(1000, '\n');
            }
            cin.ignore(1000, '\n');

            cout << "Masukkan Stok Awal: ";
            while (!(cin >> bukuBaru.stok) || bukuBaru.stok < 0) {
                cout << "Stok valid: "; cin.clear(); cin.ignore(1000, '\n');
            }
            cin.ignore(1000, '\n');

            bukuBaru.status = (bukuBaru.stok > 0) ? "Tersedia" : "Habis";
            bukuBaru.totalTerjual = 0;

            NodeBuku* nodeBaru = new NodeBuku;
            nodeBaru->data = bukuBaru;
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
            cout << "-> Buku berhasil ditambahkan." << endl;
        }
        simpanKeFile(); 
        cout << "\n[SUKSES] Semua data buku berhasil disimpan." << endl;
    }

    void lihatbuku() {
        if (head == nullptr) {
            cout << "Tidak ada data buku tersedia.\n";
            return;
        }

        cout << "\n===============================================" << endl;
        cout << "                  DAFTAR BUKU                  " << endl;
        cout << "===============================================" << endl;
        
        NodeBuku* temp = head;
        int counter = 1;
        
        while (temp != nullptr) {
            cout << "Buku #" << counter << endl;
            cout << "   ID            : " << temp->data.id << endl;
            cout << "   Judul         : " << temp->data.judul << endl;
            cout << "   Penulis       : " << temp->data.penulis << endl;
            cout << "   Tahun Terbit  : " << temp->data.tahun << endl;
            cout << "   ISBN          : " << temp->data.isbn << endl;
            cout << "   Kategori      : " << temp->data.kategori << endl;
            cout << "   Harga         : Rp " << (long)temp->data.harga << endl;
            cout << "   Stok          : " << temp->data.stok << " unit" << endl;
            cout << "   Total Terjual : " << temp->data.totalTerjual << " unit" << endl;
            cout << "   Status        : " << temp->data.status << endl;
            cout << "-----------------------------------------------" << endl;
            
            temp = temp->next;
            counter++;
        }
    }

    void hapusbuku() {
        int idHapus;
        cout << "Masukkan ID Buku yang akan dihapus: ";
        cin >> idHapus;

        if (head == nullptr) {
            cout << "Data kosong." << endl; return;
        }

        if (head->data.id == idHapus) {
            NodeBuku* temp = head;
            head = head->next;
            delete temp;
            simpanKeFile();
            cout << "Buku berhasil dihapus." << endl;
            return;
        }

        NodeBuku* temp = head;
        while (temp->next != nullptr && temp->next->data.id != idHapus) {
            temp = temp->next;
        }

        if (temp->next == nullptr) {
            cout << "ID tidak ditemukan." << endl;
        } else {
            NodeBuku* nodeHapus = temp->next;
            temp->next = nodeHapus->next;
            delete nodeHapus;
            simpanKeFile();
            cout << "Buku berhasil dihapus." << endl;
        }
    }

    void kembalikanBuku() {
        int idKembali;
        cout << "Masukkan ID Buku yang dikembalikan: ";
        cin >> idKembali;

        NodeBuku* temp = head;
        while(temp != nullptr){
            if(temp->data.id == idKembali){
                if(temp->data.status == "Dipinjam"){
                     temp->data.status = "Tersedia";
                     // temp->data.stok++; // Opsional: Tambah stok
                     simpanKeFile();
                     cout << "Buku berhasil dikembalikan.\n";
                } else {
                    cout << "Buku statusnya tidak sedang dipinjam.\n";
                }
                return;
            }
            temp = temp->next;
        }
        cout << "ID Buku tidak ditemukan.\n";
    }
};

// ===================== IMPLEMENTASI MENU =====================
void MenuKaryawan::tampilkanMenu() {
    int pilihan;
    
    // Instansiasi di sini agar data dimuat saat menu dibuka
    ManajemenBuku* manajemenBuku = new ManajemenBuku();
    AbsenKaryawan* absenKaryawan = new AbsenKaryawan(); 

    do {
        cout << "\n========== MENU Karyawan njay ==========\n";
        cout << "1. Tambah Data Buku\n";
        cout << "2. Lihat Data Buku\n";
        cout << "3. Hapus Data Buku\n";
        cout << "4. Kembalikan Buku (Update Status)\n";
        cout << "5. Absen Masuk\n";
        cout << "6. Absen Keluar\n";
        cout << "7. Lihat Riwayat Absen\n";
        cout << "8. Buat Laporan Harian\n";  // NEW
        cout << "0. Logout\n";
        cout << "Pilih menu: ";

        while (!(cin >> pilihan)) {
            cout << "Input angka: ";
            cin.clear(); cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n'); 

        switch (pilihan) {
            case 1: manajemenBuku->tambahbuku(); break;
            case 2: manajemenBuku->lihatbuku(); break;
            case 3: manajemenBuku->hapusbuku(); break;
            case 4: manajemenBuku->kembalikanBuku(); break;
            case 5: absenKaryawan->absenMasuk(); break;
            case 6: absenKaryawan->absenKeluar(); break;
            case 7: {
                int id;
                cout << "Masukkan ID Karyawan: "; cin >> id;
                absenKaryawan->tampilAbsenSendiri(id);
                break;
            }
            case 8:
                buatLaporanHarian("", "");  // User will input manually
                break;
            case 0: cout << "Logout berhasil!\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);
    
    // Cleanup - destructor akan dipanggil otomatis dan menyimpan data
    delete manajemenBuku;
    delete absenKaryawan;
}