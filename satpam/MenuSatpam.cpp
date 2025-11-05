#include "MenuSatpam.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>       // untuk mendapatkan waktu saat ini
#include <cstdlib>     // fungsi utilitas
#include <iomanip>     // format output waktu
using namespace std;

// ============================================================================
//                      KONSTANTA UNTUK FILE SATPAM
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
const string FILE_EMERGENCY_LOG      = "emergency_log.txt";

// ============================================================================
//                     STRUKTUR DATA LINKED LIST
// ============================================================================

struct Visitor {

    string nama;
    string tujuan;
    string waktu;

    Visitor *next;
};

// ============================================================================
//                      FUNGSI PENDUKUNG LINKED LIST
// ============================================================================

// Buat node baru
Visitor* buatNode(string nama, string tujuan, string waktu) {

    Visitor *baru = new Visitor;

    baru->nama   = nama;
    baru->tujuan = tujuan;
    baru->waktu  = waktu;

    baru->next   = NULL;

    return baru;
}

// Tambah di akhir
void tambahDiAkhir(Visitor* &head, string nama, string tujuan, string waktu) {

    Visitor *baru = buatNode(nama, tujuan, waktu);

    if (head == NULL) {

        head = baru;

    } else {

        Visitor *temp = head;

        while (temp->next != NULL) {
            temp = temp->next;
        }

        temp->next = baru;
    }
}

// Baca dari file
Visitor* bacaDariFile() {

    ifstream file(FILE_PENGUNJUNG);

    if (!file.is_open()) {
        return NULL;
    }

    Visitor *head = NULL;
    string baris;

    while (getline(file, baris)) {

        if (baris.empty()) continue;

        size_t p1 = baris.find(" | ");
        size_t p2 = baris.find(" | ", p1 + 3);

        if (p1 == string::npos || p2 == string::npos) continue;

        string nama   = baris.substr(0, p1);
        string tujuan = baris.substr(p1 + 3, p2 - (p1 + 3));
        string waktu  = baris.substr(p2 + 3);

        tambahDiAkhir(head, nama, tujuan, waktu);
    }

    file.close();
    return head;
}

// Simpan ke file
bool simpanKeFile(Visitor *head) {

    ofstream file(FILE_PENGUNJUNG);

    if (!file.is_open()) {

        cout << "Gagal menyimpan data ke file pengunjung.txt\n";
        return false;
    }

    Visitor *temp = head;

    while (temp != NULL) {

        file << temp->nama
             << " | " << temp->tujuan
             << " | " << temp->waktu
             << endl;

        temp = temp->next;
    }

    file.close();
    return true;
}

// Hapus semua node
void hapusSemua(Visitor *&head) {

    while (head != NULL) {

        Visitor *hapus = head;
        head = head->next;

        delete hapus;
    }
}

// ============================================================================
//                     IMPLEMENTASI CLASS MenuSatpam
// ============================================================================

void MenuSatpam::tampilkanMenu() {

    int pilihan = -1;

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
        cout << "10.emergeny Call\n";
        cout << "0. Logout\n";
        cout << "=================================================\n";

        cout << "Masukkan pilihan Anda: ";
        cin >> pilihan;

        if (pilihan == 1) {

            lihatTugas();

        } else if (pilihan == 2) {

            absenKehadiran();

        } else if (pilihan == 3) {

            laporKejadian();

        } else if (pilihan == 4) {

            selesaikanTugas();

        } else if (pilihan == 5) {

            int sub;

            cout << "\n--- MENU DATA PENGUNJUNG ---\n";
            cout << "1. Tambah Pengunjung Baru\n";
            cout << "2. Lihat Semua Pengunjung\n";
            cout << "3. Ubah Data Pengunjung\n";
            cout << "4. Hapus Data Pengunjung\n";
            cout << "Pilih: ";

            cin >> sub;

            if      (sub == 1) catatPengunjung();
            else if (sub == 2) tampilkanPengunjung();
            else if (sub == 3) ubahPengunjung();
            else if (sub == 4) hapusPengunjung();
            else               cout << "Sub-menu tidak valid!\n";

        } else if (pilihan == 6) {

            logKendaraan();

        } else if (pilihan == 7) {

            catatBarang();

        } else if (pilihan == 8) {

            laporanHarian();

        } else if (pilihan == 9) {

            pengaturanProfil();

        }else if(pilihan ==10){

            modeDarurat();

        } else if (pilihan == 0) {

            cout << "Logout berhasil. Terima kasih!\n";

        } else {

            cout << "Pilihan tidak dikenal. Silakan coba lagi.\n";
        }
    }
}

// ============================================================================
//                       FITUR - FITUR MENU
// ============================================================================

void MenuSatpam::lihatTugas() {

    ifstream file(FILE_TUGAS_SATPAM);

    if (!file.is_open()) {
        cout << "Belum ada file tugas.\n";
        return;
    }

    cout << "\n=== DAFTAR TUGAS ===\n";

    string baris;

    while (getline(file, baris)) {
        cout << "- " << baris << endl;
    }

    file.close();
}

void MenuSatpam::absenKehadiran() {

    ofstream file(FILE_ABSEN_SATPAM, ios::app);

    string nama;

    cout << "\nMasukkan nama lengkap satpam: ";
    cin.ignore();
    getline(cin, nama);

    time_t now = time(0);
    string waktu = ctime(&now);

    file << nama << " hadir pada " << waktu;
    file.close();

    cout << "✅ Absen berhasil dicatat!\n";
}

void MenuSatpam::laporKejadian() {

    ofstream file(FILE_LAPORAN_SATPAM, ios::app);
    string kejadian;

    cout << "\nMasukkan laporan kejadian: ";
    cin.ignore();
    getline(cin, kejadian);

    file << kejadian << endl;
    file.close();

    cout << "✅ Laporan berhasil disimpan.\n";
}

void MenuSatpam::selesaikanTugas() {

    ofstream file(FILE_TUGAS_SELESAI, ios::app);
    string tugas;

    cout << "\nMasukkan tugas yang selesai: ";
    cin.ignore();
    getline(cin, tugas);

    file << "Tugas '" << tugas << "' selesai pada "
         << __DATE__ << " " << __TIME__ << endl;

    file.close();

    cout << "✅ Tugas telah ditandai selesai.\n";
}

// ============================================================================
//                       CRUD PENGUNJUNG
// ============================================================================

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

void MenuSatpam::tampilkanPengunjung() {

    Visitor *head = bacaDariFile();

    if (!head) {
        cout << "Belum ada data pengunjung.\n";
        return;
    }

    cout << "\n=== DAFTAR PENGUNJUNG ===\n";

    int i = 1;
    Visitor *temp = head;

    while (temp) {

        cout << i++ << ". Nama: " << temp->nama
             << ", Tujuan: " << temp->tujuan
             << ", Waktu: " << temp->waktu << endl;

        temp = temp->next;
    }

    hapusSemua(head);
}

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

        cout << "✅ Data pengunjung diperbarui.\n";
    }

    hapusSemua(head);
}

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

        if (!prev)
            head = temp->next;
        else
            prev->next = temp->next;

        delete temp;

        simpanKeFile(head);

        cout << "✅ Data pengunjung dihapus.\n";
    }

    hapusSemua(head);
}

// ============================================================================
//                         FITUR LAINNYA
// ============================================================================

void MenuSatpam::logKendaraan() {

    ofstream file(FILE_KENDARAAN, ios::app);

    string plat, status;

    cout << "\nMasukkan plat nomor kendaraan: ";
    cin.ignore();
    getline(cin, plat);

    cout << "Masuk atau Keluar?: ";
    getline(cin, status);

    time_t now = time(0);
    string waktu = ctime(&now);

    file << plat << " - " << status << " - " << waktu;
    file.close();

    cout << "✅ Data kendaraan tersimpan.\n";
}

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

    file << "Barang: " << jenis
         << ", Lokasi: " << lokasi
         << ", Status: " << status << endl;

    file.close();

    cout << "✅ Data barang berhasil disimpan.\n";
}

void MenuSatpam::laporanHarian() {

    ofstream file(FILE_LAPORAN_HARIAN, ios::app);

    string laporan;

    cout << "\nTuliskan laporan kegiatan harian: ";
    cin.ignore();
    getline(cin, laporan);

    file << laporan << endl;
    file.close();

    cout << "✅ Laporan harian tersimpan.\n";
}

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
void MenuSatpam::modeDarurat() {
    ofstream file(FILE_EMERGENCY_LOG, ios::app);
    if (!file.is_open()) {
        cout << "❌ Gagal membuka file emergency_log.txt\n";
        return;
    }

    string pesan;
    time_t now = time(0);
    string waktu = ctime(&now);

    cout << "\n=== MODE DARURAT  ===\n";
    cout << "Masukkan deskripsi keadaan darurat: ";
    cin.ignore();
    getline(cin, pesan);

    file << "[EMERGENCY] " << waktu << " - " << pesan << endl;
    file.close();

    cout << "✅ Keadaan darurat berhasil dicatat di 'emergency_log.txt'.\n";
}
