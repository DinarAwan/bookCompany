#include "MenuSatpam.h"
#include <iostream>
using namespace std;

void MenuSatpam::tampilkanMenu() {
    int pilihan;
    do {
        cout << "========== MENU OB ==========\n";
        cout << "1. Tambah Data Buku\n";
        cout << "2. Lihat Data Buku\n";
        cout << "3. Hapus Data Buku\n";
        cout << "0. Logout\n";
        cout << "Pilih menu: ";
        cin >> pilihan;

        switch (pilihan) {
            case 1:
                cout << "Menambah data buku...\n";
                break;
            case 2:
                cout << "Menampilkan data buku...\n";
                break;
            case 3:
                cout << "Menghapus data buku...\n";
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
