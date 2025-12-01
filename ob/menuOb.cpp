#include <iostream>
#include <fstream>
#include <string>
#include <ctime>

using namespace std;

// ===================== ABSENSI OB =====================
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
            head->next = head; // circular
        } else {
            Absen* temp = head;
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

    void absenKeluar(){
        int idCari;
        if (head == nullptr) {
            cout << "Belum ada data absensi.\n";
            return;
        }

        cout << "Masukkan ID anda: ";
        cin >> idCari;
        Absen* temp = head;
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

//==================== SIMPAN DATA ABSEN =====================
    void simpankeFile(){
        ofstream file("absenOb.txt");

        if(!file.is_open()){
            cout<<"gagal menyimpan data absen"<<endl;
            return;
        }

        if(head == nullptr){
            file.close();
            return;
        }

        Absen* temp = head;
        do{
            file<<temp->id<<"|"
            <<temp->nama<<"|"
            <<temp->tanggal<<"|"
            <<temp->waktuMasuk<<"|"
            <<temp->waktuKeluar<<"|\n";
            temp = temp->next;
    }while(temp != head);

        file.close();
};

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

            Absen* baru = new Absen{id, nama, tanggal, masuk, keluar, nullptr};

            if(head == nullptr){
                head = baru;
                head->next = head;
            } else {
                Absen* temp = head;
                while(temp->next != head)
                    temp = temp->next;
                temp->next = baru;
                baru->next = head;
            }
}
    file.close();
}



};
// ===================== LAPORAN PEKERJAAN =====================
struct laporan {
    int id;
    string tanggal;
    string jenis;
    string lama;
    string laporanHariIni;
    laporan* next;

    laporan(int _id, string _t, string _j, string _l, string _h)
        : id(_id), tanggal(_t), jenis(_j), lama(_l), laporanHariIni(_h), next(nullptr) {}
};

// ===================== MENU OB =====================
class MenuOb {
private:
    laporan* head;
    AbsenOb absen;

public:
    MenuOb() {
        head = nullptr;
        loadDariFile();
    }

    void loadDariFile() {
        ifstream file("laporan.txt");
        if (!file.is_open()) return;

        string line;
        while (getline(file, line)) {
            if (line.empty()) continue;

            size_t p1 = line.find('|');
            size_t p2 = line.find('|', p1+1);
            size_t p3 = line.find('|', p2+1);
            size_t p4 = line.find('|', p3+1);

            int id = stoi(line.substr(0, p1));
            string tanggal = line.substr(p1+1, p2-p1-1);
            string jenis = line.substr(p2+1, p3-p2-1);
            string lama = line.substr(p3+1, p4-p3-1);
            string lap = line.substr(p4+1);

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

    void simpanKeFile() {
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

    int getNextId() {
        int id = 1;
        laporan* temp = head;
        while (temp != nullptr) {
            id = temp->id + 1;
            temp = temp->next;
        }
        return id;
    }

    void tambahLaporan() {
        int id = getNextId();
        cin.ignore();

        string t, j, l, h;

        cout << "Tanggal (YYYY-MM-DD): ";
        getline(cin, t);
        cout << "Jenis pekerjaan: ";
        getline(cin, j);
        cout << "Lama pekerjaan: ";
        getline(cin, l);
        cout << "Laporan hari ini: ";
        getline(cin, h);

        laporan* baru = new laporan(id, t, j, l, h);

        if (head == nullptr)
            head = baru;
        else {
            laporan* temp = head;
            while (temp->next != nullptr)
                temp = temp->next;
            temp->next = baru;
        }

        simpanKeFile();
        cout << "Laporan berhasil ditambahkan!\n";
    }

    void lihatLaporan() {
        if (head == nullptr) {
            cout << "Belum ada laporan.\n";
            return;
        }

        laporan* temp = head;

        cout << "\n===== DATA LAPORAN =====\n";
        while (temp != nullptr) {
            cout << "ID: " << temp->id << endl;
            cout << "Tanggal: " << temp->tanggal << endl;
            cout << "Jenis: " << temp->jenis << endl;
            cout << "Lama: " << temp->lama << endl;
            cout << "Laporan: " << temp->laporanHariIni << endl;
            cout << "-------------------------\n";
            temp = temp->next;
        }
    }

    void hapusLaporan() {
        if (head == nullptr) {
            cout << "Belum ada laporan.\n";
            return;
        }

        int id;
        cout << "ID laporan yang ingin dihapus: ";
        cin >> id;

        laporan* temp = head;
        laporan* prev = nullptr;

        while (temp != nullptr && temp->id != id) {
            prev = temp;
            temp = temp->next;
        }

        if (temp == nullptr) {
            cout << "ID tidak ditemukan.\n";
            return;
        }

        if (prev == nullptr)
            head = temp->next;
        else
            prev->next = temp->next;

        delete temp;
        simpanKeFile();

        cout << "Laporan berhasil dihapus!\n";
    }

    void laporKerusakanFasilitas() {
        int id;
        string nama, desk;

        cout << "ID fasilitas: ";
        cin >> id;
        cin.ignore();

        cout << "Nama fasilitas: ";
        getline(cin, nama);

        cout << "Deskripsi: ";
        getline(cin, desk);

        ofstream file("laporan_fasilitas.txt", ios::app);
        file << id << "|" << nama << "|" << desk << "|\n";
        file.close();

        cout << "Laporan fasilitas berhasil dikirim!\n";
    }

    void tampilkanMenu() {
        int p;

        do {
            cout << "\n===== MENU OB =====\n";
            cout << "1. Tambah Laporan\n";
            cout << "2. Lihat Laporan\n";
            cout << "3. Hapus Laporan\n";
            cout << "4. Lapor Kerusakan Fasilitas\n";
            cout << "5. Absen Masuk\n";
            cout << "6. Lihat Absen\n";
            cout << "7. Absen Keluar\n";
            cout << "0. Logout\n";
            cout << "Pilih: ";
            cin >> p;

            switch (p) {
                case 1: tambahLaporan(); break;
                case 2: lihatLaporan(); break;
                case 3: hapusLaporan(); break;
                case 4: laporKerusakanFasilitas(); break;
                case 5: absen.absenMasuk(); break;
                case 6:
                    int id;
                    cout << "Masukkan ID: ";
                    cin >> id;
                    absen.tampilAbsenSendiri(id);
                    break;
                case 7: absen.absenKeluar(); break;
                case 0: cout << "Logout.\n"; break;
                default: cout << "Pilihan tidak valid.\n";
            }

        } while (p != 0);
    }
};

