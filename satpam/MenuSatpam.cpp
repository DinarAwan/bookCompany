#include "MenuSatpam.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime> // untuk mendapatkan waktu saat ini
using namespace std;

// ============================================================================
//              KONSTANTA UNTUK FILE PENYIMPANAN DATA SATPAM
// ============================================================================
const string FILE_TUGAS_SATPAM       = "tugas_satpam.txt";
const string FILE_ABSEN_SATPAM       = "absen_satpam.txt";
const string FILE_LAPORAN_SATPAM     = "laporan_satpam.txt";
const string FILE_TUGAS_SELESAI      = "tugas_selesai.txt";
const string FILE_PENGUNJUNG         = "pengunjung.txt";
const string FILE_KENDARAAN          = "log_kendaraan.txt";
const string FILE_BARANG             = "barang_hilang_ditemukan.txt";
const string FILE_PROFIL             = "profil_satpam.txt";
const string FILE_LAPORAN_HARIAN     = "laporan_harian.txt";

// ============================================================================
//                  STRUKTUR DATA LINKED LIST UNTUK PENGUNJUNG
// ============================================================================

// Struktur Visitor untuk menyimpan data satu pengunjung
struct Visitor {
    string nama;      // nama pengunjung
    string tujuan;    // tujuan datang
    string waktu;     // waktu kunjungan
    Visitor *next;    // pointer untuk menunjuk ke data berikutnya
};

// Fungsi untuk membuat node baru (satu data pengunjung)
Visitor* buatNode(string nama, string tujuan, string waktu) {
    // buat node baru secara dinamis (pakai new)
    Visitor *baru = new Visitor;
    baru->nama = nama;
    baru->tujuan = tujuan;
    baru->waktu = waktu;
    baru->next = NULL; // awalnya node baru tidak menunjuk ke siapa-siapa
    return baru;
}

// Fungsi untuk menambahkan data pengunjung ke akhir linked list
void tambahDiAkhir(Visitor* &head, string nama, string tujuan, string waktu) {
    Visitor *baru = buatNode(nama, tujuan, waktu); // buat node baru
    if (head == NULL) {
        // jika list masih kosong, node baru jadi head
        head = baru;
    } else {
        // kalau sudah ada data, kita cari node terakhir
        Visitor *temp = head;
        while (temp->next != NULL) {
            temp = temp->next; // jalan terus ke node berikutnya
        }
        temp->next = baru; // sambungkan node terakhir ke node baru
    }
}

// Fungsi untuk membaca data pengunjung dari file dan membentuk linked list
Visitor* bacaDariFile() {
    ifstream file(FILE_PENGUNJUNG); // buka file pengunjung
    if (!file.is_open()) {
        return NULL; // kalau file belum ada, kembalikan NULL
    }

    Visitor *head = NULL;
    string baris;

    // baca file baris per baris
    while (getline(file, baris)) {
        if (baris.empty()) continue; // lewati baris kosong

        // cari posisi pemisah " | "
        size_t p1 = baris.find(" | ");
        size_t p2 = baris.find(" | ", p1 + 3);

        if (p1 == string::npos || p2 == string::npos) continue;

        // potong string berdasarkan posisi pemisah
        string nama = baris.substr(0, p1);
        string tujuan = baris.substr(p1 + 3, p2 - (p1 + 3));
        string waktu = baris.substr(p2 + 3);

        // tambahkan data ke linked list
        tambahDiAkhir(head, nama, tujuan, waktu);
    }

    file.close();
    return head;
}

// Fungsi untuk menyimpan data pengunjung dari linked list ke file
bool simpanKeFile(Visitor *head) {
    ofstream file(FILE_PENGUNJUNG); // mode overwrite (menimpa)
    if (!file.is_open()) {
        cout << "Gagal menyimpan data ke file pengunjung.txt\n";
        return false;
    }

    Visitor *temp = head;
    while (temp != NULL) {
        file << temp->nama << " | " << temp->tujuan << " | " << temp->waktu << endl;
        temp = temp->next;
    }

    file.close();
    return true;
}

// Fungsi untuk membersihkan seluruh linked list dari memori
void hapusSemua(Visitor *&head) {
    while (head != NULL) {
        Visitor *hapus = head;
        head = head->next;
        delete hapus;
    }
}

// ============================================================================
//                  IMPLEMENTASI CLASS MenuSatpam
// ============================================================================

// Fungsi utama untuk menampilkan semua menu satpam
void MenuSatpam::tampilkanMenu() {
    int pilihan = -1;

    // selama user belum memilih keluar
    while (pilihan != 0) {
        cout << "\n=================================================\n";
        cout << "               MENU SATPAM GEDUNG                \n";
        cout << "=================================================\n";
        cout << "1. Lihat Daftar Tugas\n";
        cout << "2. Absen Kehadiran\n";
        cout << "3. Lapor Kejadian\n";
        cout << "4. Tandai Tugas Selesai\n";
        cout << "5. Data Pengunjung (CRUD)\n";
        cout << "6. Catat Kendaraan Masuk/Keluar\n";
        cout << "7. Catat Barang Hilang/Ditemukan\n";
        cout << "8. Laporan Harian\n";
        cout << "9. Pengaturan Profil Satpam\n";
        cout << "0. Logout\n";
        cout << "=================================================\n";
        cout << "Masukkan pilihan Anda: ";
        cin >> pilihan;

        // cek pilihan user dan panggil fungsi sesuai menu
        if (pilihan == 1) {
            lihatTugas();
        } 
        else if (pilihan == 2) {
            absenKehadiran();
        } 
        else if (pilihan == 3) {
            laporKejadian();
        } 
        else if (pilihan == 4) {
            selesaikanTugas();
        } 
        else if (pilihan == 5) {
            int sub;
            cout << "\n--- MENU DATA PENGUNJUNG ---\n";
            cout << "1. Tambah Pengunjung Baru\n";
            cout << "2. Lihat Semua Pengunjung\n";
            cout << "3. Ubah Data Pengunjung\n";
            cout << "4. Hapus Data Pengunjung\n";
            cout << "Pilih: ";
            cin >> sub;

            if (sub == 1) catatPengunjung();
            else if (sub == 2) tampilkanPengunjung();
            else if (sub == 3) ubahPengunjung();
            else if (sub == 4) hapusPengunjung();
            else cout << "Sub-menu tidak valid!\n";
        } 
        else if (pilihan == 6) {
            logKendaraan();
        } 
        else if (pilihan == 7) {
            catatBarang();
        } 
        else if (pilihan == 8) {
            laporanHarian();
        } 
        else if (pilihan == 9) {
            pengaturanProfil();
        } 
        else if (pilihan == 0) {
            cout << "Logout berhasil. Terima kasih!\n";
        } 
        else {
            cout << "Pilihan tidak dikenal. Silakan coba lagi.\n";
        }
    }
}

// ============================================================================
//                      FITUR - FITUR MENU SATPAM
// ============================================================================

// Menampilkan daftar tugas
void MenuSatpam::lihatTugas() {
    ifstream file(FILE_TUGAS_SATPAM);
    if (!file.is_open()) {
        cout << "Belum ada file tugas. Satpam belum punya tugas.\n";
        return;
    }

    cout << "\n=== DAFTAR TUGAS SATPAM ===\n";
    string baris;
    while (getline(file, baris)) {
        cout << "- " << baris << endl;
    }
    file.close();
}

// Mencatat absen ke file
void MenuSatpam::absenKehadiran() {
    ofstream file(FILE_ABSEN_SATPAM, ios::app);
    string nama;

    cout << "\nMasukkan nama lengkap satpam: ";
    cin.ignore();
    getline(cin, nama);

    time_t now = time(0);
    file << nama << " hadir pada " << ctime(&now);
    file.close();

    cout << "✅ Absen berhasil dicatat!\n";
}

// Mencatat laporan kejadian
void MenuSatpam::laporKejadian() {
    ofstream file(FILE_LAPORAN_SATPAM, ios::app);
    string kejadian;

    cout << "\nMasukkan laporan kejadian (contoh: Orang mencurigakan di parkiran): ";
    cin.ignore();
    getline(cin, kejadian);

    file << kejadian << endl;
    file.close();

    cout << "✅ Laporan berhasil disimpan ke file.\n";
}

// Tandai tugas selesai
void MenuSatpam::selesaikanTugas() {
    ofstream file(FILE_TUGAS_SELESAI, ios::app);
    string tugas;

    cout << "\nMasukkan nama tugas yang telah selesai: ";
    cin.ignore();
    getline(cin, tugas);

    file << "Tugas '" << tugas << "' selesai pada " << __DATE__ << " " << __TIME__ << endl;
    file.close();

    cout << "✅ Tugas telah ditandai selesai.\n";
}

// CRUD - Tambah Pengunjung
void MenuSatpam::catatPengunjung() {
    Visitor *head = bacaDariFile();
    string nama, tujuan;

    cout << "\nNama pengunjung: ";
    cin.ignore();
    getline(cin, nama);

    cout << "Tujuan datang: ";
    getline(cin, tujuan);

    time_t now = time(0);
    string waktu = ctime(&now);

    tambahDiAkhir(head, nama, tujuan, waktu);
    simpanKeFile(head);
    hapusSemua(head);

    cout << "✅ Data pengunjung baru berhasil disimpan.\n";
}

// CRUD - Lihat Pengunjung
void MenuSatpam::tampilkanPengunjung() {
    Visitor *head = bacaDariFile();
    if (!head) {
        cout << "Belum ada data pengunjung.\n";
        return;
    }

    int i = 1;
    Visitor *temp = head;
    cout << "\n=== DAFTAR PENGUNJUNG ===\n";

    while (temp) {
        cout << i++ << ". Nama: " << temp->nama << ", Tujuan: " << temp->tujuan
             << ", Waktu: " << temp->waktu << endl;
        temp = temp->next;
    }

    hapusSemua(head);
}

// CRUD - Ubah Pengunjung
void MenuSatpam::ubahPengunjung() {
    Visitor *head = bacaDariFile();
    tampilkanPengunjung();

    cout << "\nMasukkan nomor pengunjung yang ingin diubah: ";
    int no;
    cin >> no;
    cin.ignore();

    int i = 1;
    Visitor *temp = head;
    while (temp && i < no) {
        temp = temp->next;
        i++;
    }

    if (!temp) {
        cout << "Nomor tidak ditemukan!\n";
    } else {
        cout << "Masukkan nama baru: ";
        getline(cin, temp->nama);
        cout << "Masukkan tujuan baru: ";
        getline(cin, temp->tujuan);
        simpanKeFile(head);
        cout << "✅ Data pengunjung berhasil diperbarui.\n";
    }

    hapusSemua(head);
}

// CRUD - Hapus Pengunjung
void MenuSatpam::hapusPengunjung() {
    Visitor *head = bacaDariFile();
    tampilkanPengunjung();

    cout << "\nMasukkan nomor pengunjung yang ingin dihapus: ";
    int no;
    cin >> no;

    Visitor *temp = head;
    Visitor *prev = NULL;
    int i = 1;

    while (temp && i < no) {
        prev = temp;
        temp = temp->next;
        i++;
    }

    if (!temp) {
        cout << "Nomor pengunjung tidak ditemukan.\n";
    } else {
        if (!prev) head = temp->next;
        else prev->next = temp->next;
        delete temp;
        simpanKeFile(head);
        cout << "✅ Data pengunjung berhasil dihapus.\n";
    }

    hapusSemua(head);
}

// Catat Kendaraan Masuk / Keluar
void MenuSatpam::logKendaraan() {
    ofstream file(FILE_KENDARAAN, ios::app);
    string plat, status;

    cout << "\nMasukkan plat nomor kendaraan: ";
    cin.ignore();
    getline(cin, plat);

    cout << "Masuk atau Keluar?: ";
    getline(cin, status);

    time_t now = time(0);
    file << plat << " - " << status << " - " << ctime(&now);
    file.close();

    cout << "✅ Data kendaraan tersimpan.\n";
}

// Catat Barang Hilang / Ditemukan
void MenuSatpam::catatBarang() {
    ofstream file(FILE_BARANG, ios::app);
    string jenis, lokasi, status;

    cout << "\nMasukkan nama/jenis barang: ";
    cin.ignore();
    getline(cin, jenis);

    cout << "Lokasi barang ditemukan/hilang: ";
    getline(cin, lokasi);

    cout << "Status barang (hilang/ditemukan): ";
    getline(cin, status);

    file << "Barang: " << jenis << ", Lokasi: " << lokasi << ", Status: " << status << endl;
    file.close();

    cout << "✅ Data barang berhasil disimpan.\n";
}

// Laporan Harian Satpam
void MenuSatpam::laporanHarian() {
    ofstream file(FILE_LAPORAN_HARIAN, ios::app);
    string laporan;

    cout << "\nTuliskan laporan kegiatan harian satpam: ";
    cin.ignore();
    getline(cin, laporan);

    file << laporan << endl;
    file.close();

    cout << "✅ Laporan harian tersimpan.\n";
}

// Profil Satpam
void MenuSatpam::pengaturanProfil() {
    ofstream file(FILE_PROFIL);
    string nama, shift, kontak;

    cout << "\nMasukkan nama lengkap: ";
    cin.ignore();
    getline(cin, nama);

    cout << "Shift bertugas (pagi/siang/malam): ";
    getline(cin, shift);

    cout << "Nomor kontak (WA/HP): ";
    getline(cin, kontak);

    file << "Nama: " << nama << endl;
    file << "Shift: " << shift << endl;
    file << "Kontak: " << kontak << endl;
    file.close();

    cout << "✅ Profil satpam berhasil disimpan.\n";
}
