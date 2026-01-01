#include "menuKaryawan.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

struct Buku {
    int id;
    string judul;
    string penulis;
    int tahun;
    string isbn;
    string kategori;
    string status;
};

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
        cin.ignore();

        cout << "Masukkan nama Karyawan: ";
        getline(cin, nama);

        string tanggal = ambilTanggalSekarang();
        string waktuMasuk = ambilWaktuSekarang();
        string waktuKeluar = "-";

        AbsenK* baru = new AbsenK{id, nama, tanggal, waktuMasuk, waktuKeluar, nullptr};

        if (head == nullptr) {
            head = baru;
            head->next = head; // circular
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

    void tampilAbsenSendiri(int idCari) {
        if (head == nullptr) {
            cout << "Belum ada data absensi.\n";
            return;
        }

        AbsenK* temp = head;
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

     void absenKeluar(){
        int idCari;
        if (head == nullptr) {
            cout << "Belum ada data absensi.\n";
            return;
        }

        cout << "Masukkan ID anda: ";
        cin >> idCari;
        AbsenK* temp = head;
        bool ditemukan = false;

        do{
            if(temp->id == idCari && temp->tanggal == ambilTanggalSekarang()){
              ditemukan = true;

              if(temp->waktuKeluar != "-"){
                cout<<"anda sudah absen keluar hari ini pada pukul "<<temp->waktuKeluar<<endl;
                return;
              }
                temp->waktuKeluar = ambilWaktuSekarang();
                cout<<"anda berhasil absen keluar pada pukul "<<temp->waktuKeluar<<endl;
                return;
            }
            temp = temp->next;
        }while(temp != head);

        if(!ditemukan){
            cout<<"anda belum absen masuk hari ini"<<endl;
        }
        simpankeFile();

    }

    // ===================== SIMPAN & LOAD FILE =====================
    void simpankeFile(){
        ofstream file("absenKaryawan.txt");

        if(!file.is_open()){
            cout<<"gagal menyimpan data absen"<<endl;
            return;
        }

        if(head == nullptr){
            file.close();
            return;
        }

        AbsenK* temp = head;
        do{
            file<<temp->id<<"|"
            <<temp->nama<<"|"
            <<temp->tanggal<<"|"
            <<temp->waktuMasuk<<"|"
            <<temp->waktuKeluar<<"|\n";
            temp = temp->next;
        }while(temp != head);
        file.close();
    }

    void loadDariFile(){
 ifstream file("absenOb.txt");
 if(!file.is_open()){
    return;
 }

    string line;
 while (getline(file, line)) {
            if (line.empty()) continue;

            size_t p1 = line.find('|');
            size_t p2 = line.find('|', p1+1);
            size_t p3 = line.find('|', p2+1);
            size_t p4 = line.find('|', p3+1);

            int id = stoi(line.substr(0, p1));
            string nama = line.substr(p1+1, p2-p1-1);
            string tanggal = line.substr(p2+1, p3-p2-1);
            string masuk = line.substr(p3+1, p4-p3-1);
            string keluar = line.substr(p4+1);

            AbsenK* baru = new AbsenK{id, nama, tanggal, masuk, keluar, nullptr};

            if(head == nullptr){
                head = baru;
                head->next = head;
            } else {
                AbsenK* temp = head;
                while(temp->next != head)
                    temp = temp->next;
                temp->next = baru;
                baru->next = head;
            }
}
    file.close();
}

};

// ========== SINGLE LINKED LIST UNTUK BUKU ==========
class ManajemenBuku {
private:
    struct NodeBuku {
        Buku data;
        NodeBuku* next;
    };

    NodeBuku* head;

    // Fungsi untuk memuat data dari file
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

    // Fungsi untuk menyimpan data ke file
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

public:
    ManajemenBuku() {
        head = nullptr;
        muatDariFile();
    }

    ~ManajemenBuku() {
        simpanKeFile();
        // Hapus semua node
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
            cout << "Input tidak valid. Masukkan angka.\n";
            cin.clear();
            cin.ignore(1000, '\n');
            return;
        }
        if (jumlah <= 0) {
            cout << "Jumlah harus lebih dari 0.\n";
            cin.ignore(1000, '\n');
            return;
        }
        cin.ignore(1000, '\n');

        for (int i = 0; i < jumlah; i++) {
            Buku bukuBaru;
            cout << "\nData buku ke-" << (i + 1) << ":" << endl;

            cout << "Masukkan ID Buku: ";
            while (!(cin >> bukuBaru.id)) {
                cout << "ID harus angka. Coba lagi: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            cin.ignore(1000, '\n');

            cout << "Masukkan Judul Buku: ";
            getline(cin, bukuBaru.judul);

            cout << "Masukkan Penulis Buku: ";
            getline(cin, bukuBaru.penulis);

            cout << "Masukkan Tahun Terbit: ";
            while (!(cin >> bukuBaru.tahun)) {
                cout << "Tahun harus angka. Coba lagi: ";
                cin.clear();
                cin.ignore(1000, '\n');
            }
            cin.ignore(1000, '\n');

            cout << "Masukkan ISBN: ";
            getline(cin, bukuBaru.isbn);

            cout << "Masukkan Kategori: ";
            getline(cin, bukuBaru.kategori);

            bukuBaru.status = "Tersedia";

            // Tambahkan ke linked list
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
        }
        simpanKeFile();
        cout << "Buku berhasil ditambahkan!" << endl;
    }

    void lihatbuku() {
        if (head == nullptr) {
            cout << "Tidak ada data buku tersedia" << endl;
            return;
        }

        cout << "============= Daftar Buku =============" << endl;
        NodeBuku* temp = head;
        int counter = 1;
        while (temp != nullptr) {
            cout << "Buku ke-" << counter << ":\n";
            cout << "ID Buku: " << temp->data.id << endl;
            cout << "Judul: " << temp->data.judul << endl;
            cout << "Penulis: " << temp->data.penulis << endl;
            cout << "Tahun Terbit: " << temp->data.tahun << endl;
            cout << "ISBN: " << temp->data.isbn << endl;
            cout << "Kategori: " << temp->data.kategori << endl;
            cout << "Status: " << temp->data.status << endl;
            cout << "-----------------------------------" << endl;
            temp = temp->next;
            counter++;
        }
    }

    void hapusbuku() {
        int idHapus;
        cout << "Masukkan ID Buku yang akan dihapus: ";
        while (!(cin >> idHapus)) {
            cout << "ID harus angka. Coba lagi: ";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        cin.ignore(1000, '\n');

        // Jika list kosong
        if (head == nullptr) {
            cout << "Buku dengan ID " << idHapus << " tidak ditemukan" << endl;
            return;
        }

        // Jika node pertama yang dihapus
        if (head->data.id == idHapus) {
            NodeBuku* temp = head;
            head = head->next;
            delete temp;
            simpanKeFile();
            cout << "Buku dengan ID " << idHapus << " berhasil dihapus." << endl;
            return;
        }

        // Cari node yang akan dihapus
        NodeBuku* temp = head;
        while (temp->next != nullptr && temp->next->data.id != idHapus) {
            temp = temp->next;
        }

        if (temp->next == nullptr) {
            cout << "Buku dengan ID " << idHapus << " tidak ditemukan" << endl;
            return;
        }

        NodeBuku* nodeHapus = temp->next;
        temp->next = nodeHapus->next;
        delete nodeHapus;
        simpanKeFile();
        cout << "Buku dengan ID " << idHapus << " berhasil dihapus." << endl;
    }

    void pinjamBuku() {
        int idPinjam;
        cout << "============= Pinjam Buku =============" << endl;
        cout << "Masukkan ID Buku yang akan dipinjam: ";
        if (!(cin >> idPinjam)) {
            cout << "Input tidak valid. Masukkan angka.\n";
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
                    cout << "Buku '" << temp->data.judul << "' berhasil dipinjam." << endl;
                } else {
                    cout << "Maaf, buku '" << temp->data.judul << "' sedang dipinjam." << endl;
                }
                return;
            }
            temp = temp->next;
        }
        cout << "Buku dengan ID " << idPinjam << " tidak ditemukan." << endl;
    }

    void kembalikanBuku() {
        int idKembali;
        cout << "============= Kembalikan Buku =============" << endl;
        cout << "Masukkan ID Buku yang akan dikembalikan: ";
        if (!(cin >> idKembali)) {
            cout << "Input tidak valid. Masukkan angka.\n";
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
                    cout << "Buku '" << temp->data.judul << "' berhasil dikembalikan." << endl;
                } else {
                    cout << "Buku '" << temp->data.judul << "' memang berstatus tersedia." << endl;
                }
                return;
            }
            temp = temp->next;
        }
        cout << "Buku dengan ID " << idKembali << " tidak ditemukan." << endl;
    }
};

void MenuKaryawan::tampilkanMenu() {
    int pilihan;
    ManajemenBuku manajemenBuku;
    AbsenKaryawan absenKaryawan;
    do {

        cout << "========== MENU Karyawan ==========\n";
        cout << "1. Tambah Data Buku\n";
        cout << "2. Lihat Data Buku\n";
        cout << "3. Hapus Data Buku\n";
        cout << "4. Kembalikan Buku\n";
        cout << "5. Absen Masuk\n";
        cout << "6. Absen Keluar\n";
        cout << "7. Lihat Data Absen\n";
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
                cout << "Menambah data buku...\n\n";
                manajemenBuku.tambahbuku();
                break;
            case 2:
                cout << "Menampilkan data buku...\n\n";
                manajemenBuku.lihatbuku();
                break;
            case 3:
                cout << "Menghapus data buku...\n\n";
                manajemenBuku.hapusbuku();
                break;
            case 4:
                cout << "Mengembalikan buku...\n\n";
                manajemenBuku.kembalikanBuku();
                break;
            case 5:
                cout << "Absen Masuk...\n\n";
                absenKaryawan.absenMasuk();
                break;
            case 6:
                cout << "Absen Keluar...\n\n";
                absenKaryawan.absenKeluar();
                break;
            case 7:
                cout << "Melihat data absen...\n\n";
                int id;
                cout << "Masukkan ID Karyawan: ";
                cin >> id;
                absenKaryawan.tampilAbsenSendiri(id);
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