#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
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

// ===================== CLASS ABSEN OB  =====================
class AbsenOb {
    public: 
    struct Absen {
        int id;
        string nama;
        string tanggal;
        string waktuMasuk;
        string waktuKeluar;
        Absen* next;
    };
    Absen* head;

    AbsenOb() {
        head = NULL;
    }

    string ambilWaktuSekarang(){
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[10];
        strftime(buffer, sizeof(buffer), "%H:%M:%S", ltm);
        return string(buffer);
    }

    string ambilTanggalSekarang(){
        time_t now = time(0);
        tm* ltm = localtime(&now);
        char buffer[11];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d", ltm);
        return string(buffer);
    }

    void absenMasuk() {
        int id;
        string nama;
        cout<<"masukkan id ob: ";
        cin>>id;
        cin.ignore();
        cout<<"masukkan nama ob: ";
        getline(cin,nama);

        string tanggal = ambilTanggalSekarang();
        string waktuMasuk = ambilWaktuSekarang();
        string waktuKeluar = "-";

        Absen* baru = new Absen{id, nama, tanggal, waktuMasuk, waktuKeluar, NULL};

        if (head == NULL) {
            head = baru;
            head->next = head; //ciircular
        } else {
            Absen* temp = head;
            while (temp->next != head) {
                temp = temp->next;
            }
            temp->next = baru;
            baru->next = head; //circular
        }
        cout<<nama<<" berhasil absen masuk pada "<<tanggal<<" pukul "<<waktuMasuk<<endl;
    }

    void tampilAbsenSendiri(int idCari) {
    if (head == NULL) {
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

};

// ===================== CLASS LAPORAN OB =====================
class MenuOb {
private:
    laporan* head;
    AbsenOb absen;

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

    //=========== Lapor Kerusakan Fasilitas ===========
    class Fasilitas {
    public:
    int id;
    string namaFasilitas;
    string deskripsiKerusakan;
    };

    void laporKerusakanFasilitas() {
        Fasilitas fasilitasBaru;
        cout << "Masukkan ID Fasilitas: ";
        cin >> fasilitasBaru.id;
        cin.ignore();

        cout << "Masukkan Nama Fasilitas: ";
        getline(cin, fasilitasBaru.namaFasilitas);

        cout << "Masukkan Deskripsi Kerusakan: ";
        getline(cin, fasilitasBaru.deskripsiKerusakan);

        ofstream file("laporan_fasilitas.txt", ios::app);
        if (file.is_open()) {
            file << fasilitasBaru.id << "|" << fasilitasBaru.namaFasilitas << "|" << fasilitasBaru.deskripsiKerusakan << "|\n";
            file.close();
            cout << "Laporan kerusakan fasilitas berhasil dikirim!\n";
        } else {
            cout << " Gagal membuka file laporan fasilitas.\n";
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
            cout << "4. Lapor Kerusakan Fasilitas\n";
            cout << "5. Absen Masuk\n";
            cout << "6. Lihat Data Absen\n";
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
                case 4:
                    laporKerusakanFasilitas();
                    break;
                case 5:
                absen.absenMasuk();
                break;
                case 6: {
                    int idCari;
                    cout << "Masukkan ID Anda untuk melihat data absen: ";
                    cin >> idCari;
                    absen.tampilAbsenSendiri(idCari);
                    break;
                }
                case 0:
                    cout << "Logout berhasil!\n";
                    break;
                default:
                    cout << "Pilihan tidak valid!\n";
            }
        } while (pilihan != 0);
    }
};

