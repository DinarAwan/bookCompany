#include "menuKaryawan.h"
#include <iostream>
using namespace std;

struct Buku {
    int id;
    string judul;
    string penulis;
    int tahun;
    string isbn;
    string kategori;
};

class ManajemenBuku {
    private:
    Buku* bukuDaftar;
    int jumlahBuku;
    int kapasitas;

    void perbesarKapasitas() {
        kapasitas *= 2;
        Buku* bukuBaru = new Buku[kapasitas];
        for (int i = 0; i < jumlahBuku; i++) {
            bukuBaru[i] = bukuDaftar[i];
        }
        delete[] bukuDaftar;
        bukuDaftar = bukuBaru;
    }

    public:
    ManajemenBuku() {
        kapasitas = 10;
        jumlahBuku = 0;
        bukuDaftar = new Buku[kapasitas];
    }

    ~ManajemenBuku() {
        delete[] bukuDaftar;
    }

    void tambahbuku(){
        int jumlah;
        cout<<"============= Tambah Buku ============="<<endl;
        cout<<"masukkan jumnlah buku yang akan ditambahkan: ";
        cin>>jumlah;
        cin.ignore();

        for (int i = 0; i < jumlah; i++) {
        if (jumlahBuku >= kapasitas) {
            perbesarKapasitas();
        }

        Buku bukuBaru;
        cout<< "\nData buku ke-"<< (i+1) << ":" <<endl;
        cout << "Masukkan ID Buku: ";
        cin >> bukuBaru.id;
        cin.ignore();
        cout << "Masukkan Judul Buku: ";
        getline(cin, bukuBaru.judul);
        cout << "Masukkan Penulis Buku: ";
        getline(cin, bukuBaru.penulis);
        cout << "Masukkan Tahun Terbit: ";
        cin >> bukuBaru.tahun;
        cout << "Masukkan ISBN: ";
        cin.ignore();
        getline(cin, bukuBaru.isbn);
        cout << "Masukkan Kategori: ";
        getline(cin, bukuBaru.kategori);

        bukuDaftar[jumlahBuku++] = bukuBaru;
    }
        cout<<"Buku berhasil ditambahkan!"<<endl;
    }

    void lihatbuku(){
        if (jumlahBuku == 0) {
            cout << "Tidak ada data buku tersedia"<<endl;
            return;
        }

        else{
        cout<<"============= Daftar Buku ============="<<endl;
        for (int i = 0; i < jumlahBuku; i++) {
            cout << "Buku ke-" << (i + 1) << ":\n";
            cout << "ID Buku: " << bukuDaftar[i].id<<endl;
            cout << "Judul: " << bukuDaftar[i].judul<<endl;
            cout << "Penulis: " << bukuDaftar[i].penulis<<endl;
            cout << "Tahun Terbit: " << bukuDaftar[i].tahun<<endl;
            cout << "ISBN: " << bukuDaftar[i].isbn<<endl;
            cout << "Kategori: " << bukuDaftar[i].kategori<<endl;
            cout << "-----------------------------------"<<endl;
        }
     }
    }

    void hapusbuku(){
        Buku bukuHapus;
        cout << "Masukkan ID Buku yang akan dihapus: ";
        cin >> bukuHapus.id;

        for (int i = 0; i < jumlahBuku; i++){
            if (bukuDaftar[i].id == bukuHapus.id){
                for(int j = i; j < jumlahBuku - 1; j++){
                    bukuDaftar[j] = bukuDaftar[j+1];
                }
                jumlahBuku--;
                cout<<"Buku dengan ID "<<bukuHapus.id<<" berhasil dihapus."<<endl;
                return;
            }
        }
        cout<<"Buku dengan ID "<<bukuHapus.id<<" tidak ditemukan"<<endl;
    }
};



void MenuKaryawan::tampilkanMenu() {
    int pilihan;
    ManajemenBuku manajemenBuku;
    do {
        cout << "========== MENU Karyawan ==========\n";
        cout << "1. Tambah Data Buku\n";
        cout << "2. Lihat Data Buku\n";
        cout << "3. Hapus Data Buku\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                cout << "Menambah data buku...\n";
                cout<<endl;
               manajemenBuku.tambahbuku();
                break;
            case 2:
                cout << "Menampilkan data buku...\n";
                cout<<endl;
                manajemenBuku.lihatbuku();
                break;
            case 3:
                cout << "Menghapus data buku...\n";
                cout<<endl;
                manajemenBuku.hapusbuku();
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

//cobain program udah jalan atau belum nanti hapus aja
int main () {
    MenuKaryawan menu;
    menu.tampilkanMenu();
    return 0;
}