#include "MenuOb.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

// ===================== KONFIGURASI QUEUE (LINKED LIST) =====================
const string FILE_TUGAS_OB = "tugas_ob.txt";

struct NodeTugas {
    string deskripsi;
    NodeTugas* next;
};

// Variabel STATIC agar tidak bentrok dengan MenuAdmin.cpp
static NodeTugas* front = nullptr;
static NodeTugas* rear = nullptr;

// ===================== HELPER QUEUE (LOAD & SAVE) =====================

// Tambahkan 'static' di depan fungsi-fungsi ini:

// Helper: Bersihkan memori linked list sebelum reload
static void bersihkanQueue() {
    while (front != nullptr) {
        NodeTugas* temp = front;
        front = front->next;
        delete temp;
    }
    rear = nullptr;
}

// Helper: Masukkan data ke Linked List (di memori)
static void enqueueLinkedList(string tugas) {
    NodeTugas* baru = new NodeTugas;
    baru->deskripsi = tugas;
    baru->next = nullptr;

    if (front == nullptr) {
        front = rear = baru;
    } else {
        rear->next = baru;
        rear = baru;
    }
}

// Baca file -> Masukkan ke Linked List
static void muatAntrian() {
    bersihkanQueue(); // Reset agar tidak duplikat
    ifstream file(FILE_TUGAS_OB);
    string line;

    while (getline(file, line)) {
        if (!line.empty()) {
            enqueueLinkedList(line);
        }
    }
    file.close();
}

// Tulis Linked List -> Timpa ke File
static void simpanAntrian() {
    ofstream file(FILE_TUGAS_OB, ios::trunc);
    
    NodeTugas* temp = front;
    while (temp != nullptr) {
        file << temp->deskripsi << "\n";
        temp = temp->next;
    }
    file.close();
}

// ... (Sisa kode ke bawah sama persis, tidak perlu diubah) ...
// ... Mulai dari Implementasi Struct Laporan dst ...

// ===================== IMPLEMENTASI STRUCT LAPORAN =====================
// Constructor laporan
laporan::laporan(int _id, string _t, string _j, string _l, string _h) {
    id = _id;
    tanggal = _t;
    jenis = _j;
    lama = _l;
    laporanHariIni = _h;
    next = nullptr;
}

// ===================== IMPLEMENTASI CLASS AbsenOb =====================

AbsenOb::AbsenOb() {
    head = nullptr;
}

string AbsenOb::ambilWaktuSekarang() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[10];
    strftime(buffer, sizeof(buffer), "%H:%M:%S", ltm);
    return string(buffer);
}

string AbsenOb::ambilTanggalSekarang() {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    char buffer[11];
    strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
    return string(buffer);
}

void AbsenOb::absenMasuk() {
    int id;
    string nama;

    cout << "Masukkan ID OB: ";
    cin >> id;
    cin.ignore();

    cout << "Masukkan nama OB: ";
    getline(cin, nama);

    string tanggal = ambilTanggalSekarang();
    string waktuMasuk = ambilWaktuSekarang();
    string waktuKeluar = "-";

    Absen* baru = new Absen{id, nama, tanggal, waktuMasuk, waktuKeluar, nullptr};

    if (head == nullptr) {
        head = baru;
        head->next = head; // circular list logic
    } else {
        Absen* temp = head;
        while (temp->next != head)
            temp = temp->next;
        temp->next = baru;
        baru->next = head;
    }

    cout << "✅ " << nama << " berhasil absen masuk pada " << tanggal << " pukul " << waktuMasuk << endl;
    simpankeFile();
}

void AbsenOb::tampilAbsenSendiri(int idCari) {
    if (head == nullptr) {
        cout << "Belum ada data absensi.\n";
        return;
    }

    Absen* temp = head;
    bool ditemukan = false;

    do {
        if (temp->id == idCari) {
            cout << "\n===== Data Absensi =====\n";
            cout << "ID           : " << temp->id << endl;
            cout << "Nama         : " << temp->nama << endl;
            cout << "Tanggal      : " << temp->tanggal << endl;
            cout << "Waktu Masuk  : " << temp->waktuMasuk << endl;
            cout << "Waktu Keluar : " << temp->waktuKeluar << endl;
            ditemukan = true;
        }
        temp = temp->next;
    } while (temp != head);

    if (!ditemukan)
        cout << "Belum ada data absensi untuk ID tersebut.\n";
}

void AbsenOb::absenKeluar() {
    int idCari;
    if (head == nullptr) {
        cout << "Belum ada data absensi.\n";
        return;
    }

    cout << "Masukkan ID anda untuk Absen Keluar: ";
    cin >> idCari;
    Absen* temp = head;
    bool ditemukan = false;

    do {
        if (temp->id == idCari && temp->tanggal == ambilTanggalSekarang()) {
            ditemukan = true;

            if (temp->waktuKeluar != "-") {
                cout << "Anda sudah absen keluar hari ini pada pukul " << temp->waktuKeluar << endl;
                return;
            }

            temp->waktuKeluar = ambilWaktuSekarang();
            cout << "✅ Anda berhasil absen keluar pada pukul " << temp->waktuKeluar << endl;
            simpankeFile();
            return;
        }
        temp = temp->next;
    } while (temp != head);

    if (!ditemukan)
        cout << "Data absen masuk hari ini tidak ditemukan untuk ID tersebut.\n";
}

void AbsenOb::simpankeFile() {
    ofstream file("absenOb.txt");

    if (!file.is_open()) {
        cout << "Gagal menyimpan data absen\n";
        return;
    }

    if (head == nullptr) {
        file.close();
        return;
    }

    Absen* temp = head;
    do {
        file << temp->id << "|"
             << temp->nama << "|"
             << temp->tanggal << "|"
             << temp->waktuMasuk << "|"
             << temp->waktuKeluar << "|\n";
        temp = temp->next;
    } while (temp != head);

    file.close();
}

void AbsenOb::loadDariFile() {
    ifstream file("absenOb.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1 + 1);
        size_t p3 = line.find('|', p2 + 1);
        size_t p4 = line.find('|', p3 + 1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos || p4 == string::npos) continue;

        int id = stoi(line.substr(0, p1));
        string nama = line.substr(p1 + 1, p2 - p1 - 1);
        string tanggal = line.substr(p2 + 1, p3 - p2 - 1);
        string masuk = line.substr(p3 + 1, p4 - p3 - 1);
        string keluar = line.substr(p4 + 1, line.find('|', p4 + 1) - p4 - 1);

        Absen* baru = new Absen{id, nama, tanggal, masuk, keluar, nullptr};

        if (head == nullptr) {
            head = baru;
            head->next = head;
        } else {
            Absen* temp = head;
            while (temp->next != head)
                temp = temp->next;
            temp->next = baru;
            baru->next = head;
        }
    }
    file.close();
}

// ===================== IMPLEMENTASI CLASS MenuOb =====================

MenuOb::MenuOb() {
    head = nullptr;
    loadDariFile();
    // Muat data absen juga saat MenuOb dibuat
    absen.loadDariFile();
}

void MenuOb::loadDariFile() {
    ifstream file("laporan.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t p1 = line.find('|');
        size_t p2 = line.find('|', p1+1);
        size_t p3 = line.find('|', p2+1);
        size_t p4 = line.find('|', p3+1);

        if (p1 == string::npos || p2 == string::npos || p3 == string::npos || p4 == string::npos) continue;

        int id = stoi(line.substr(0, p1));
        string tanggal = line.substr(p1+1, p2-p1-1);
        string jenis = line.substr(p2+1, p3-p2-1);
        string lama = line.substr(p3+1, p4-p3-1);
        string lap = line.substr(p4+1, line.find('|', p4 + 1) - p4 - 1);

        laporan* baru = new laporan(id, tanggal, jenis, lama, lap);

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

void MenuOb::simpanKeFile() {
    ofstream file("laporan.txt");

    laporan* temp = head;
    while (temp != nullptr) {
        file << temp->id << "|"
             << temp->tanggal << "|"
             << temp->jenis << "|"
             << temp->lama << "|"
             << temp->laporanHariIni << "|\n";
        temp = temp->next;
    }

    file.close();
}

int MenuOb::getNextId() {
    int id = 1;
    laporan* temp = head;
    while (temp != nullptr) {
        id = temp->id + 1;
        temp = temp->next;
    }
    return id;
}

void MenuOb::tambahLaporan() {
    int id = getNextId();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string tanggal, jenis, lama, laporanHari;

    cout << "Tanggal (YYYY-MM-DD): ";
    getline(cin, tanggal);
    cout << "Jenis pekerjaan: ";
    getline(cin, jenis);
    cout << "Lama pekerjaan: ";
    getline(cin, lama);
    cout << "Laporan hari ini: ";
    getline(cin, laporanHari);

    laporan* baru = new laporan(id, tanggal, jenis, lama, laporanHari);

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
        cout << "Jenis     : " << temp->jenis << endl;
        cout << "Lama      : " << temp->lama << endl;
        cout << "Laporan   : " << temp->laporanHariIni << endl;
        cout << "--------------------------------------\n";
        temp = temp->next;
    }
}

void MenuOb::hapusLaporan() {
    if (head == nullptr) {
        cout << "Belum ada laporan untuk dihapus.\n";
        return;
    }

    int idHapus;
    cout << "Masukkan ID laporan yang ingin dihapus: ";
    cin >> idHapus;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

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
        cout << "❌ ID laporan tidak ditemukan!\n";
    }
}

void MenuOb::laporKerusakanFasilitas() {
    int id;
    string namaFasilitas, deskripsi;
    cout << "Masukkan ID Fasilitas: ";
    cin >> id;
    cin.ignore();

    cout << "Masukkan Nama Fasilitas: ";
    getline(cin, namaFasilitas);

    cout << "Masukkan Deskripsi Kerusakan: ";
    getline(cin, deskripsi);

    ofstream file("laporan_fasilitas.txt", ios::app);
    if (file.is_open()) {
        file << id << "|" << namaFasilitas << "|" << deskripsi << "|\n";
        file.close();
        cout << "✅ Laporan fasilitas berhasil dikirim!\n";
    } else {
        cout << "❌ Gagal membuka file laporan fasilitas.\n";
    }
}

// ===================== QUEUE TUGAS (LINKED LIST IMPLEMENTATION) =====================

void MenuOb::lihatTugasSaya() {
    // Cek apakah Linked List kosong
    if (front == nullptr) {
        cout << "[INFO] Tidak ada tugas. Anda bisa bersantai!\n";
        return;
    }

    cout << "\n=== Daftar Tugas Yang Harus Dikerjakan (FIFO - Linked List) ===\n";
    
    NodeTugas* temp = front;
    int urutan = 1;
    
    while (temp != nullptr) {
        if (urutan == 1) cout << "[PRIORITAS] ";
        else cout << "            ";

        cout << "Tugas " << urutan++ << ": " << temp->deskripsi << endl;
        temp = temp->next;
    }
}

void MenuOb::selesaikanTugas() {
    // Cek Underflow (Apakah linked list kosong?)
    if (front == nullptr) {
        cout << "[INFO] Tidak ada tugas untuk diselesaikan.\n";
        return;
    }

    // Ambil data dari node paling depan (Front)
    NodeTugas* temp = front;
    string tugasSelesai = temp->deskripsi;

    // Geser Front ke node berikutnya
    front = front->next;

    // Jika setelah digeser Front jadi null, maka Rear juga harus null
    if (front == nullptr) {
        rear = nullptr;
    }

    // Hapus node lama dari memori
    delete temp;

    // Simpan perubahan ke file
    simpanAntrian();

    cout << "\n[SUKSES] Anda telah menyelesaikan tugas:\n";
    cout << ">>> \"" << tugasSelesai << "\" <<<\n";
    cout << "Tugas dihapus dari daftar antrian.\n";
}

// ===================== MAIN MENU =====================

void MenuOb::tampilkanMenu() {
    // Sinkronisasi tugas dari file ke Linked List saat menu dibuka
    muatAntrian();

    int pilihan;
    do {
        cout << "\n========== MENU PETUGAS KEBERSIHAN (OB) ==========\n";
        cout << "1. Tambah Laporan Harian\n";
        cout << "2. Lihat Laporan Harian\n";
        cout << "3. Hapus Laporan Harian\n";
        cout << "4. Lapor Kerusakan Fasilitas\n";
        cout << "5. Absen Masuk\n";
        cout << "6. Absen Keluar\n"; 
        cout << "7. Lihat Data Absen\n";
        cout << "8. Lihat Daftar Tugas (Dari Admin)\n";
        cout << "9. Selesaikan Tugas (Dequeue)\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";

        while (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka: ";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (pilihan) {
            case 1: tambahLaporan(); break;
            case 2: lihatLaporan(); break;
            case 3: hapusLaporan(); break;
            case 4: laporKerusakanFasilitas(); break;
            case 5: absen.absenMasuk(); break;
            case 6: absen.absenKeluar(); break;
            case 7: {
                int idCari;
                cout << "Masukkan ID Anda: ";
                cin >> idCari;
                absen.tampilAbsenSendiri(idCari);
                break;
            }
            case 8: lihatTugasSaya(); break;
            case 9: selesaikanTugas(); break;
            case 0: cout << "Logout berhasil!\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }

    } while (pilihan != 0);
}