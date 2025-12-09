#include "MenuSatpam.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>     // Standard C Library untuk fungsi waktu
#include <cstdlib>   // Standard C Library untuk utilitas
#include <iomanip>   // Untuk format output
using namespace std;

// ============================================================================
//                           KONSTANTA DEFINISI FILE LOG
// ============================================================================
const string FILE_TUGAS_SATPAM     = "tugas_satpam.txt";
const string FILE_ABSEN_SATPAM     = "absen_satpam.txt";
const string FILE_LAPORAN_SATPAM   = "laporan_satpam.txt";
const string FILE_TUGAS_SELESAI    = "tugas_selesai.txt";
const string FILE_PENGUNJUNG       = "pengunjung.txt";
const string FILE_KENDARAAN        = "log_kendaraan.txt";
const string FILE_BARANG           = "barang_hilang_ditemukan.txt";
const string FILE_PROFIL           = "profil_satpam.txt";
const string FILE_LAPORAN_HARIAN   = "laporan_harian.txt";
const string FILE_EMERGENCY_LOG    = "emergency_log.txt";
const string FILE_PATROLI          = "log_patroli.txt";

// ============================================================================
//                    STRUKTUR DATA LINKED LIST (SEMUA ENTITAS)
// ============================================================================

// 1. Struct Pengunjung (Visitor)
struct Visitor {
    string nama, tujuan, waktu;
    Visitor *next;
};

// 2. Struct Absensi
struct Absensi {
    string nama, status, waktu;
    Absensi *next;
};

// 3. Struct Tugas
struct Tugas {
    string deskripsi, waktu; // Waktu bisa kosong jika tugas statis
    Tugas *next;
};

// 4. Struct Laporan Insiden
struct Insiden {
    string waktu, lokasi, kejadian, tindakan;
    Insiden *next;
};

// 5. Struct Patroli
struct Patroli {
    string waktu, area, catatan;
    Patroli *next;
};

// 6. Struct Kendaraan
struct Kendaraan {
    string waktu, plat, jenis, status;
    Kendaraan *next;
};

// 7. Struct Barang
struct Barang {
    string waktu, jenis, deskripsi, lokasi, status;
    Barang *next;
};

// 8. Struct Laporan Harian & Emergency (Sederhana)
struct LogUmum {
    string waktu, isi;
    LogUmum *next;
};

// ============================================================================
//                    FUNGSI UTILITAS UMUM (HELPER)
// ============================================================================

string getCurrentTime() {
    time_t now = time(0);
    string waktu = ctime(&now);
    while (!waktu.empty() && (waktu.back() == '\n' || waktu.back() == '\r')) {
        waktu.pop_back();
    }
    return waktu;
}

void clearInput() {
    if (cin.peek() == '\n' || cin.peek() == EOF) {
        cin.ignore(1000, '\n');
    } else {
        cin.clear();
        cin.ignore(1000, '\n');
    }
}

// ============================================================================
//              FUNGSI HELPER LINKED LIST (Load & Delete)
// ============================================================================

// --- HELPER GENERIC UNTUK PARSING STRING "A | B | C" ---
// Fungsi ini memecah string berdasarkan delimiter '|' tanpa <vector>
string ambilToken(string &baris, int index) {
    stringstream ss(baris);
    string token;
    int current = 0;
    while (getline(ss, token, '|')) {
        // Hapus spasi di awal/akhir token manual
        size_t first = token.find_first_not_of(' ');
        if (string::npos == first) {
            if (current == index) return "";
        } else {
            size_t last = token.find_last_not_of(' ');
            if (current == index) return token.substr(first, (last - first + 1));
        }
        current++;
    }
    return "";
}

// ------------------- 1. PENGUNJUNG -------------------
Visitor* buatNodeVisitor(string nama, string tujuan, string waktu) {
    Visitor *baru = new Visitor;
    baru->nama = nama; baru->tujuan = tujuan; baru->waktu = waktu;
    baru->next = NULL;
    return baru;
}

void tambahVisitorAkhir(Visitor* &head, string nama, string tujuan, string waktu) {
    Visitor *baru = buatNodeVisitor(nama, tujuan, waktu);
    if (!head) head = baru;
    else {
        Visitor *temp = head;
        while (temp->next) temp = temp->next;
        temp->next = baru;
    }
}

void hapusListVisitor(Visitor *&head) {
    while (head) {
        Visitor *hapus = head;
        head = head->next;
        delete hapus;
    }
}

// ------------------- 2. ABSENSI -------------------
Absensi* loadAbsensi() {
    ifstream file(FILE_ABSEN_SATPAM);
    Absensi *head = NULL;
    string baris;
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        // Format: WAKTU | STATUS | NAMA
        string waktu = ambilToken(baris, 0);
        string status = ambilToken(baris, 1);
        string nama = ambilToken(baris, 2);
        
        Absensi *baru = new Absensi;
        baru->waktu = waktu; baru->status = status; baru->nama = nama; baru->next = NULL;
        
        if (!head) head = baru;
        else {
            Absensi *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = baru;
        }
    }
    file.close();
    return head;
}

void hapusListAbsensi(Absensi *&head) {
    while (head) { Absensi *t = head; head = head->next; delete t; }
}

// ------------------- 3. TUGAS (Umum & Selesai) -------------------
Tugas* loadTugas(string namaFile, bool isSelesai) {
    ifstream file(namaFile);
    Tugas *head = NULL;
    string baris;
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        Tugas *baru = new Tugas;
        if (isSelesai) {
            // Format: WAKTU | DESKRIPSI
            baru->waktu = ambilToken(baris, 0);
            baru->deskripsi = ambilToken(baris, 1);
        } else {
            // Format: DESKRIPSI (Tugas statis)
            baru->deskripsi = baris; 
            baru->waktu = "-";
        }
        baru->next = NULL;

        if (!head) head = baru;
        else {
            Tugas *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = baru;
        }
    }
    file.close();
    return head;
}
void hapusListTugas(Tugas *&head) {
    while (head) { Tugas *t = head; head = head->next; delete t; }
}

// ------------------- 4. PATROLI -------------------
Patroli* loadPatroli() {
    ifstream file(FILE_PATROLI);
    Patroli *head = NULL;
    string baris;
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        // Format: WAKTU | AREA | CATATAN
        Patroli *baru = new Patroli;
        baru->waktu = ambilToken(baris, 0);
        baru->area = ambilToken(baris, 1);
        baru->catatan = ambilToken(baris, 2);
        baru->next = NULL;

        if (!head) head = baru;
        else {
            Patroli *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = baru;
        }
    }
    file.close();
    return head;
}
void hapusListPatroli(Patroli *&head) {
    while (head) { Patroli *t = head; head = head->next; delete t; }
}

// ------------------- 5. KENDARAAN -------------------
Kendaraan* loadKendaraan() {
    ifstream file(FILE_KENDARAAN);
    Kendaraan *head = NULL;
    string baris;
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        // Format: WAKTU | PLAT | JENIS | STATUS
        Kendaraan *baru = new Kendaraan;
        baru->waktu = ambilToken(baris, 0);
        baru->plat = ambilToken(baris, 1);
        baru->jenis = ambilToken(baris, 2);
        baru->status = ambilToken(baris, 3);
        baru->next = NULL;

        if (!head) head = baru;
        else {
            Kendaraan *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = baru;
        }
    }
    file.close();
    return head;
}
void hapusListKendaraan(Kendaraan *&head) {
    while (head) { Kendaraan *t = head; head = head->next; delete t; }
}

// ------------------- 6. BARANG -------------------
Barang* loadBarang() {
    ifstream file(FILE_BARANG);
    Barang *head = NULL;
    string baris;
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        // Format: WAKTU | JENIS | DESKRIPSI | LOKASI | STATUS
        Barang *baru = new Barang;
        baru->waktu = ambilToken(baris, 0);
        baru->jenis = ambilToken(baris, 1);
        baru->deskripsi = ambilToken(baris, 2);
        baru->lokasi = ambilToken(baris, 3);
        baru->status = ambilToken(baris, 4);
        baru->next = NULL;

        if (!head) head = baru;
        else {
            Barang *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = baru;
        }
    }
    file.close();
    return head;
}
void hapusListBarang(Barang *&head) {
    while (head) { Barang *t = head; head = head->next; delete t; }
}

// ------------------- 7. LAPORAN INSIDEN -------------------
Insiden* loadInsiden() {
    ifstream file(FILE_LAPORAN_SATPAM);
    Insiden *head = NULL;
    string baris;
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        // Format: WAKTU | LOKASI | KEJADIAN | TINDAKAN
        Insiden *baru = new Insiden;
        baru->waktu = ambilToken(baris, 0);
        baru->lokasi = ambilToken(baris, 1);
        baru->kejadian = ambilToken(baris, 2);
        baru->tindakan = ambilToken(baris, 3);
        baru->next = NULL;

        if (!head) head = baru;
        else {
            Insiden *temp = head;
            while (temp->next) temp = temp->next;
            temp->next = baru;
        }
    }
    file.close();
    return head;
}
void hapusListInsiden(Insiden *&head) {
    while (head) { Insiden *t = head; head = head->next; delete t; }
}

// ------------------- 8. PENGUNJUNG (IMPLEMENTASI ULANG) -------------------
Visitor* bacaVisitorFile() {
    ifstream file(FILE_PENGUNJUNG);
    Visitor *head = NULL;
    if (!file.is_open()) return NULL;
    
    string baris;
    while (getline(file, baris)) {
        if (baris.empty()) continue;
        // Format: NAMA | TUJUAN | WAKTU
        string nama = ambilToken(baris, 0);
        string tujuan = ambilToken(baris, 1);
        string waktu = ambilToken(baris, 2);
        tambahVisitorAkhir(head, nama, tujuan, waktu);
    }
    file.close();
    return head;
}

bool simpanVisitorFile(Visitor *head) {
    ofstream file(FILE_PENGUNJUNG);
    if (!file.is_open()) return false;
    Visitor *temp = head;
    while (temp != NULL) {
        file << temp->nama << " | " << temp->tujuan << " | " << temp->waktu << endl;
        temp = temp->next;
    }
    file.close();
    return true;
}


// ============================================================================
//               IMPLEMENTASI CLASS MenuSatpam (LOGIC UTAMA)
// ============================================================================

void MenuSatpam::tampilkanMenu() {
    int pilihan = -1; 
    while (pilihan != 0) {
        cout << "\n=================================================\n";
        cout << "        SISTEM MANAJEMEN SATPAM GEDUNG           \n";
        cout << "=================================================\n";
        cout << " [1-5] TUGAS & KEHADIRAN\n";
        cout << " 1. Lihat Daftar Tugas (Aktif)\n";
        cout << " 2. Absen Kehadiran (Masuk/Pulang)\n";
        cout << " 3. Tandai Tugas Selesai\n";
        cout << " 4. Lihat Log Absen Kehadiran\n";
        cout << " 5. Lihat Riwayat Tugas Selesai\n";
        cout << "-------------------------------------------------\n";
        cout << " [6-12] OPERASI KEAMANAN & PELAPORAN\n";
        cout << " 6. Lapor Kejadian (Insiden)\n";
        cout << " 7. Catat Log Patroli Area\n";
        cout << " 8. Laporkan Kegiatan Harian\n";
        cout << " 9. Mode Panggilan Darurat\n";
        cout << " 10. Cek Status Sistem Keamanan\n";
        cout << " 11. Lihat Riwayat Laporan Kejadian\n";
        cout << " 12. Lihat Catatan Log Patroli\n";
        cout << "-------------------------------------------------\n";
        cout << " [13-17] LOGISTIK & PENGUNJUNG\n";
        cout << " 13. Data Pengunjung (CRUD)\n";
        cout << " 14. Catat Kendaraan Masuk/Keluar\n";
        cout << " 15. Catat Barang Hilang/Ditemukan\n";
        cout << " 16. Lihat Log Kendaraan\n";
        cout << " 17. Lihat Daftar Barang Hilang/Ditemukan\n";
        cout << "-------------------------------------------------\n";
        cout << " [18-19] PENGATURAN & INFO\n";
        cout << " 18. Lihat Notifikasi\n";
        cout << " 19. Pengaturan Profil\n";
        cout << "-------------------------------------------------\n";
        cout << " 0. LOGOUT\n";
        cout << "=================================================\n";
        cout << "Masukkan pilihan Anda (0-19): ";

        if (!(cin >> pilihan)) {
            cout << "❌ Input tidak valid.\n";
            clearInput();
            pilihan = -1; 
            continue;
        }
        clearInput(); 

        switch (pilihan) {
            case 1: lihatTugas(); break;
            case 2: absenKehadiran(); break;
            case 3: selesaikanTugas(); break;
            case 4: lihatLogAbsen(); break;
            case 5: lihatLaporanSelesai(); break;
            case 6: laporKejadian(); break;
            case 7: logPatroli(); break;
            case 8: laporanHarian(); break;
            case 9: modeDarurat(); break;
            case 10: cekStatusSistem(); break;
            case 11: lihatLaporanKejadian(); break;
            case 12: {
                // Tampilkan Patroli menggunakan Linked List
                Patroli *head = loadPatroli();
                if (!head) {
                    cout << "Belum ada data patroli.\n";
                } else {
                    cout << "\n=== LOG CATATAN PATROLI ===\n";
                    Patroli *temp = head;
                    int i = 1;
                    while (temp) {
                        cout << i++ << ". [" << temp->waktu << "] Area: " << temp->area 
                             << " | Kondisi: " << temp->catatan << endl;
                        temp = temp->next;
                    }
                    hapusListPatroli(head);
                }
                break;
            }
            case 13: {
                int sub;
                cout << "\n--- SUB-MENU DATA PENGUNJUNG ---\n";
                cout << "1. Tambah | 2. Lihat | 3. Ubah | 4. Hapus\nPilih: ";
                if (cin >> sub) {
                    clearInput();
                    if (sub==1) catatPengunjung();
                    else if (sub==2) tampilkanPengunjung();
                    else if (sub==3) ubahPengunjung();
                    else if (sub==4) hapusPengunjung();
                } else clearInput();
                break;
            }
            case 14: logKendaraan(); break;
            case 15: catatBarang(); break;
            case 16: lihatLogKendaraan(); break;
            case 17: lihatBarang(); break;
            case 18: lihatNotifikasi(); break; 
            case 19: pengaturanProfil(); break; 
            case 0: cout << "Logout berhasil.\n"; break;
            default: cout << "Pilihan tidak valid.\n"; break;
        }
    }
}

// ============================================================================
//                          IMPLEMENTASI FITUR
// ============================================================================

// --- TUGAS & KEHADIRAN ---

void MenuSatpam::lihatTugas() {
    // Membaca ke Linked List -> Tampilkan -> Hapus List
    Tugas *head = loadTugas(FILE_TUGAS_SATPAM, false);
    cout << "\n=== DAFTAR TUGAS AKTIF ===\n";
    if (!head) {
        cout << "Tidak ada daftar tugas.\n";
    } else {
        Tugas *temp = head;
        int i = 1;
        while (temp) {
            cout << i++ << ". " << temp->deskripsi << endl;
            temp = temp->next;
        }
    }
    hapusListTugas(head);
}

void MenuSatpam::absenKehadiran() {
    ofstream file(FILE_ABSEN_SATPAM, ios::app);
    string nama, status, waktu = getCurrentTime();
    
    cout << "\n--- ABSENSI KEHADIRAN ---\n";
    cout << "Nama Satpam: "; getline(cin, nama);
    cout << "Status (MASUK/PULANG): "; getline(cin, status);
    for (char &c : status) c = ::toupper(c);

    // Format Simpan: WAKTU | STATUS | NAMA (Agar mudah diparsing ke LL)
    file << waktu << " | " << status << " | " << nama << endl;
    file.close();
    cout << "✅ Absen tercatat.\n";
}

void MenuSatpam::lihatLogAbsen() {
    Absensi *head = loadAbsensi();
    cout << "\n=== LOG RIWAYAT KEHADIRAN ===\n";
    if (!head) {
        cout << "Belum ada data absensi.\n";
    } else {
        Absensi *temp = head;
        while (temp) {
            cout << "[" << temp->waktu << "] " << temp->nama 
                 << " - Status: " << temp->status << endl;
            temp = temp->next;
        }
    }
    hapusListAbsensi(head);
}

void MenuSatpam::selesaikanTugas() {
    ofstream file(FILE_TUGAS_SELESAI, ios::app);
    string tugas, waktu = getCurrentTime();
    cout << "\n--- TUGAS SELESAI ---\n";
    cout << "Deskripsi tugas: "; getline(cin, tugas);
    
    // Format Simpan: WAKTU | DESKRIPSI
    file << waktu << " | " << tugas << endl;
    file.close();
    cout << "✅ Tugas ditandai selesai.\n";
}

void MenuSatpam::lihatLaporanSelesai() {
    Tugas *head = loadTugas(FILE_TUGAS_SELESAI, true);
    cout << "\n=== RIWAYAT TUGAS SELESAI ===\n";
    if (!head) {
        cout << "Belum ada tugas yang diselesaikan.\n";
    } else {
        Tugas *temp = head;
        int i = 1;
        while (temp) {
            cout << i++ << ". [" << temp->waktu << "] " << temp->deskripsi << endl;
            temp = temp->next;
        }
    }
    hapusListTugas(head);
}

// --- KEAMANAN & PELAPORAN ---

void MenuSatpam::laporKejadian() {
    ofstream file(FILE_LAPORAN_SATPAM, ios::app);
    string lokasi, kejadian, tindakan, waktu = getCurrentTime();

    cout << "\n--- LAPOR INSIDEN ---\n";
    cout << "Lokasi: "; getline(cin, lokasi);
    cout << "Kejadian: "; getline(cin, kejadian);
    cout << "Tindakan: "; getline(cin, tindakan);

    // Simpan Format Satu Baris: WAKTU | LOKASI | KEJADIAN | TINDAKAN
    file << waktu << " | " << lokasi << " | " << kejadian << " | " << tindakan << endl;
    file.close();
    cout << "✅ Laporan insiden tersimpan.\n";
}

void MenuSatpam::lihatLaporanKejadian() {
    Insiden *head = loadInsiden();
    cout << "\n=== RIWAYAT INSIDEN ===\n";
    if (!head) {
        cout << "Tidak ada data insiden.\n";
    } else {
        Insiden *temp = head;
        int i = 1;
        while (temp) {
            cout << "----------------------------------------\n";
            cout << "Laporan #" << i++ << endl;
            cout << "Waktu   : " << temp->waktu << endl;
            cout << "Lokasi  : " << temp->lokasi << endl;
            cout << "Kejadian: " << temp->kejadian << endl;
            cout << "Tindakan: " << temp->tindakan << endl;
            temp = temp->next;
        }
        cout << "----------------------------------------\n";
    }
    hapusListInsiden(head);
}

void MenuSatpam::logPatroli() {
    ofstream file(FILE_PATROLI, ios::app);
    string area, catatan, waktu = getCurrentTime();
    cout << "\n--- CATAT PATROLI ---\n";
    cout << "Area: "; getline(cin, area);
    cout << "Kondisi: "; getline(cin, catatan);

    // Format: WAKTU | AREA | CATATAN
    file << waktu << " | " << area << " | " << catatan << endl;
    file.close();
    cout << "✅ Log patroli dicatat.\n";
}

// (Laporan Harian & Darurat tetap menggunakan append sederhana namun
// secara konsep struktur data, kita menganggapnya sebagai node teks tunggal saat diproses)
void MenuSatpam::laporanHarian() {
    ofstream file(FILE_LAPORAN_HARIAN, ios::app);
    string laporan;
    cout << "Ringkasan Harian: "; getline(cin, laporan);
    file << getCurrentTime() << " | " << laporan << endl;
    file.close();
    cout << "✅ Laporan harian tersimpan.\n";
}

void MenuSatpam::modeDarurat() {
    ofstream file(FILE_EMERGENCY_LOG, ios::app);
    string pesan;
    cout << "PESAN DARURAT: "; getline(cin, pesan);
    file << getCurrentTime() << " | " << pesan << endl;
    file.close();
    cout << "🚨 DARURAT DICATAT & NOTIFIKASI DIKIRIM!\n";
}

void MenuSatpam::cekStatusSistem() {
    cout << "\n[SISTEM] Semua sensor (CCTV, Fire, Door) -> ONLINE (Simulasi Linked List Node Sensor OK).\n";
}

void MenuSatpam::lihatNotifikasi() {
    cout << "\n[INFO] Tidak ada notifikasi baru.\n";
}

// --- LOGISTIK & KENDARAAN ---

void MenuSatpam::logKendaraan() {
    ofstream file(FILE_KENDARAAN, ios::app);
    string plat, jenis, status, waktu = getCurrentTime();
    cout << "\n--- LOG KENDARAAN ---\n";
    cout << "Plat: "; getline(cin, plat);
    cout << "Jenis: "; getline(cin, jenis);
    cout << "Status (MASUK/KELUAR): "; getline(cin, status);

    // Format: WAKTU | PLAT | JENIS | STATUS
    file << waktu << " | " << plat << " | " << jenis << " | " << status << endl;
    file.close();
    cout << "✅ Data kendaraan dicatat.\n";
}

void MenuSatpam::lihatLogKendaraan() {
    Kendaraan *head = loadKendaraan();
    cout << "\n=== RIWAYAT KENDARAAN ===\n";
    if (!head) {
        cout << "Data kosong.\n";
    } else {
        Kendaraan *temp = head;
        while (temp) {
            cout << "[" << temp->waktu << "] " << temp->status 
                 << " - " << temp->plat << " (" << temp->jenis << ")" << endl;
            temp = temp->next;
        }
    }
    hapusListKendaraan(head);
}

void MenuSatpam::catatBarang() {
    ofstream file(FILE_BARANG, ios::app);
    string jenis, deskripsi, lokasi, status, waktu = getCurrentTime();
    cout << "\n--- BARANG HILANG/DITEMUKAN ---\n";
    cout << "Jenis: "; getline(cin, jenis);
    cout << "Deskripsi: "; getline(cin, deskripsi);
    cout << "Lokasi: "; getline(cin, lokasi);
    cout << "Status (HILANG/DITEMUKAN): "; getline(cin, status);

    // Format: WAKTU | JENIS | DESKRIPSI | LOKASI | STATUS
    file << waktu << " | " << jenis << " | " << deskripsi << " | " << lokasi << " | " << status << endl;
    file.close();
    cout << "✅ Data barang dicatat.\n";
}

void MenuSatpam::lihatBarang() {
    Barang *head = loadBarang();
    cout << "\n=== DAFTAR BARANG ===\n";
    if (!head) {
        cout << "Data kosong.\n";
    } else {
        Barang *temp = head;
        while (temp) {
            cout << "[" << temp->status << "] " << temp->jenis << " - " << temp->deskripsi 
                 << " di " << temp->lokasi << " (" << temp->waktu << ")" << endl;
            temp = temp->next;
        }
    }
    hapusListBarang(head);
}

// --- PENGUNJUNG (CRUD Linked List) ---

void MenuSatpam::catatPengunjung() {
    Visitor *head = bacaVisitorFile();
    string nama, tujuan, waktu = getCurrentTime();
    cout << "Nama: "; getline(cin, nama);
    cout << "Tujuan: "; getline(cin, tujuan);
    tambahVisitorAkhir(head, nama, tujuan, waktu);
    simpanVisitorFile(head);
    hapusListVisitor(head);
    cout << "✅ Pengunjung ditambahkan.\n";
}

void MenuSatpam::tampilkanPengunjung() {
    Visitor *head = bacaVisitorFile();
    if (!head) {
        cout << "Data pengunjung kosong.\n"; return;
    }
    cout << "\n=== DATA PENGUNJUNG ===\n";
    Visitor *temp = head;
    int i = 1;
    while (temp) {
        cout << i++ << ". " << temp->nama << " | Ke: " << temp->tujuan << " | Jam: " << temp->waktu << endl;
        temp = temp->next;
    }
    hapusListVisitor(head);
}

void MenuSatpam::ubahPengunjung() {
    Visitor *head = bacaVisitorFile();
    if (!head) { cout << "Data kosong.\n"; return; }
    tampilkanPengunjung();
    
    int no;
    cout << "Nomor yang diubah: "; cin >> no; clearInput();
    
    Visitor *temp = head;
    int i = 1;
    while (temp && i < no) { temp = temp->next; i++; }
    
    if (temp) {
        cout << "Nama baru: "; getline(cin, temp->nama);
        cout << "Tujuan baru: "; getline(cin, temp->tujuan);
        simpanVisitorFile(head);
        cout << "✅ Data diubah.\n";
    } else {
        cout << "❌ Nomor tidak ditemukan.\n";
    }
    hapusListVisitor(head);
}

void MenuSatpam::hapusPengunjung() {
    Visitor *head = bacaVisitorFile();
    if (!head) { cout << "Data kosong.\n"; return; }
    tampilkanPengunjung();
    
    int no;
    cout << "Nomor yang dihapus: "; cin >> no; clearInput();
    
    Visitor *temp = head;
    Visitor *prev = NULL;
    int i = 1;
    
    while (temp && i < no) {
        prev = temp;
        temp = temp->next;
        i++;
    }
    
    if (temp) {
        if (!prev) head = temp->next;
        else prev->next = temp->next;
        delete temp;
        simpanVisitorFile(head);
        cout << "✅ Data dihapus.\n";
    } else {
        cout << "❌ Nomor tidak ditemukan.\n";
    }
    hapusListVisitor(head);
}

void MenuSatpam::pengaturanProfil() {
    ofstream file(FILE_PROFIL);
    string nama, shift, kontak;
    cout << "\n--- PROFIL SATPAM ---\n";
    cout << "Nama: "; getline(cin, nama);
    cout << "Shift: "; getline(cin, shift);
    cout << "Kontak: "; getline(cin, kontak);
    file << "Nama: " << nama << endl << "Shift: " << shift << endl << "Kontak: " << kontak << endl;
    file.close();
    cout << "✅ Profil disimpan.\n";
}