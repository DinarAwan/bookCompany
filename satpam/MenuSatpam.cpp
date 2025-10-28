#include "MenuSatpam.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>

using namespace std;

const string FILE_TUGAS_SATPAM = "tugas_satpam.txt";
const string FILE_ABSEN_SATPAM = "absen_satpam.txt";
const string FILE_LAPORAN_SATPAM = "laporan_satpam.txt";
const string FILE_TUGAS_SELESAI = "tugas_selesai.txt";
const string FILE_PENGUNJUNG = "pengunjung.txt";

// Struktur node linked list untuk menyimpan data pengunjung di memori
struct Visitor {
    std::string nama;
    std::string tujuan;
    std::string waktu;
    Visitor* next;
    Visitor(const std::string& n, const std::string& t, const std::string& w)
        : nama(n), tujuan(t), waktu(w), next(nullptr) {}
};

// Fungsi util untuk load dari file ke linked list (mengembalikan hea
static Visitor* loadPengunjungFromFile() {
    std::ifstream file(FILE_PENGUNJUNG);
    if (!file.is_open()) return nullptr;

    Visitor* head = nullptr;
    Visitor* tail = nullptr;
    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;

        // Format: nama | tujuan | waktu
        size_t p1 = line.find(" | ");
        size_t p2 = std::string::npos;
        if (p1 != std::string::npos) {
            p2 = line.find(" | ", p1 + 3);
        }

        std::string nama, tujuan, waktu;
        if (p1 != std::string::npos && p2 != std::string::npos) {
            nama = line.substr(0, p1);
            tujuan = line.substr(p1 + 3, p2 - (p1 + 3));
            waktu = line.substr(p2 + 3);
        } else {
            // Jika format tidak sesuai, simpan seluruh baris sebagai tujuan agar tidak hilang
            nama = line;
            tujuan = "";
            waktu = "";
        }

        Visitor* node = new Visitor(nama, tujuan, waktu);
        if (!head) head = tail = node;
        else { tail->next = node; tail = node; }
    }
    file.close();
    return head;
}

// Simpan seluruh linked list ke file (overwrite)
static bool savePengunjungToFile(Visitor* head) {
    std::ofstream file(FILE_PENGUNJUNG, std::ios::trunc);
    if (!file.is_open()) return false;
    Visitor* cur = head;
    while (cur) {
        file << cur->nama << " | " << cur->tujuan << " | " << cur->waktu << std::endl;
        cur = cur->next;
    }
    file.close();
    return true;
}

// Bebaskan memori linked list
static void freePengunjungList(Visitor* head) {
    while (head) {
        Visitor* next = head->next;
        delete head;
        head = next;
    }
}

void MenuSatpam::tampilkanMenu() {
    int pilihan;
    do {
        cout << "\n========== MENU SATPAM ==========" << endl;
        cout << "1. Lihat Daftar Tugas" << endl;
        cout << "2. Absen Kehadiran" << endl;
        cout << "3. Lapor Kejadian" << endl;
        cout << "4. Tandai Tugas Selesai" << endl;
        cout << "5. Catat Pengunjung" << endl;
        cout << "0. Logout" << endl;
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                lihatTugas();
                break;
            case 2:
                absenKehadiran();
                break;
            case 3:
                laporKejadian();
                break;
            case 4:
                selesaikanTugas();
                break;
            case 5:
                catatPengunjung();
                break;
            case 0:
                cout << "Logout berhasil!" << endl;
                break;
            default:
                cout << "Pilihan tidak valid!" << endl;
        }
        cout << endl;
    } while (pilihan != 0);
}

// ====================================================
// (1) Lihat Daftar Tugas
// ====================================================
void MenuSatpam::lihatTugas() {
    ifstream file(FILE_TUGAS_SATPAM);
    if (!file.is_open()) {
        cout << "Belum ada tugas yang ditambahkan oleh Admin." << endl;
        return;
    }

    string line;
    cout << "\n--- Daftar Tugas Anda ---" << endl;
    bool adaTugas = false;

    while (getline(file, line)) {
        adaTugas = true;
        stringstream ss(line);
        string id, deskripsi;

        getline(ss, id, ',');
        getline(ss, deskripsi);

        cout << "ID: " << id << " | Tugas: " << deskripsi << endl;
    }

    if (!adaTugas) {
        cout << "Belum ada tugas yang ditambahkan oleh Admin." << endl;
    }
    file.close();
}

// ====================================================
// (2) Absen Kehadiran
// ====================================================
void MenuSatpam::absenKehadiran() {
    ofstream file(FILE_ABSEN_SATPAM, ios::app);
    string nama;
    cout << "Masukkan nama Anda: ";
    cin >> ws;
    getline(cin, nama);
    file << nama << " hadir pada " << __DATE__ << " " << __TIME__ << endl;
    cout << "Kehadiran tercatat!" << endl;
    file.close();
}

// ====================================================
// (3) Lapor Kejadian
// ====================================================
void MenuSatpam::laporKejadian() {
    ofstream file(FILE_LAPORAN_SATPAM, ios::app);
    string laporan;
    cout << "Masukkan laporan kejadian: ";
    cin >> ws;
    getline(cin, laporan);
    file << laporan << endl;
    cout << "Laporan berhasil disimpan!" << endl;
    file.close();
}

// ====================================================
// (4) Tandai Tugas Selesai
// ====================================================
void MenuSatpam::selesaikanTugas() {
    string id;
    cout << "Masukkan ID tugas yang sudah selesai: ";
    cin >> ws;
    getline(cin, id);

    // Simpan ID tugas yang selesai
    ofstream file(FILE_TUGAS_SELESAI, ios::app);
    file << id << endl;
    file.close();

    cout << "Tugas dengan ID " << id << " telah ditandai selesai." << endl;
}

// ====================================================
// (5) Catat Pengunjung
// ====================================================
void MenuSatpam::catatPengunjung() {
    // Muat daftar pengunjung saat ini ke linked list
    Visitor* head = loadPengunjungFromFile();

    string nama, tujuan;
    cout << "Masukkan nama pengunjung: ";
    cin >> ws;
    getline(cin, nama);
    cout << "Masukkan tujuan/keperluan: ";
    getline(cin, tujuan);

    // Ambil waktu sekarang
    time_t t = time(nullptr);
    struct tm *tm_info = localtime(&t);
    char waktu_buf[64];
    strftime(waktu_buf, sizeof(waktu_buf), "%Y-%m-%d %H:%M:%S", tm_info);

    // Buat node baru dan tambahkan di akhir linked list
    Visitor* node = new Visitor(nama, tujuan, waktu_buf);
    if (!head) {
        head = node;
    } else {
        Visitor* cur = head;
        while (cur->next) cur = cur->next;
        cur->next = node;
    }

    // Simpan kembali seluruh linked list ke file
    if (!savePengunjungToFile(head)) {
        cout << "Gagal menyimpan data pengunjung ke file." << endl;
    } else {
        cout << "Catatan pengunjung tersimpan: " << nama << " pada " << waktu_buf << endl;
    }

    // Bebaskan memori
    freePengunjungList(head);
}
