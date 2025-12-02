#include "MenuOb.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <limits>

using namespace std;

// ===================== KONFIGURASI QUEUE TUGAS (DARI ADMIN) =====================
const string FILE_TUGAS_OB = "tugas_ob.txt";
const int MAX_QUEUE = 5; 
string queueOb[MAX_QUEUE];
int front = -1;
int rear = -1;

// --- FUNGSI BANTUAN QUEUE: SINKRONISASI ---
void muatAntrian() {
    ifstream file(FILE_TUGAS_OB);
    string line;
    
    front = -1;
    rear = -1;

    while (getline(file, line)) {
        if (!line.empty()) {
            if (front == -1) front = 0;
            rear = (rear + 1) % MAX_QUEUE;
            queueOb[rear] = line;
        }
    }
    file.close();
}

void simpanAntrian() {
    ofstream file(FILE_TUGAS_OB, ios::trunc);
    
    if (front == -1) { 
        file.close(); return; 
    }

    int i = front;
    while (true) {
        file << queueOb[i] << "\n";
        if (i == rear) break;
        i = (i + 1) % MAX_QUEUE; 
    }
    file.close();
}

// ===================== IMPLEMENTASI ABSEN OB =====================
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
        head->next = head; // circular list
    } else {
        Absen* temp = head;
        while (temp->next != head) {
            temp = temp->next;
        }
        temp->next = baru;
        baru->next = head;
    }

    cout << nama << " berhasil absen masuk pada " << tanggal << " pukul " << waktuMasuk << endl;
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
            cout << "\n===== Data Absensi Anda =====\n";
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

// ===================== IMPLEMENTASI MENU OB =====================
MenuOb::MenuOb() {
    head = nullptr;
    loadDariFile();
    // Tidak perlu muatAntrian() disini karena akan dipanggil di tampilkanMenu()
}

void MenuOb::simpanKeFile() {
    ofstream file("laporan.txt");
    laporan* temp = head;
    while (temp != nullptr) {
        file << temp->id << "|"
             << temp->tanggal << "|"
             << temp->JenisPekerjaan << "|"
             << temp->LamaPekerjaan << "|"
             << temp->laporanHariIni << "|\n";
        temp = temp->next;
    }
    file.close();
}

void MenuOb::loadDariFile() {
    ifstream file("laporan.txt");
    if (!file.is_open()) return;

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        size_t pos1 = line.find('|');
        size_t pos2 = line.find('|', pos1 + 1);
        size_t pos3 = line.find('|', pos2 + 1);
        size_t pos4 = line.find('|', pos3 + 1);
        size_t pos5 = line.find('|', pos4 + 1);

        int id = stoi(line.substr(0, pos1));
        string tanggal = line.substr(pos1 + 1, pos2 - pos1 - 1);
        string jenis = line.substr(pos2 + 1, pos3 - pos2 - 1);
        string lama = line.substr(pos3 + 1, pos4 - pos3 - 1);
        string laporanHarian = line.substr(pos4 + 1, pos5 - pos4 - 1);

        laporan* baru = new laporan(id, tanggal, jenis, lama, laporanHarian);
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
    cin.ignore(); // Bersihkan buffer sebelum getline

    string tanggal, jenis, lama, laporanHarian;

    cout << "Masukkan tanggal (DD-MM-YYYY): ";
    getline(cin, tanggal);
    cout << "Masukkan jenis pekerjaan: ";
    getline(cin, jenis);
    cout << "Masukkan lama pekerjaan: ";
    getline(cin, lama);
    cout << "Masukkan laporan hari ini: ";
    getline(cin, laporanHarian);

    laporan* baru = new laporan(id, tanggal, jenis, lama, laporanHarian);

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
        cout << "Jenis     : " << temp->JenisPekerjaan << endl;
        cout << "Lama      : " << temp->LamaPekerjaan << endl;
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
        cout << "❌ Laporan dengan ID tersebut tidak ditemukan.\n";
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
        cout << "✅ Laporan kerusakan fasilitas berhasil dikirim!\n";
    } else {
        cout << "❌ Gagal membuka file laporan fasilitas.\n";
    }
}

// ===================== FITUR BARU: QUEUE TUGAS =====================

void MenuOb::lihatTugasSaya() {
    if (front == -1) {
        cout << "[INFO] Tidak ada tugas. Anda bisa bersantai!\n";
        return;
    }

    cout << "\n=== Daftar Tugas Yang Harus Dikerjakan (FIFO) ===\n";
    int i = front;
    int urutan = 1;
    while (true) {
        if (i == front) cout << "[PRIORITAS] ";
        else cout << "            ";
        
        cout << "Tugas " << urutan++ << ": " << queueOb[i] << endl;

        if (i == rear) break;
        i = (i + 1) % MAX_QUEUE;
    }
}

void MenuOb::selesaikanTugas() {
    if (front == -1) {
        cout << "[INFO] Antrian kosong! Tidak ada tugas untuk diselesaikan.\n";
        return;
    }

    string tugasSelesai = queueOb[front];

    // Logika Dequeue
    if (front == rear) {
        front = -1;
        rear = -1;
    } else {
        front = (front + 1) % MAX_QUEUE;
    }

    simpanAntrian(); // Simpan perubahan ke file

    cout << "\n[SUKSES] Anda telah menyelesaikan tugas:\n";
    cout << ">>> \"" << tugasSelesai << "\" <<<\n";
    cout << "Tugas dihapus dari daftar antrian.\n";
}

// ===================== MAIN MENU LOOP =====================

void MenuOb::tampilkanMenu() {
    // PENTING: Muat tugas setiap kali menu dibuka agar sinkron dengan Admin
    muatAntrian();

    int pilihan;
    do {
        cout << "\n========== MENU OB ==========\n";
        cout << "1. Tambah laporan Harian\n";
        cout << "2. Lihat laporan Harian\n";
        cout << "3. Hapus laporan Harian\n";
        cout << "4. Lapor Kerusakan Fasilitas\n";
        cout << "5. Absen Masuk\n";
        cout << "6. Lihat Data Absen\n";
        cout << "7. Lihat Daftar Tugas (Dari Admin)\n"; // MENU BARU
        cout << "8. Selesaikan Tugas (Dequeue)\n";    // MENU BARU
        cout << "0. Logout\n";
        cout << "Pilih menu: ";
        
        while (!(cin >> pilihan)) {
            cout << "Input tidak valid. Masukkan angka: ";
            cin.clear(); cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Bersihkan newline

        switch (pilihan) {
            case 1: tambahLaporan(); break;
            case 2: lihatLaporan(); break;
            case 3: hapusLaporan(); break;
            case 4: laporKerusakanFasilitas(); break;
            case 5: absen.absenMasuk(); break;
            case 6: {
                int idCari;
                cout << "Masukkan ID Anda untuk melihat data absen: ";
                cin >> idCari;
                absen.tampilAbsenSendiri(idCari);
                break;
            }
            case 7: lihatTugasSaya(); break; // Panggil fungsi queue
            case 8: selesaikanTugas(); break; // Panggil fungsi queue
            case 0: cout << "Logout berhasil!\n"; break;
            default: cout << "Pilihan tidak valid!\n";
        }
    } while (pilihan != 0);
}