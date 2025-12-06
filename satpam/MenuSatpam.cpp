#include "MenuSatpam.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <ctime>     // Standard C Library untuk fungsi waktu: time(), ctime()
#include <cstdlib>   // Standard C Library untuk fungsi utilitas: ::toupper
#include <iomanip>   // Untuk format output seperti setw dan left
// #include <algorithm> // Library ini DILARANG/DIHILANGKAN sesuai permintaan user
using namespace std;

// ============================================================================
//                          KONSTANTA DEFINISI FILE LOG
// ============================================================================
/**
 * @section Konfigurasi File Log
 * Konstanta ini mendefinisikan nama-nama file teks yang digunakan untuk 
 * penyimpanan data persisten (semua data log dan konfigurasi).
 */

const string FILE_TUGAS_SATPAM     = "tugas_satpam.txt";     ///< Daftar tugas yang harus dilakukan.
const string FILE_ABSEN_SATPAM     = "absen_satpam.txt";     ///< Log catatan waktu masuk dan pulang.
const string FILE_LAPORAN_SATPAM   = "laporan_satpam.txt";   ///< Arsip laporan kejadian/insiden.
const string FILE_TUGAS_SELESAI    = "tugas_selesai.txt";    ///< Log tugas yang sudah ditandai selesai.
const string FILE_PENGUNJUNG       = "pengunjung.txt";       ///< Database data pengunjung (Linked List).
const string FILE_KENDARAAN        = "log_kendaraan.txt";    ///< Log kendaraan yang melintas/parkir.
const string FILE_BARANG           = "barang_hilang_ditemukan.txt"; ///< Log barang hilang atau ditemukan.
const string FILE_PROFIL           = "profil_satpam.txt";    ///< Data konfigurasi profil satpam.
const string FILE_LAPORAN_HARIAN   = "laporan_harian.txt";   ///< Ringkasan laporan kegiatan harian.
const string FILE_EMERGENCY_LOG    = "emergency_log.txt";    ///< Catatan log panggilan darurat.
const string FILE_PATROLI          = "log_patroli.txt";      ///< Log catatan waktu dan area patroli.

// ============================================================================
//                      STRUKTUR DATA LINKED LIST (Pengunjung)
// ============================================================================
/**
 * @brief Struktur node untuk menyimpan data Pengunjung.
 * Struktur ini adalah elemen dasar dari Singly Linked List yang digunakan 
 * untuk manajemen data pengunjung (CRUD).
 */
struct Visitor {
    string nama;    ///< Nama lengkap Pengunjung.
    string tujuan;  ///< Tujuan kedatangan Pengunjung di gedung.
    string waktu;   ///< Waktu kedatangan (Timestamp saat dicatat).
    Visitor *next;  ///< Pointer ke node Pengunjung berikutnya. NULL jika node terakhir.
};

// ============================================================================
//                          FUNGSI PENDUKUNG LINKED LIST
// ============================================================================

/**
 * @brief Membuat node pengunjung baru.
 * Fungsi ini mengalokasikan memori untuk satu node Visitor.
 * * @param nama Nama pengunjung yang baru.
 * @param tujuan Tujuan kunjungan pengunjung.
 * @param waktu Waktu pencatatan (timestamp).
 * @return Pointer ke node Visitor yang baru dibuat.
 */
Visitor* buatNode(string nama, string tujuan, string waktu) {
    // 1. Alokasikan memori di heap menggunakan operator new
    Visitor *baru = new Visitor;

    // 2. Inisialisasi data anggota (fields)
    baru->nama   = nama;
    baru->tujuan = tujuan;
    baru->waktu  = waktu;
    
    // 3. Set pointer 'next' ke NULL karena ini adalah node terakhir sementara
    baru->next   = NULL; 

    // 4. Kembalikan pointer ke node yang baru
    return baru;
}

/**
 * @brief Menambahkan node Pengunjung baru di akhir linked list.
 * Fungsi ini memastikan penambahan data baru di akhir daftar.
 * * @param head Referensi ke pointer kepala (head) list.
 * @param nama Nama pengunjung yang akan ditambahkan.
 * @param tujuan Tujuan kunjungan.
 * @param waktu Waktu pencatatan.
 */
void tambahDiAkhir(Visitor* &head, string nama, string tujuan, string waktu) {
    // 1. Buat node baru dengan data yang disediakan
    Visitor *baru = buatNode(nama, tujuan, waktu);

    // 2. Cek apakah linked list saat ini kosong
    if (head == NULL) {
        // Jika kosong, node baru menjadi kepala (head) list
        head = baru;
    } 
    // 3. Jika list sudah ada isinya
    else {
        // Deklarasi pointer sementara untuk penelusuran (traversal)
        Visitor *temp = head; 

        // Lakukan penelusuran sampai menemukan node terakhir (next == NULL)
        while (temp->next != NULL) {
            temp = temp->next;
        }
        
        // Setelah node terakhir ditemukan, hubungkan node terakhir lama ke node baru
        temp->next = baru;
    }
}

/**
 * @brief Membaca seluruh data pengunjung dari file teks dan membangun linked list.
 * Fungsi ini bertanggung jawab untuk memuat data saat sistem dimulai atau diakses.
 * * @return Pointer ke kepala (head) linked list. NULL jika file kosong atau gagal dibuka.
 */
Visitor* bacaDariFile() {
    // 1. Buka file pengunjung untuk dibaca
    ifstream file(FILE_PENGUNJUNG);
    
    // 2. Cek status operasi pembukaan file
    if (!file.is_open()) {
        // Jika gagal, kembalikan NULL
        return NULL; 
    }

    // 3. Inisialisasi kepala list dan variabel penampung
    Visitor *head = NULL;
    string baris;

    // 4. Loop untuk membaca setiap baris (record) dari file
    while (getline(file, baris)) {
        // Validasi: Abaikan baris yang benar-benar kosong
        if (baris.empty()) {
            continue; 
        }

        // 5. Parsing data berdasarkan delimiter " | "
        size_t p1 = baris.find(" | ");
        size_t p2 = baris.find(" | ", p1 + 3);

        // Validasi: Pastikan kedua delimiter ditemukan
        if (p1 == string::npos || p2 == string::npos) {
            continue; 
        }

        // 6. Ekstraksi segmen data (Nama, Tujuan, Waktu)
        string nama   = baris.substr(0, p1);
        string tujuan = baris.substr(p1 + 3, p2 - (p1 + 3));
        string waktu  = baris.substr(p2 + 3);

        // 7. Membersihkan karakter newline/carriage return yang mungkin tersisa
        //    (Implementasi manual tanpa <algorithm> agar sesuai batasan library)
        while (!waktu.empty() && (waktu.back() == '\n' || waktu.back() == '\r')) {
            waktu.pop_back(); 
        }

        // 8. Tambahkan data yang sudah diparsing ke linked list
        tambahDiAkhir(head, nama, tujuan, waktu);
    }

    // 9. Tutup file dan kembalikan kepala list
    file.close();
    return head;
}

/**
 * @brief Menyimpan seluruh data dari linked list ke file teks.
 * Fungsi ini menimpa (overwrite) isi file lama dengan isi list yang baru.
 * * @param head Pointer ke kepala linked list.
 * @return True jika berhasil menyimpan, False jika gagal.
 */
bool simpanKeFile(Visitor *head) {
    // 1. Buka file pengunjung untuk ditulis (mode overwrite default)
    ofstream file(FILE_PENGUNJUNG); 

    // 2. Cek apakah operasi pembukaan file berhasil
    if (!file.is_open()) {
        cout << "❌ Gagal menyimpan data ke file pengunjung.txt\n";
        return false;
    }

    // 3. Deklarasikan pointer sementara untuk traversal
    Visitor *temp = head;

    // 4. Lakukan Traversal (penelusuran) linked list
    while (temp != NULL) {
        string waktu = temp->waktu;
        
        // 5. Pastikan waktu bersih dari karakter kontrol sebelum disimpan
        //    (Implementasi manual)
        while (!waktu.empty() && (waktu.back() == '\n' || waktu.back() == '\r')) {
            waktu.pop_back();
        }

        // 6. Tulis data ke file dengan format "Nama | Tujuan | Waktu"
        file << temp->nama
             << " | " << temp->tujuan
             << " | " << waktu 
             << endl; // Pindahkan ke baris baru untuk record berikutnya

        // 7. Pindah ke node berikutnya
        temp = temp->next;
    }

    // 8. Tutup file dan laporkan keberhasilan
    file.close();
    return true;
}

/**
 * @brief Menghapus semua node dalam linked list dan membersihkan memori heap.
 * Ini adalah langkah penting untuk mencegah memory leak setelah operasi selesai.
 * * @param head Referensi ke pointer kepala (head) list.
 */
void hapusSemua(Visitor *&head) {
    // Loop akan berjalan selama list memiliki node (head != NULL)
    while (head != NULL) {
        // 1. Simpan pointer ke node yang akan dihapus
        Visitor *hapus = head;
        
        // 2. Majukan pointer head ke node berikutnya (putuskan hubungan)
        head = head->next; 
        
        // 3. Bebaskan memori node yang disimpan tadi
        delete hapus;     
    }
}

// ============================================================================
//                      FUNGSI PENDUKUNG UTILITY UMUM
// ============================================================================

/**
 * @brief Mendapatkan waktu sistem saat ini dalam format string yang bersih.
 * @return String yang berisi tanggal dan waktu saat ini (tanpa newline).
 */
string getCurrentTime() {
    // 1. Mendapatkan waktu mentah (timestamp) sebagai time_t
    time_t now = time(0); 

    // 2. Konversi timestamp ke format string C-style (mengandung newline)
    string waktu = ctime(&now); 

    // 3. Membersihkan karakter newline (\n) dan carriage return (\r) dari string
    //    (Implementasi manual)
    while (!waktu.empty() && (waktu.back() == '\n' || waktu.back() == '\r')) {
        waktu.pop_back(); 
    }

    // 4. Kembalikan string waktu yang sudah bersih
    return waktu;
}

/**
 * @brief Fungsi umum untuk membaca dan menampilkan isi file teks ke konsol.
 * @param namaFile Nama file yang akan dibaca (konstanta global).
 * @param judul Judul yang akan ditampilkan di header output.
 */
void tampilkanIsiFile(const string& namaFile, const string& judul) {
    // 1. Deklarasi stream untuk membaca file
    ifstream file(namaFile);

    // 2. Tampilkan header yang rapi
    cout << "\n=== " << judul << " ===\n";

    // 3. Cek apakah file berhasil dibuka
    if (!file.is_open()) {
        cout << "Belum ada data di " << namaFile << ". File tidak ditemukan.\n";
        return; // Keluar dari fungsi jika gagal
    }

    // 4. Inisialisasi variabel untuk loop pembacaan
    string baris;
    int hitung = 1; // Penghitung baris dimulai dari 1

    cout << "---------------------------------------------------------\n";

    // 5. Loop untuk membaca setiap baris dari file
    while (getline(file, baris)) {
        // 6. Tampilkan nomor urut dan isi baris
        cout << "[" << setw(3) << right << hitung++ << "] " << baris << endl;
    }

    cout << "---------------------------------------------------------\n";

    // 7. Tutup file setelah selesai
    file.close();
}

/**
 * @brief Fungsi untuk membuang karakter newline yang tersisa di buffer cin.
 * Ini mencegah fungsi getline() berikutnya mengambil newline sisa dari cin >> int.
 */
void clearInput() {
    // 1. Memeriksa apakah input buffer perlu dibersihkan
    if (cin.peek() == '\n' || cin.peek() == EOF) {
        cin.ignore(1000, '\n');
    } else {
        // 2. Jika status cin error (misalnya input tidak sesuai tipe)
        cin.clear();
        cin.ignore(1000, '\n'); // Buang semua sisa input yang bermasalah
    }
}


// ============================================================================
//       IMPLEMENTASI CLASS MenuSatpam - Menu Utama (Display Logic)
// ============================================================================

void MenuSatpam::tampilkanMenu() {
    // 1. Inisialisasi variabel pilihan
    int pilihan = -1; 

    // 2. Loop utama menu: akan terus berjalan hingga pengguna memilih '0' (Logout)
    while (pilihan != 0) {
        // --- Tampilan Header Menu ---
        cout << "\n=================================================\n";
        cout << "         SISTEM MANAJEMEN SATPAM GEDUNG          \n";
        cout << "=================================================\n";
        
        // --- KELOMPOK 1: TUGAS & KEHADIRAN ---
        cout << " [1-5] TUGAS & KEHADIRAN\n";
        cout << " 1. Lihat Daftar Tugas (Aktif)\n";
        cout << " 2. Absen Kehadiran (Masuk/Pulang)\n";
        cout << " 3. Tandai Tugas Selesai\n";
        cout << " 4. Lihat Log Absen Kehadiran\n";
        cout << " 5. Lihat Riwayat Tugas Selesai\n";
        cout << "-------------------------------------------------\n";
        
        // --- KELOMPOK 2: OPERASI KEAMANAN & PELAPORAN ---
        cout << " [6-12] OPERASI KEAMANAN & PELAPORAN\n";
        cout << " 6. Lapor Kejadian (Insiden)\n";
        cout << " 7. Catat Log Patroli Area\n";
        cout << " 8. Laporkan Kegiatan Harian (Ringkasan Shift)\n";
        cout << " 9. Mode Panggilan Darurat (Emergency Call)\n";
        cout << " 10. Cek Status Sistem Keamanan (CCTV, Alarm)\n";
        cout << " 11. Lihat Riwayat Laporan Kejadian\n";
        cout << " 12. Lihat Catatan Log Patroli\n";
        cout << "-------------------------------------------------\n";
        
        // --- KELOMPOK 3: LOGISTIK & PENGUNJUNG ---
        cout << " [13-17] LOGISTIK & PENGUNJUNG\n";
        cout << " 13. Data Pengunjung (CRUD Sub-Menu)\n";
        cout << " 14. Catat Kendaraan Masuk/Keluar\n";
        cout << " 15. Catat Barang Hilang/Ditemukan\n";
        cout << " 16. Lihat Log Kendaraan\n";
        cout << " 17. Lihat Daftar Barang Hilang/Ditemukan\n";
        cout << "-------------------------------------------------\n";

        // --- KELOMPOK 4: PENGATURAN & INFO ---
        cout << " [18-19] PENGATURAN & INFO\n";
        cout << " 18. Lihat Notifikasi & Informasi Internal\n";
        cout << " 19. Pengaturan Profil Satpam\n";
        cout << "-------------------------------------------------\n";
        
        cout << " 0. LOGOUT & Keluar dari Sistem\n";
        cout << "=================================================\n";

        cout << "Masukkan pilihan Anda (0-19): ";

        // 3. Membaca input pilihan
        if (!(cin >> pilihan)) {
            // Jika pembacaan gagal (misal user input teks)
            cout << "❌ Input tidak valid. Harap masukkan angka saja.\n";
            clearInput();
            pilihan = -1; 
            continue;
        }

        // 4. Bersihkan buffer input setelah cin >> int
        clearInput(); 

        // 5. --- Pemrosesan Pilihan Menu menggunakan Switch Case ---
        switch (pilihan) {
            // Kelompok 1: Tugas & Kehadiran
            case 1: lihatTugas(); break;
            case 2: absenKehadiran(); break;
            case 3: selesaikanTugas(); break;
            case 4: lihatLogAbsen(); break;
            case 5: lihatLaporanSelesai(); break;
            
            // Kelompok 2: Operasi Keamanan & Pelaporan
            case 6: laporKejadian(); break;
            case 7: logPatroli(); break;
            case 8: laporanHarian(); break;
            case 9: modeDarurat(); break;
            case 10: cekStatusSistem(); break;
            case 11: lihatLaporanKejadian(); break;
            case 12: tampilkanIsiFile(FILE_PATROLI, "LOG CATATAN PATROLI"); break; 

            // Kelompok 3: Logistik & Pengunjung
            case 13: {
                // Sub-menu Pengunjung memerlukan blok sendiri
                int sub;
                cout << "\n--- SUB-MENU DATA PENGUNJUNG (CRUD) ---\n";
                cout << "1. Tambah Pengunjung Baru\n";
                cout << "2. Lihat Semua Pengunjung\n";
                cout << "3. Ubah Data Pengunjung\n";
                cout << "4. Hapus Data Pengunjung\n";
                cout << "Pilih Sub-Menu: ";
                if (!(cin >> sub)) {
                    cout << "❌ Input Sub-Menu tidak valid!\n";
                    clearInput();
                    break;
                }
                clearInput();
                if      (sub == 1) catatPengunjung();
                else if (sub == 2) tampilkanPengunjung();
                else if (sub == 3) ubahPengunjung();
                else if (sub == 4) hapusPengunjung();
                else               cout << "❌ Sub-menu tidak valid!\n";
                break;
            }
            case 14: logKendaraan(); break;
            case 15: catatBarang(); break;
            case 16: lihatLogKendaraan(); break;
            case 17: lihatBarang(); break;

            // Kelompok 4: Pengaturan & Info
            case 18: lihatNotifikasi(); break; 
            case 19: pengaturanProfil(); break; 

            // Pilihan Logout
            case 0: 
                cout << "Logout berhasil. Selamat bertugas kembali dan selalu waspada!\n"; 
                break;
            
            // Pilihan Default (Tidak Valid)
            default: 
                cout << "Pilihan menu tidak ditemukan. Mohon masukkan angka antara 0-19.\n"; 
                break;
        }
    }
}

// ============================================================================
//         IMPLEMENTASI CLASS MenuSatpam - FITUR-FITUR
// ============================================================================

// ----------------------------------------------------------------------------
// Kelompok 1: Tugas & Kehadiran
// ----------------------------------------------------------------------------

void MenuSatpam::lihatTugas() {
    // Fungsi ini hanya memanggil fungsi utilitas untuk menampilkan isi file.
    tampilkanIsiFile(FILE_TUGAS_SATPAM, "DAFTAR TUGAS AKTIF DAN TERJADWAL");
}

void MenuSatpam::absenKehadiran() {
    // 1. Buka file absen dalam mode append (ios::app)
    ofstream file(FILE_ABSEN_SATPAM, ios::app);
    
    // 2. Deklarasi variabel input
    string nama;
    int status_int; // Variabel untuk input 1 atau 0
    string status_str = ""; // Variabel untuk string status yang akan dicatat

    cout << "\n--- PROSES ABSENSI KEHADIRAN ---\n";
    
    // 3. Ambil input Nama
    cout << "Masukkan nama lengkap Satpam: ";
    getline(cin, nama);

    // 4. Ambil input Status (1 atau 0)
    cout << "Pilih Status Absensi:\n";
    cout << "  1. MASUK\n";
    cout << "  0. PULANG\n";
    cout << "Masukkan pilihan (1/0): ";
    
    // Loop validasi input
    while (!(cin >> status_int) || (status_int != 1 && status_int != 0)) {
        cout << "❌ Input tidak valid! Harap masukkan 1 (MASUK) atau 0 (PULANG): ";
        clearInput(); // Bersihkan buffer input
    }
    clearInput(); // Bersihkan buffer input setelah cin >> int

    // 5. Konversi input integer ke string status
    if (status_int == 1) {
        status_str = "MASUK";
    } else {
        status_str = "PULANG";
    }

    // 6. Dapatkan waktu saat ini
    string waktu = getCurrentTime();

    // 7. Tulis data absen ke file
    file << "[" << status_str << "] " << nama << " dicatat pada waktu: " << waktu << endl;
    
    // 8. Tutup file dan berikan konfirmasi
    file.close(); 
    cout << "✅ Absen **" << status_str << "** berhasil dicatat pada " << waktu << "!\n";
}

void MenuSatpam::selesaikanTugas() {
    // 1. Buka file tugas selesai dalam mode append
    ofstream file(FILE_TUGAS_SELESAI, ios::app);
    
    // 2. Deklarasi variabel input
    string tugas;

    cout << "\n--- PENCATATAN TUGAS SELESAI ---\n";
    
    // 3. Ambil input tugas yang selesai
    cout << "Masukkan deskripsi tugas yang telah diselesaikan (Contoh: Patroli Area Basement): ";
    getline(cin, tugas);

    // 4. Pencatatan log dengan waktu saat ini
    file << "[SELESAI] Tugas '" << tugas << "' berhasil diselesaikan pada: " << getCurrentTime() << endl;

    // 5. Tutup file dan konfirmasi
    file.close();
    cout << "✅ Tugas telah ditandai selesai dan log telah diperbarui.\n";
}

void MenuSatpam::lihatLogAbsen() {
    // Tampilkan isi file Absen
    tampilkanIsiFile(FILE_ABSEN_SATPAM, "LOG RIWAYAT KEHADIRAN (ABSENSI)");
}

void MenuSatpam::lihatLaporanSelesai() {
    // Tampilkan isi file Tugas Selesai
    tampilkanIsiFile(FILE_TUGAS_SELESAI, "RIWAYAT TUGAS YANG SUDAH DISELESAIKAN");
}

// ----------------------------------------------------------------------------
// Kelompok 2: Laporan & Keamanan
// ----------------------------------------------------------------------------

void MenuSatpam::laporKejadian() {
    // 1. Buka file laporan insiden dalam mode append
    ofstream file(FILE_LAPORAN_SATPAM, ios::app);
    
    // 2. Deklarasi variabel input
    string kejadian;
    string lokasi;
    string tindakan;

    cout << "\n--- FORMULIR LAPORAN INSIDEN/KEJADIAN ---\n";
    
    // 3. Ambil input Lokasi
    cout << "Lokasi Kejadian yang pasti: ";
    getline(cin, lokasi);
    
    // 4. Ambil input Deskripsi Kejadian
    cout << "Deskripsi Lengkap Kejadian (apa, siapa, mengapa): ";
    getline(cin, kejadian);
    
    // 5. Ambil input Tindakan yang diambil
    cout << "Tindakan Cepat yang diambil di lapangan: ";
    getline(cin, tindakan);
    
    // 6. Format pencatatan laporan yang detail dan terstruktur (Multi-baris untuk keterbacaan)
    file << "================ LAPORAN BARU ================" << endl;
    file << "[WAKTU] " << getCurrentTime() << endl;
    file << "[LOKASI] " << lokasi << endl;
    file << "[KEJADIAN] " << kejadian << endl;
    file << "[TINDAKAN] " << tindakan << endl;
    file << "==============================================\n" << endl;
    
    // 7. Tutup file dan konfirmasi
    file.close();
    cout << "✅ Laporan kejadian berhasil disimpan ke dalam arsip.\n";
}

void MenuSatpam::lihatLaporanKejadian() {
    // Menampilkan riwayat laporan insiden
    tampilkanIsiFile(FILE_LAPORAN_SATPAM, "RIWAYAT LOG LAPORAN KEJADIAN (INSIDEN)");
}

void MenuSatpam::logPatroli() {
    // 1. Buka file log patroli dalam mode append
    ofstream file(FILE_PATROLI, ios::app);
    
    // 2. Deklarasi variabel input
    string area;
    string catatan;

    cout << "\n--- CATATAN LOG PATROLI HARIAN ---\n";
    
    // 3. Ambil input Area
    cout << "Area yang dipatroli (Contoh: Lantai 3, Basement Blok A): ";
    getline(cin, area);
    
    // 4. Ambil input Catatan/Kondisi
    cout << "Catatan/Kondisi ditemukan (Contoh: Aman, ada pintu darurat terbuka): ";
    getline(cin, catatan);

    // 5. Tulis log patroli
    file << "[PATROLI SELESAI] Waktu: " << getCurrentTime()
         << " | Area: " << area
         << " | Kondisi: " << catatan << endl;
          
    // 6. Tutup file dan konfirmasi
    file.close();
    cout << "✅ Log Patroli untuk area " << area << " berhasil dicatat.\n";
}

void MenuSatpam::laporanHarian() {
    // 1. Buka file laporan harian dalam mode append
    ofstream file(FILE_LAPORAN_HARIAN, ios::app);

    // 2. Deklarasi variabel input
    string laporan;
    
    cout << "\n--- FORMULIR LAPORAN KEGIATAN HARIAN ---\n";
    
    // 3. Ambil input Ringkasan Laporan
    cout << "Tuliskan ringkasan kegiatan, isu, dan kondisi umum gedung selama shift: ";
    getline(cin, laporan);

    // 4. Format laporan (Header, Waktu, Konten)
    file << "========================================" << endl;
    file << "LAPORAN HARIAN - SHIFT BERAKHIR PADA " << getCurrentTime() << endl;
    file << "RINGKASAN KEGIATAN:\n" << laporan << endl;
    file << "========================================\n" << endl;
    
    // 5. Tutup file dan konfirmasi
    file.close();
    cout << "✅ Laporan harian tersimpan. Terima kasih atas laporannya.\n";
}

void MenuSatpam::modeDarurat() {
    // 1. Buka file log darurat
    ofstream file(FILE_EMERGENCY_LOG, ios::app);
    
    // 2. Periksa apakah file berhasil dibuka
    if (!file.is_open()) {
        cout << "❌ Gagal membuka file emergency_log.txt. Tidak dapat mencatat darurat.\n";
        return;
    }

    // 3. Deklarasi variabel input
    string pesan;
    
    cout << "\n=== MODE PANGGILAN DARURAT 🚨 (PENTING!) ===\n";
    
    // 4. Ambil input pesan darurat
    cout << "Masukkan deskripsi singkat keadaan darurat dan lokasi: ";
    getline(cin, pesan);

    // 5. Mencatat log darurat dengan cap waktu (Format mencolok)
    file << "[!!! EMERGENCY AKTIF !!!] Waktu: " << getCurrentTime() << " - PESAN: " << pesan << endl;
    
    // 6. Tutup file
    file.close();

    // 7. Berikan konfirmasi dan simulasi pengiriman notifikasi
    cout << "🚨🚨🚨 LOG DARURAT BERHASIL DICATAT! 🚨🚨🚨\n";
    cout << "Sistem DUMMY telah mengirim notifikasi ke Manajemen dan Tim Respon. Tetap tenang dan ikuti prosedur!\n";
}

void MenuSatpam::cekStatusSistem() {
    cout << "\n=== CEK STATUS SISTEM KEAMANAN (SIMULASI REAL-TIME) ===\n";
    cout << "Waktu Pemeriksaan: " << getCurrentTime() << endl;
    cout << "------------------------------------------------------\n";
    
    // Simulasi status setiap komponen keamanan
    
    // 1. Status Simulasi CCTV
    cout << "Status CCTV Area Utama & Lobby:  [STATUS: OK ✅, Rekam]\n";
    
    // 2. Status Simulasi Alarm Kebakaran
    cout << "Status Alarm Kebakaran Lantai 5: [STATUS: OK ✅, Siaga]\n";
    
    // 3. Status Simulasi Alarm Banjir
    cout << "Status Alarm Banjir Basement:    [STATUS: OK ✅, Normal]\n";
    
    // 4. Status Simulasi Kontrol Akses
    cout << "Status Kontrol Akses Pintu Server: [STATUS: TERKUNCI ✅]\n";
    
    // 5. Status Simulasi Daya Cadangan
    cout << "Status Power Backup (UPS/Genset):[STATUS: OK ✅, Baterai 100%]\n";
    
    // 6. Kondisi Lingkungan Simulasi
    cout << "Kondisi Jaringan Sensor Suhu:    [STATUS: Normal (25°C)]\n";
    
    cout << "------------------------------------------------------\n";
    cout << "Kesimpulan: Semua sistem inti beroperasi dengan Normal. Pertahankan kewaspadaan.\n";
}

void MenuSatpam::lihatNotifikasi() {
    cout << "\n=== NOTIFIKASI DAN PENGUMUMAN INTERNAL ===\n";
    cout << "Waktu Akses: " << getCurrentTime() << endl;
    cout << "------------------------------------------\n";
    
    // Daftar notifikasi simulasi
    
    // 1. Penggantian Shift
    cout << "1. [07:50] PENGINGAT SHIFT: Bersiaplah untuk serah terima shift dengan Satpam Ahmad pukul 08:00.\n";
    
    // 2. Pekerjaan Teknis
    cout << "2. [10:15] INFO TEKNIS: Teknisi AC akan melakukan perawatan rutin di lantai 4, harap dipandu ke ruang server.\n";
    
    // 3. Peringatan Lingkungan
    cout << "3. [14:00] PERINGATAN: Dilaporkan ada potensi hujan deras. Mohon periksa penutup atap parkiran.\n";
    
    cout << "------------------------------------------\n";
    cout << "Semua notifikasi penting telah dibaca. Tidak ada notifikasi baru yang mendesak.\n";
}


// ----------------------------------------------------------------------------
// Kelompok 3: Log Barang & Kendaraan
// ----------------------------------------------------------------------------

void MenuSatpam::logKendaraan() {
    // 1. Buka file log kendaraan
    ofstream file(FILE_KENDARAAN, ios::app);
    
    // 2. Deklarasi variabel input
    string plat;
    string jenis;
    int status_int; // Variabel untuk input 1 atau 0
    string status_str = ""; // Variabel untuk string status yang akan dicatat

    cout << "\n--- PENCATATAN KENDARAAN MASUK/KELUAR ---\n";
    
    // 3. Ambil input Plat Nomor
    cout << "Masukkan plat nomor kendaraan (Contoh: B 1234 ABC): ";
    getline(cin, plat);

    // 4. Ambil input Jenis Kendaraan
    cout << "Jenis Kendaraan (Mobil / Motor / Lainnya): ";
    getline(cin, jenis);

    // 5. Ambil input Status Pergerakan (1 atau 0)
    cout << "Pilih Status Pergerakan:\n";
    cout << "  1. MASUK\n";
    cout << "  0. KELUAR\n";
    cout << "Masukkan pilihan (1/0): ";

    // Loop validasi input
    while (!(cin >> status_int) || (status_int != 1 && status_int != 0)) {
        cout << "❌ Input tidak valid! Harap masukkan 1 (MASUK) atau 0 (KELUAR): ";
        clearInput(); // Bersihkan buffer input
    }
    clearInput(); // Bersihkan buffer input setelah cin >> int

    // 6. Konversi input integer ke string status
    if (status_int == 1) {
        status_str = "MASUK";
    } else {
        status_str = "KELUAR";
    }
    
    // 7. Tulis log ke file
    file << "[" << getCurrentTime() << "][" << status_str << "] Plat: " 
         << plat << " | Jenis: " << jenis << endl;
          
    // 8. Tutup file dan konfirmasi
    file.close();
    cout << "✅ Data kendaraan **" << status_str << "** berhasil dicatat.\n";
}

void MenuSatpam::lihatLogKendaraan() {
    // Tampilkan isi file Log Kendaraan
    tampilkanIsiFile(FILE_KENDARAAN, "LOG RIWAYAT KENDARAAN MASUK/KELUAR");
}

void MenuSatpam::catatBarang() {
    // 1. Buka file log barang
    ofstream file(FILE_BARANG, ios::app);

    // 2. Deklarasi variabel input
    string jenis;
    string lokasi;
    string status;
    string deskripsi;

    cout << "\n--- PENCATATAN BARANG HILANG ATAU DITEMUKAN ---\n";
    
    // 3. Ambil input Jenis Barang
    cout << "Masukkan nama/jenis barang: ";
    getline(cin, jenis);
    
    // 4. Ambil input Deskripsi Barang
    cout << "Deskripsi singkat barang (Contoh: Dompet kulit hitam, kunci motor Yamaha): ";
    getline(cin, deskripsi);

    // 5. Ambil input Lokasi
    cout << "Lokasi barang ditemukan/hilang: ";
    getline(cin, lokasi);

    // 6. Ambil input Status
    cout << "Status barang (HILANG / DITEMUKAN): ";
    getline(cin, status);
    
    // 7. Tulis log ke file
    file << "[" << getCurrentTime() << "][" << status << "] Jenis: " << jenis
         << " | Deskripsi: " << deskripsi
         << " | Lokasi: " << lokasi << endl;

    // 8. Tutup file dan konfirmasi
    file.close();
    cout << "✅ Data barang berhasil disimpan.\n";
}

void MenuSatpam::lihatBarang() {
    // Tampilkan isi file Log Barang
    tampilkanIsiFile(FILE_BARANG, "DAFTAR BARANG HILANG/DITEMUKAN");
}

// ----------------------------------------------------------------------------
// Kelompok 4: CRUD Pengunjung (Menggunakan Linked List)
// ----------------------------------------------------------------------------

void MenuSatpam::catatPengunjung() {
    // 1. Muat data pengunjung yang sudah ada ke dalam linked list
    Visitor *head = bacaDariFile();
    
    // 2. Deklarasi variabel input
    string nama;
    string tujuan;

    cout << "\n--- TAMBAH PENGUNJUNG BARU ---\n";
    
    // 3. Ambil input Nama
    cout << "Nama pengunjung: ";
    getline(cin, nama);

    // 4. Ambil input Tujuan
    cout << "Tujuan datang/Kontak yang dituju: ";
    getline(cin, tujuan);

    // 5. Dapatkan waktu saat ini
    string waktu = getCurrentTime();

    // 6. Menambahkan node baru ke akhir list
    tambahDiAkhir(head, nama, tujuan, waktu);

    // 7. Menyimpan kembali seluruh list (data pengunjung) ke file
    simpanKeFile(head);
    
    // 8. Membersihkan memori (penting!)
    hapusSemua(head);

    // 9. Konfirmasi
    cout << "✅ Data pengunjung baru berhasil disimpan pada waktu " << waktu << ".\n";
}

void MenuSatpam::tampilkanPengunjung() {
    // 1. Muat data pengunjung dari file
    Visitor *head = bacaDariFile();

    // 2. Cek jika list kosong
    if (!head) {
        cout << "Belum ada data pengunjung yang tercatat saat ini.\n";
        return;
    }

    // 3. Tampilkan header
    cout << "\n=== DAFTAR PENGUNJUNG (LOG KUNJUNGAN) ===\n";
    cout << "------------------------------------------------------------------------------------------------\n";

    // 4. Inisialisasi pointer traversal dan counter
    int i = 1;
    Visitor *temp = head;

    // 5. Traversal dan tampilkan data
    while (temp) {
        // Menggunakan setw dan left untuk format kolom yang rapi
        cout << setw(2) << right << i++ << ". [Nama] " << setw(20) << left << temp->nama
             << " | [Tujuan] " << setw(20) << left << temp->tujuan
             << " | [Waktu Masuk] " << temp->waktu << endl;

        temp = temp->next;
    }

    cout << "------------------------------------------------------------------------------------------------\n";

    // 6. Membersihkan memori setelah selesai
    hapusSemua(head);
}

void MenuSatpam::ubahPengunjung() {
    // 1. Muat data dari file
    Visitor *head = bacaDariFile();

    // 2. Cek jika list kosong
    if (!head) {
        cout << "❌ Belum ada data pengunjung untuk diubah. List kosong.\n";
        return;
    }

    // 3. Tampilkan daftar saat ini agar user bisa memilih
    tampilkanPengunjung();

    // 4. Ambil input nomor urut yang akan diubah
    cout << "\nMasukkan nomor pengunjung yang ingin diubah: ";
    int no = 0; // Inisialisasi eksplisit

    if (!(cin >> no)) {
        cout << "❌ Input tidak valid! Harap masukkan nomor urut.\n";
        clearInput();
        hapusSemua(head);
        return;
    }

    clearInput();

    // 5. Cari node berdasarkan nomor urut
    int i = 1;
    Visitor *temp = head;

    while (temp && i < no) {
        temp = temp->next;
        i++;
    }

    // 6. Cek hasil pencarian
    if (!temp) {
        cout << "❌ Nomor pengunjung (" << no << ") tidak ditemukan dalam daftar!\n";
    } else {
        // 7. Proses pengubahan data
        cout << "--- Mengubah data untuk: " << temp->nama << " ---\n";
        
        cout << "Masukkan NAMA baru (lama: " << temp->nama << "): ";
        getline(cin, temp->nama);

        cout << "Masukkan TUJUAN baru (lama: " << temp->tujuan << "): ";
        getline(cin, temp->tujuan);

        // Catatan: Waktu (temp->waktu) TIDAK diubah, mencerminkan waktu masuk awal

        // 8. Simpan perubahan ke file
        simpanKeFile(head);
        cout << "✅ Data pengunjung berhasil diperbarui.\n";
    }

    // 9. Bersihkan memori
    hapusSemua(head);
}

void MenuSatpam::hapusPengunjung() {
    // 1. Muat data dari file
    Visitor *head = bacaDariFile();

    // 2. Cek jika list kosong
    if (!head) {
        cout << "❌ Belum ada data pengunjung untuk dihapus. List kosong.\n";
        return;
    }

    // 3. Tampilkan daftar saat ini
    tampilkanPengunjung();

    // 4. Ambil input nomor urut yang akan dihapus
    cout << "\nMasukkan nomor pengunjung yang ingin dihapus: ";
    int no = 0; // Inisialisasi eksplisit

    if (!(cin >> no)) {
        cout << "❌ Input tidak valid! Harap masukkan nomor urut.\n";
        clearInput();
        hapusSemua(head);
        return;
    }

    // 5. Cari node berdasarkan nomor urut (cari temp dan prev)
    Visitor *temp = head;
    Visitor *prev = NULL;
    int i = 1;

    while (temp && i < no) {
        prev = temp;
        temp = temp->next;
        i++;
    }

    // 6. Cek hasil pencarian
    if (!temp) {
        cout << "❌ Nomor pengunjung (" << no << ") tidak ditemukan.\n";
    } else {
        // 7. Logika penghapusan node
        string namaHapus = temp->nama;

        if (!prev)
            head = temp->next; // Kasus: Menghapus node pertama
        else
            prev->next = temp->next; // Kasus: Menghapus node di tengah/akhir

        delete temp; // Membebaskan memori node yang dihapus

        // 8. Simpan perubahan ke file
        simpanKeFile(head);

        // 9. Konfirmasi
        cout << "✅ Data pengunjung atas nama " << namaHapus << " berhasil dihapus.\n";
    }

    // 10. Bersihkan memori
    hapusSemua(head);
}

// ----------------------------------------------------------------------------
// Kelompok 5: Pengaturan
// ----------------------------------------------------------------------------

void MenuSatpam::pengaturanProfil() {
    // 1. Buka file profil (mode overwrite, karena hanya satu profil aktif)
    ofstream file(FILE_PROFIL); 

    // 2. Deklarasi dan inisialisasi variabel input
    string nama = "";
    string shift = "";
    string kontak = "";

    cout << "\n--- PENGATURAN PROFIL & DATA DIRI SATPAM ---\n";
    
    // 3. Ambil input Nama
    cout << "Masukkan nama lengkap: ";
    getline(cin, nama);

    // 4. Ambil input Shift
    cout << "Shift bertugas saat ini (Pagi / Siang / Malam / Bebas): ";
    getline(cin, shift);

    // 5. Ambil input Kontak
    cout << "Nomor kontak darurat (WA/HP): ";
    getline(cin, kontak);

    // 6. Menulis data profil ke file
    file << "Nama Satpam: " << nama << endl;
    file << "Shift Tugas: " << shift << endl;
    file << "Nomor Kontak: " << kontak << endl;

    // 7. Tutup file
    file.close();

    // 8. Konfirmasi
    cout << "✅ Profil satpam  berhasil disimpan dan diperbarui.\n";
}