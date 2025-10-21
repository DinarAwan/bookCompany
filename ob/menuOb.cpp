#include <iostream>
#include <fstream>
#include <string>
using namespace std;

// ===================== CLASS LAPORAN =====================
class laporan {
public:
    int id;
    string JenisPekerjaan;
    string LamaPekerjaan;
    laporan* next;

    laporan(int id, string jenis, string lama) {
        this->id = id;
        this->JenisPekerjaan = jenis;
        this->LamaPekerjaan = lama;
        this->next = nullptr;
    }
};

// ===================== CLASS MENU OB =====================
class MenuOb {
private:
    laporan* head;

    // Simpan ke file TXT
    void simpanKeFile() {
        ofstream file("laporan.txt");
        laporan* temp = head;
        while (temp != nullptr) {
            file << temp->id << "|" << temp->JenisPekerjaan << "|" << temp->LamaPekerjaan << "|\n";
            temp = temp->next;
        }
        file.close();
    }

    // Load dari file TXT ke linked list
    void loadDariFile() {
        ifstream file("laporan.txt");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            size_t pos1 = line.find('|');
            size_t pos2 = line.find('|', pos1 + 1);
            size_t pos3 = line.find('|', pos2 + 1);

            int id = stoi(line.substr(0, pos1));
            string jenis = line.substr(pos1 + 1, pos2 - pos1 - 1);
            string lama = line.substr(pos2 + 1, pos3 - pos2 - 1);

            laporan* baru = new laporan(id, jenis, lama);
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

    // Dapatkan ID berikutnya
    int getNextId() {
        int id = 1;
        laporan* temp = head;
        while (temp != nullptr) {
            id = temp->id + 1;
            temp = temp->next;
        }
        return id;
    }

public:
    MenuOb() {
        head = nullptr;
        loadDariFile();
    }

    // ========== TAMBAH LAPORAN ==========
    void tambahLaporan() {
        int id = getNextId();
        cin.ignore();
        string jenis, lama;

        cout << "Masukkan jenis pekerjaan: ";
        getline(cin, jenis);
        cout << "Masukkan lama pekerjaan: ";
        getline(cin, lama);

        laporan* baru = new laporan(id, jenis, lama);

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

    // ========== LIHAT LAPORAN ==========
    void lihatLaporan() {
        if (head == nullptr) {
            cout << "Belum ada laporan.\n";
            return;
        }

        cout << "\n===== Daftar Laporan =====\n";
        laporan* temp = head;
        while (temp != nullptr) {
            cout << "ID   : " << temp->id << endl;
            cout << "Jenis: " << temp->JenisPekerjaan << endl;
            cout << "Lama : " << temp->LamaPekerjaan << endl;
            cout << "---------------------------\n";
            temp = temp->next;
        }
    }

    // ========== HAPUS LAPORAN ==========
    void hapusLaporan() {
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

    // ========== MENU UTAMA ==========
    void tampilkanMenu() {
        int pilihan;
        do {
            cout << "\n========== MENU OB ==========\n";
            cout << "1. Tambah laporan\n";
            cout << "2. Lihat laporan\n";
            cout << "3. Hapus laporan\n";
            cout << "0. Logout\n";
            cout << "Pilih menu: ";
            cin >> pilihan;

            switch (pilihan) {
                case 1:
                    tambahLaporan();
                    break;
                case 2:
                    lihatLaporan();
                    break;
                case 3:
                    hapusLaporan();
                    break;
                case 0:
                    cout << "Logout berhasil!\n";
                    break;
                default:
                    cout << "Pilihan tidak valid!\n";
            }
        } while (pilihan != 0);
    }
};


